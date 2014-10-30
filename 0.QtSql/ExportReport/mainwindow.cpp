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
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("盘点报表导出程序");
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
    QDir dir(QDir::currentPath());
    QDate date;
    QSettings config(dir.currentPath()+"//app.ini",QSettings::IniFormat);
    QString path = config.value("/SETTINGS/path").toString();

    QString fileName = QFileDialog::getSaveFileName(this,tr("保存文件"),path + "/" + date.currentDate().toString("yyyy-MM-dd")+ui->cbWareH->currentText()+".xls",tr("Microsoft Office 2003(*.xls)"));
    if(fileName.isEmpty()){
        QMessageBox::critical(0,tr("错误"),tr("要保存的文件名为空"));
        return;
    }
    QSqlQuery query;

    query.exec(QString("execute dbo.pr_report_jxc_item;1 @as_from = '%1', @as_to = '%2', @as_branch = '%3', @as_itemcls = '%'")
               .arg(date.currentDate().toString("yyyy-MM-dd"))
               .arg(date.currentDate().toString("yyyy-MM-dd"))
               .arg(ui->cbWareH->currentData().toString()));

    QAxObject *excel = new QAxObject("Excel.Application");

    if(!excel){
        QMessageBox::critical(0,tr("错误"),tr("找不到Excel组件！"));
        delete excel;
        return;
    }

    excel->dynamicCall("SetVisible(bool)",false);
    excel->setProperty("Visible", false);
    excel->setProperty("DisplayAlerts", false);

    QAxObject *workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Add");
    QAxObject *workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject *worksheets = workbook->querySubObject("Sheets");
    QAxObject *worksheet = worksheets->querySubObject("Item(int)",1);
    int row = 1;


    while(query.next()){
        setCellValue(worksheet,row,"A","'"+query.value("cItem_C").toString());
        setCellValue(worksheet,row,"B","'"+query.value("cItem_N").toString());
        setCellValue(worksheet,row,"C","0");
        setCellValue(worksheet,row,"D",query.value("nSettleQty"));
        row ++;
    };

    workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));
    config.setValue("/SETTINGS/path",fileName.left(fileName.lastIndexOf("/")));
    QMessageBox::information(this,tr("操作成功"),tr("保存成功！"));
    workbook->dynamicCall("Close()");
    worksheet->clear();
    excel->dynamicCall("Quit()");
    delete excel;
}

void MainWindow::setCellValue(QAxObject *worksheet,int row,const QString &column,const QVariant &value){
    QAxObject *cell = worksheet->querySubObject("Range(QVariant, QVariant)",column+QString::number(row));
    cell->dynamicCall("SetValue(const QVariant&)", value);
}
