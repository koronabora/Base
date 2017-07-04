// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include <QSettings>
#include <QObject>
#include <QFile>
#include <QDebug>

#define DEF_FILE_NAME "Settings.ini"

class Settings : public QObject
{
Q_OBJECT
public:
	QString dateFormat;
	quint64 datePosInTable;
	QString baseFile;
	QString sqlDriver;
	QString sqlSelectAllRequest;
	QString sqlInsertRequest;
	quint64 sqlInsertIdPosition;
	QChar sqlDataSymbol;
	quint64 sqlRecordSize;
	quint64 sqlModifyRequestIdPosition;
	QString sqlModifyRequest;
	QString sqlRemoveRequest;
	QString language;

	static Settings* inst();

	void init();
private:
	Settings();
	~Settings();
};