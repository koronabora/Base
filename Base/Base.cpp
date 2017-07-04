// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Base.h"

Base::Base(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//=menu_action_edit
	QAction* a1 = menu.addAction(trUtf8("Редактировать"));
	if (a1 != nullptr)
	{
		a1->setParent(this);
		connect(a1, &QAction::triggered, this, &Base::menuEdit);
	}
	//=menu_action_remove
	QAction* a2 = menu.addAction(trUtf8("Удалить"));
	if (a2 != nullptr)
	{
		a2->setParent(this);
		connect(a2, &QAction::triggered, this, &Base::menuDelete);
	}

	clearAllButton = ui.clearAllButton;
	addButton = ui.addButton;
	exportButton = ui.exportButton;

	nameEdit = ui.name;
	surNameEdit = ui.surName;
	tel1Edit = ui.tel0;
	tel2Edit = ui.tel1;
	tel3Edit = ui.tel2;
	birthEdit = ui.birth;
	tw = ui.tableWidget;

	if (tw != nullptr)
	{
		tw->setSelectionBehavior(QAbstractItemView::SelectRows);
		tw->setSelectionMode(QAbstractItemView::SingleSelection);
		tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tw->setEnabled(false);
	}
	if (clearAllButton != nullptr)
	{
		clearAllButton->setEnabled(false);
		connect(clearAllButton, &QPushButton::pressed, this, &Base::clearButtonPressed);
	}
	if (addButton != nullptr)
	{
		addButton->setEnabled(false);
		connect(addButton, &QPushButton::pressed, this, &Base::addButtonPressed);
	}
	if (exportButton != nullptr)
	{
		exportButton->setEnabled(false);
		connect(exportButton, &QPushButton::pressed, this, &Base::exportButtonPressed);
	}
	if (nameEdit != nullptr)
		nameEdit->setEnabled(false);
	if (surNameEdit != nullptr)
		surNameEdit->setEnabled(false);
	if (tel1Edit != nullptr)
		tel1Edit->setEnabled(false);
	if (tel2Edit != nullptr)
		tel2Edit->setEnabled(false);
	if (tel3Edit != nullptr)
		tel3Edit->setEnabled(false);
	if (birthEdit != nullptr)
		birthEdit->setEnabled(false);
}

void Base::showMessage(QString text)
{
	QMessageBox ms;
	ms.setWindowIcon(QIcon(":/Resources/icon.ico"));
	ms.setText(text);
	ms.exec();
	//this->close();
}

void Base::allLoaded()
{
	if (tw != nullptr)
	{
		tw->setEnabled(true);
		if (tw->columnCount() > 0)
		{
			quint64 width = tw->size().width()*1.0 / (tw->columnCount()*1.0);
			for (quint64 i=0; i<tw->columnCount(); i++)
				tw->horizontalHeader()->resizeSection(i, width);
			tw->update();
		}
	}
	if (clearAllButton != nullptr)
		clearAllButton->setEnabled(true);
	if (addButton != nullptr)
		addButton->setEnabled(true);
	if (exportButton != nullptr)
		exportButton->setEnabled(true);
	if (nameEdit != nullptr)
		nameEdit->setEnabled(true);
	if (surNameEdit != nullptr)
		surNameEdit->setEnabled(true);
	if (tel1Edit != nullptr)
		tel1Edit->setEnabled(true);
	if (tel2Edit != nullptr)
		tel2Edit->setEnabled(true);
	if (tel3Edit != nullptr)
		tel3Edit->setEnabled(true);
	if (birthEdit != nullptr)
		birthEdit->setEnabled(true);
}

