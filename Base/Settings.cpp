// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Settings.h"

Settings::Settings()
{
	dateFormat = "d.M";
	baseFile = "database.sqlite";
	datePosInTable = 5;
	sqlDriver = "QSQLITE";
	sqlSelectAllRequest = "SELECT id, name, surname, tel1, tel2, tel3, birth FROM visitors;";
	sqlInsertRequest = "INSERT INTO visitors VALUES ('?', '?', '?', '?', '?', ?, '?');";
	sqlInsertIdPosition = 5;
	sqlDataSymbol = '?';
	sqlRecordSize = 7;
	sqlModifyRequestIdPosition = 6;
	sqlModifyRequest = "UPDATE visitors SET name='?', surname='?', tel1='?', tel2='?', tel3 = '?', birth = '?' WHERE id = ?;";
	sqlRemoveRequest = "DELETE FROM visitors WHERE id = ?;";
	language = "ru_RU";
}

Settings* Settings::inst() {
	static Settings instance;
	return &instance;
}

Settings::~Settings()
{}

void Settings::init()
{
	QSettings settings(DEF_FILE_NAME, QSettings::IniFormat);
	QFile file(DEF_FILE_NAME);
	if (file.exists())
	{
		// Loading settings from file
		settings.beginGroup("Locale");
		//qDebug() << settings.value("language", language).toString();
		language = settings.value("language", language).toString();
		settings.endGroup();

		settings.beginGroup("Main_database_settings");
		baseFile = settings.value("baseFile", baseFile).toString();
		datePosInTable = settings.value("datePosInTable", datePosInTable).toULongLong();
		sqlDriver = settings.value("sqlDriver", sqlDriver).toString();
		sqlRecordSize = settings.value("sqlRecordSize", sqlRecordSize).toULongLong();
		sqlDataSymbol = settings.value("sqlDataSymbol", sqlDataSymbol).toChar();
		datePosInTable = settings.value("datePosInTable", datePosInTable).toULongLong();
		dateFormat = settings.value("dateFormat", dateFormat).toString();
		settings.endGroup();
		
		settings.beginGroup("Select_all_request");
		sqlSelectAllRequest = settings.value("sqlSelectAllRequest", sqlSelectAllRequest).toString();
		settings.endGroup();

		settings.beginGroup("Insert_new_record_request");
		sqlInsertRequest = settings.value("sqlInsertRequest", sqlInsertRequest).toString();
		sqlInsertIdPosition = settings.value("sqlInsertIdPosition", sqlInsertIdPosition).toULongLong();
		settings.endGroup();

		settings.beginGroup("Modify_record_requst");
		sqlModifyRequest = settings.value("sqlModifyRequest", sqlModifyRequest).toString();
		sqlModifyRequestIdPosition = settings.value("sqlModifyRequestIdPosition", sqlModifyRequestIdPosition).toULongLong();
		settings.endGroup();

		settings.beginGroup("Remove_record_request");
		sqlRemoveRequest = settings.value("sqlRemoveRequest", sqlRemoveRequest).toString();
		settings.endGroup();
	}
	else
	{
		// Saving default settings into file
		settings.beginGroup("Locale");
		settings.setValue("language", language);
		settings.endGroup();
		settings.beginGroup("Main_database_settings");
		settings.setValue("baseFile", baseFile);
		settings.setValue("sqlDriver", sqlDriver);
		settings.setValue("sqlRecordSize", sqlRecordSize);
		settings.setValue("sqlDataSymbol", sqlDataSymbol);
		settings.setValue("datePosInTable", datePosInTable);
		settings.setValue("dateFormat", dateFormat);
		settings.endGroup();
		settings.beginGroup("Select_all_request");
		settings.setValue("sqlSelectAllRequest", sqlSelectAllRequest);
		settings.endGroup();
		settings.beginGroup("Insert_new_record_request");
		settings.setValue("sqlInsertRequest", sqlInsertRequest);
		settings.setValue("sqlInsertIdPosition", sqlInsertIdPosition);
		settings.endGroup();
		settings.beginGroup("Modify_record_requst");
		settings.setValue("sqlModifyRequest", sqlModifyRequest);
		settings.setValue("sqlModifyRequestIdPosition", sqlModifyRequestIdPosition);
		settings.endGroup();
		settings.beginGroup("Remove_record_request");
		settings.setValue("sqlRemoveRequest", sqlRemoveRequest);
		settings.endGroup();
		settings.sync();
	}
}