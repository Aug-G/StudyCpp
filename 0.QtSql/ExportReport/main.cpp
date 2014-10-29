#include "mainwindow.h"
#include <QApplication>

#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QSettings>

#include <QDebug>
#include <QDir>

void OpenDatabse()
{
    QDir dir(QDir::currentPath());
    QSettings config(dir.currentPath()+"//app.ini",QSettings::IniFormat);
    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                               "SERVER=%1;" //服务器名称
                               "DATABASE=%2;"//数据库名
                               "UID=%3;"           //登录名
                               "PWD=%4;"        //密码
                               ).arg(config.value("/SQL/server").toString())
                       .arg(config.value("/SQL/database").toString())
                       .arg(config.value("/SQL/uid").toString())
                       .arg(config.value("/SQL/pwd").toString())
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