void Base::newRecord(QStringList val, quint64 id)
{
	if (tw != nullptr && val.size() > Settings::inst()->datePosInTable)
	{
		if (id > lastId)
			lastId = id;
		ids.append(id);

		quint64 s=0;

		QDate cd = QDate::fromString(val.at(Settings::inst()->datePosInTable), Settings::inst()->dateFormat);
		QDate date = QDate::currentDate();
		// Lets find some position to insert with sorting by birth date. Birthdate should be last!
		if (tw->rowCount() > 0)
		{
			bool found = false;
			for (quint64 i = 0; i < tw->rowCount(); i++)
				if (tw->item(i, Settings::inst()->datePosInTable) != nullptr)
				{
					QDate d1 = QDate::fromString(tw->item(i, Settings::inst()->datePosInTable)->text(), Settings::inst()->dateFormat);
					if (cd.dayOfYear() < d1.dayOfYear())
					{
						found = true;
						if (i == 0)
							s = 0;
						else
						{
							s = i;
						}
						break;
					}
				}
			if (!found)
				s = tw->rowCount();
		}
		// Lets move all table down
		tw->insertRow(s);
		// Lets insert new record
		for (quint64 i = 0; i < qMin(val.size(), tw->columnCount()); i++)
		{
			QTableWidgetItem* t = new QTableWidgetItem(val.at(i));
			t->setTextAlignment(Qt::AlignHCenter);
			if (cd.dayOfYear() == date.dayOfYear())
			{
				QFont ff = t->font();
				ff.setBold(true);
				t->setFont(ff);
			}
			tw->setItem(s, i, t);
		}
		tw->update();
	}
}

void Base::clearButtonPressed()
{
	if (nameEdit != nullptr)
		nameEdit->clear();
	if (surNameEdit != nullptr)
		surNameEdit->clear();
	if (tel1Edit != nullptr)
		tel1Edit->clear();
	if (tel2Edit != nullptr)
		tel2Edit->clear();
	if (tel3Edit != nullptr)
		tel3Edit->clear();
	if (birthEdit != nullptr)
		birthEdit->clear();
}

void Base::addButtonPressed()
{
	if (nameEdit != nullptr && surNameEdit != nullptr && tel1Edit != nullptr &&
		tel2Edit != nullptr && tel3Edit != nullptr && birthEdit != nullptr)
	{
		if (nameEdit->text().length() > 0 && tel1Edit->text().length() > 0 && birthEdit->text().length() > 0)
		{
			if (checkDate())
			{
				QStringList sl;
				sl.append(nameEdit->text());
				sl.append(surNameEdit->text());
				sl.append(tel1Edit->text());
				sl.append(tel2Edit->text());
				sl.append(tel3Edit->text());
				sl.append(birthEdit->text());
				clearButtonPressed();
				if (!editMode)
				{
					lastId++;
					newRecord(sl, lastId);
					emit insertNewUser(sl, lastId);
				}
				else
				{
					//=add_button_add
					addButton->setText(trUtf8("Add"));
					if (editPos < ids.size())
					{
						editMode = false;
						emit modifyUser(sl, ids.at(editPos));
						modifyRecord(sl, editPos);
					}
				}
			}
			else
			{
				//=message_box_"Enter name and phone!"_about_date_text
				showMessage(trUtf8("Not valid date!"));
			}
		}
		else
		{
			//=message_box_"Enter name and phone!"_about_all_data
			showMessage(trUtf8("Enter name and phone!"));
		}
	}
}

void Base::contextMenuEvent(QContextMenuEvent* e)
{
	menu.exec(e->globalPos());
}

void Base::showCurButtonPressed()
{
	if (tw != nullptr)
	{
		QDate cd = QDate::currentDate();
		qint64 first = -1;
		qint64 last = -1;
		quint64 curPos = qAbs(tw->currentRow());
		if (tw->rowCount() > 0)
		{
			bool found = false;
			for (quint64 i = 0; i < tw->rowCount(); i++)
			{
				QTableWidgetItem* t = tw->item(i, Settings::inst()->datePosInTable);
				if (t != nullptr)
				{
					QDate d = QDate::fromString(t->text(), Settings::inst()->dateFormat);
					if (d.dayOfYear() == cd.dayOfYear())
					{
						if (first < 0)
							first = i;
						last = i;
						found = true;
						for (quint64 k = 0; k < tw->columnCount(); k++)
						{
							t = tw->item(i, k);
							if (t != nullptr)
							{
								QFont ff = t->font();
								ff.setBold(true);
								t->setFont(ff);
							}
						}
					}
					else
					{
						for (quint64 k = 0; k < tw->columnCount(); k++)
						{
							t = tw->item(i, k);
							if (t != nullptr)
							{
								QFont ff = t->font();
								ff.setBold(false);
								t->setFont(ff);
							}
						}
					}
				}
			}
			if (!found)
			{
				//=message_box_"Enter name and phone!"_about_no_cur_day_data
				showMessage(trUtf8("Nothing found!"));
			}
		}
	}
}

