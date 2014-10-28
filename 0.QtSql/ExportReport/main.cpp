#include "mainwindow.h"
#include <QApplication>

#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QSettings>

#include <QDebug>

void OpenDatabse()
{
    QSettings config("config.ini",QSettings::IniFormat);
    config.beginGroup("sql");
    config.setValue("server","localhost");
    qDebug() << config.value("server").toString();
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                               "SERVER=%1;" //服务器名称
                               "DATABASE=%2;"//数据库名
                               "UID=%3;"           //登录名
                               "PWD=%4;"        //密码
                               ).arg(config.value("/sql/server").toString())
                       .arg(config.value("/sql/databse").toString())
                       .arg(config.value("/sql/uid").toString())
                       .arg(config.value("/sql/pwd").toString())
                       );

     if(!db.open()){
        QMessageBox::critical(0,qApp->tr("Cannot open database"),db.lastError().databaseText(),QMessageBox::Cancel);
    }

}

int main(int argc, char *argv[])
{
    OpenDatabse();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
