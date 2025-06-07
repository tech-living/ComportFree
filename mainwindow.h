#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnPortsInfo_clicked();

    void on_btnOpenPort_clicked();

private:
    Ui::MainWindow *ui;

    void loadPorts();
    QSerialPort *_serialPort;
};
#endif // MAINWINDOW_H
