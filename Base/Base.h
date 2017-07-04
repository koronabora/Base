// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <QtWidgets/QMainWindow>
#include <QErrorMessage>
#include "ui_Base.h"
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include "Settings.h"
#include <QtXlsx\QtXlsx>
#include <QFileDialog>

using namespace QXlsx;

class Base : public QMainWindow
{
    Q_OBJECT

public:
    Base(QWidget *parent = Q_NULLPTR);

public slots:
	void showMessage(QString text);
	void allLoaded();
	void newRecord(QStringList val, quint64 id);

	void clearButtonPressed();
	void addButtonPressed();
	void showCurButtonPressed();
	void exportButtonPressed();

	void menuDelete();
	void menuEdit();
signals:
	void insertNewUser(QStringList val, quint64 id);
	void modifyUser(QStringList val, quint64 id);
	void removeUser(quint64 id);
protected:
	void contextMenuEvent(QContextMenuEvent* e);
private:
    Ui::BaseClass ui;

	QMenu menu;
	bool editMode = false;
	quint64 editPos = 0;

	QPushButton* clearAllButton;
	QPushButton* addButton;
	QPushButton* exportButton;

	QLineEdit* nameEdit;
	QLineEdit* surNameEdit;
	QLineEdit* tel1Edit;
	QLineEdit* tel2Edit;
	QLineEdit* tel3Edit;
	QLineEdit* birthEdit;
	QTableWidget* tw;
	
	bool checkDate();
	void modifyRecord(QStringList sl, quint64 pos);

	QVector<quint64> ids;
	quint64 lastId = 0;
};
