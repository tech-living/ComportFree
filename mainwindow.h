#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSerialPortInfo>
#include <QScrollBar>
#include <QStatusBar>

#include "CSerialport.h"

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
    void slotBtnPortsInfo();
    void slotBtnOpenPort();
    void slotBtnSend();
    void slotBtnClearLog();
    void slotBtnClosePort();
    void slotBtnSaveLog();

    void slotReadData(QString _data);


private:
    Ui::MainWindow *ui;

    QStatusBar *m_statusBar;

    void loadPorts();
    //QSerialPort *m_serialPort;
    CSerialport m_csPort;

    QString m_strLogData;

};
#endif // MAINWINDOW_H
