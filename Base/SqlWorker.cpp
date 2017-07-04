// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SqlWorker.h"

SqlWorker::SqlWorker()
{}

SqlWorker::~SqlWorker()
{
	_base.close();
	_base.removeDatabase(Settings::inst()->sqlDriver);
	emit workFinished();
}

void SqlWorker::run()
{
	QStringList sl;
	bool ok;
	quint64 id = 0;

	_base = QSqlDatabase::addDatabase(Settings::inst()->sqlDriver);
	_base.setDatabaseName(Settings::inst()->baseFile);
	bool res = _base.open();
	if (!res)
		//=base_error_message
		emit showMessage(trUtf8("Database error!"));
	else
	{
		QSqlQuery query = _base.exec(Settings::inst()->sqlSelectAllRequest);
		QSqlError tt = query.lastError();
		if (tt.text().size() > 1)
		{
			emit showMessage(tt.text());
		}
		else
			while (query.next())
			{
				sl.clear();
				id = query.record().value(0).toString().toULongLong(&ok);
				if (ok)
				{
					for (quint64 i = 1; i < Settings::inst()->sqlRecordSize; i++)
						sl.append(query.record().value(i).toString());
					emit newRecord(sl, id);

				}
			}
	}
	emit allLoaded();
}

void SqlWorker::insertNewUser(QStringList val, quint64 id)
{
	QString seq = QString(Settings::inst()->sqlInsertRequest);
	if (seq.length() > 0)
	{
		quint64 k = 0;
		for (quint64 i = 0; i < seq.length(); i++)
			if (seq.at(i) == Settings::inst()->sqlDataSymbol)
			{
				seq.remove(i, 1);
				if (k == Settings::inst()->sqlInsertIdPosition)
				{
					seq.insert(i, QString::number(id));
				}
				else
				{
					if (val.size() > 0)
					{
						seq.insert(i, val.takeFirst());
					}
				}
				k++;
			}
		QSqlQuery q = _base.exec(seq);

		QSqlError tt = q.lastError();
		if (tt.text().size() > 1)
		{
			emit showMessage(tt.text());
		}
	}
}

void SqlWorker::modifyUser(QStringList val, quint64 id)
{
	QString seq = QString(Settings::inst()->sqlModifyRequest);
	if (seq.length() > 0)
	{
		quint64 k = 0;
		for (quint64 i = 0; i < seq.length(); i++)
			if (seq.at(i) == Settings::inst()->sqlDataSymbol)
			{
				seq.remove(i, 1);
				if (k == Settings::inst()->sqlModifyRequestIdPosition)
				{
					seq.insert(i, QString::number(id));
				}
				else
				{
					if (val.size() > 0)
					{
						seq.insert(i, val.takeFirst());
					}
				}
				k++;
			}
		QSqlQuery q = _base.exec(seq);

		QSqlError tt = q.lastError();
		if (tt.text().size() > 1)
		{
			emit showMessage(tt.text());
		}
	}
}

void SqlWorker::removeUser(quint64 id)
{
	QString seq = QString(Settings::inst()->sqlRemoveRequest);
	if (seq.length() > 0)
	{
		quint64 k = 0;
		for (quint64 i = 0; i < seq.length(); i++)
			if (seq.at(i) == Settings::inst()->sqlDataSymbol)
			{
				seq.remove(i, 1);
				seq.insert(i, QString::number(id));
				break;
			}
		QSqlQuery q = _base.exec(seq);
		QSqlError tt = q.lastError();
		if (tt.text().size() > 1)
		{
			emit showMessage(tt.text());
		}
	}
}