bool Base::checkDate()
{
	if (birthEdit != nullptr)
	{
		QDate d = QDate::fromString(birthEdit->text(), Settings::inst()->dateFormat);
		return d.isValid();
	}
	return false;
}

void Base::menuDelete()
{
	if (tw != nullptr)
	{
		quint64 pos = tw->currentRow();
		if (pos < ids.size())
		{
			emit removeUser(ids.takeAt(pos));
			tw->removeRow(pos);
		}
	}
}

void Base::menuEdit()
{
	if (tw != nullptr && nameEdit != nullptr && surNameEdit != nullptr && tel1Edit != nullptr &&
		tel2Edit != nullptr && tel3Edit != nullptr && birthEdit != nullptr)
	{
		quint64 pos = tw->currentRow();
		if (pos < ids.size())
		{
			//=add_button_"Save"
			addButton->setText(trUtf8("Save"));
			editMode = true;
			editPos = pos;
			if (tw->item(pos, 0) != nullptr)
			{
				nameEdit->setText(tw->item(pos, 0)->text());
			}
			if (tw->item(pos, 1) != nullptr)
			{
				surNameEdit->setText(tw->item(pos, 1)->text());
			}
			if (tw->item(pos, 2) != nullptr)
			{
				tel1Edit->setText(tw->item(pos, 2)->text());
			}
			if (tw->item(pos, 3) != nullptr)
			{
				tel2Edit->setText(tw->item(pos, 3)->text());
			}
			if (tw->item(pos, 4) != nullptr)
			{
				tel3Edit->setText(tw->item(pos, 4)->text());
			}
			if (tw->item(pos, 5) != nullptr)
			{
				birthEdit->setText(tw->item(pos, 5)->text());
			}
		}
	}
}

void Base::modifyRecord(QStringList sl, quint64 pos)
{
	if (tw != nullptr)
	{
		if (pos < tw->rowCount())
		{
			tw->removeRow(pos);
			newRecord(sl, pos);
		}
	}
}

void Base::exportButtonPressed()
{
	if (tw != nullptr)
	{
		Document file;
		int x = tw->rowCount();
		int y = tw->columnCount();
		if (y > 0)
		{
			//=file_dialog_caption
			QString fileName = QFileDialog::getSaveFileName(this, trUtf8("Choose File"), QDir::homePath());
			if (fileName.size() > 0)
			{
				if (fileName.right(5).compare(".xlsx") != 0)
					fileName.append(".xlsx");
				for (int j = 0; j < y; j++)
				{
					QTableWidgetItem* t = tw->horizontalHeaderItem(j);
					if (t != nullptr)
					{
						Format hFmt;
						hFmt.setFontBold(true);
						file.write(1, j + 1, t->text(), hFmt);
						file.setColumnWidth(j + 1, 20);
					}
				}
				for (int i = 0; i < x; i++)
					for (int j = 0; j < y; j++)
					{
						QTableWidgetItem* t = tw->item(i, j);
						if (t != nullptr)
						{
							file.write(i + 2, j + 1, t->text());
						}
					}
				file.saveAs(fileName);
				//=message_box_all_done
				showMessage(trUtf8("Сохранено!"));
			}
			else
			{
				//=message_box_nothing_saved
				showMessage(trUtf8("Ничего не было сохранено!"));
			}
		}
		else
		{
			//=message_box_no_data_in_table
			showMessage(trUtf8("Table is clear!"));
		}
	}
}