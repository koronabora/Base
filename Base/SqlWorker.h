// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QSqlResult>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "Settings.h"

class SqlWorker : public QObject
{
	Q_OBJECT
public:
	SqlWorker();
	~SqlWorker();

private:
	QSqlDatabase _base;
public slots :
	void run();
	void insertNewUser(QStringList val, quint64 id);
	void modifyUser(QStringList val, quint64 id);
	void removeUser(quint64 id);
signals:
	void workFinished();
	void showMessage(QString text);
	void newRecord(QStringList val, quint64 id);
	void allLoaded();
};