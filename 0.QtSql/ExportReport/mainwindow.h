#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAxObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnExport_clicked();


private:
    Ui::MainWindow *ui;
    void setCellValue(QAxObject *worksheet,int row, const QString &column, const QVariant &value);
};

#endif // MAINWINDOW_H
