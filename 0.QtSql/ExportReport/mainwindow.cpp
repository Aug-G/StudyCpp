#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QSqlDatabase"
#include "QSqlQuery"
#include "QIcon"
#include "QDebug"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlQuery query;
    query.exec("select branch_no,branch_name from bi_t_branch_info order by branch_no asc");
    while(query.next()){
        ui->cbWareH->addItem(query.value("branch_name").toString(), query.value("branch_no"));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnExport_clicked()
{

}
