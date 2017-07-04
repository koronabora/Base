// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Base.h"
#include "SqlWorker.h"
#include <QtWidgets/QApplication>
#include <QThread>
#include <QObject>
#include <QTranslator>
#include <QSettings>
#include "Settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QApplication::setApplicationName("Base");
	QApplication::setOrganizationName("Aqua");

	Settings::inst()->init();

	QTranslator trans;
	trans.load(Settings::inst()->language);
	a.installTranslator(&trans);

    Base gui;
	SqlWorker w;
	QThread* t = new QThread();
	w.moveToThread(t);
	QObject::connect(t, SIGNAL(started()), &w, SLOT(run()));
	QObject::connect(&w, SIGNAL(workFinished()), t, SLOT(quit()));
	//QObject::connect(&t, SIGNAL(finished()), &w, SLOT(deleteLater()));
	QObject::connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));

	QObject::connect(&w, &SqlWorker::showMessage, &gui, &Base::showMessage);
	QObject::connect(&w, &SqlWorker::allLoaded, &gui, &Base::allLoaded);
	QObject::connect(&w, &SqlWorker::newRecord, &gui, &Base::newRecord);
	QObject::connect(&gui, &Base::insertNewUser, &w, &SqlWorker::insertNewUser);
	QObject::connect(&gui, &Base::removeUser, &w, &SqlWorker::removeUser);
	QObject::connect(&gui, &Base::modifyUser, &w, &SqlWorker::modifyUser);

	t->start();
	gui.showMaximized();
    return a.exec();
}
