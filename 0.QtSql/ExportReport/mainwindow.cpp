#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QSqlDatabase"
#include "QSqlQuery"
#include "QIcon"
#include "QDebug"
#include "QDateTime"
#include "QFileDialog"
#include "QMessageBox"
#include "QAxObject"
#include "QDir"

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
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Excel"),".",tr("Microsoft Office 2003(*.xls)"));
    if(fileName.isEmpty()){
        QMessageBox::critical(0,tr("错误"),tr("要保存的文件名为空"));
        return;
    }

    QAxObject *excel = new QAxObject("Excel.Appcation");
    excel->dynamicCall("SetVisible(bool)",false);
    excel->setProperty("Visible", false);
    QAxObject *workbooks = excel->querySubObject("Workbooks");
    workbooks->dynamicCall("Add");
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject *worksheet = workbook->querySubObject("WorkSheet(int)", 1);
    int row = 1;

    QSqlQuery query;
    QDate date;
    query.exec(QString("execute dbo.pr_report_jxc_item;1 @as_from = '%1', @as_to = '%2', @as_branch = '%3', @as_itemcls = '%'")
               .arg(date.toString("yyyy-MM-dd"))
               .arg(date.toString("yyyy-MM-dd"))
               .arg(ui->cbWareH->currentData().toString()));
    while(query.next()){

        setCellValue(worksheet,row,1,query.value("cItem_C").toString());
        setCellValue(worksheet,row,2,query.value("cItem_N").toString());
        setCellValue(worksheet,row,3,"0");
        setCellValue(worksheet,row,4,query.value("nSettleQty").toString());
        row ++;
    }
    workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));
    QMessageBox::information(this,tr("OK"),tr("保存成功！"));
    workbook->dynamicCall("Close()");
    worksheet->clear();
    excel->dynamicCall("Quit()");
    delete excel;
}

void MainWindow::setCellValue(QAxObject *worksheet,int row,int column,const QString &value){
    QAxObject *range = worksheet->querySubObject("Cells(int,int)",row,column);
    range->setProperty("Value", value);
    delete range;
}
