#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QByteArray>
#include <QTimer>
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
    void slotBtnClearTxtBrower();
    void slotBtnClosePort();

    void slotReadData(QByteArray _data);
    //void slotProcessAndDisplayBuffer(); // QTimer::time

private:
    Ui::MainWindow *ui;

    QStatusBar *m_statusBar;


    void loadPorts();
    //QSerialPort *m_serialPort;
    CSerialport m_csPort;

    QByteArray m_receivedBuffer;         // receive buffer
    QTimer* m_displayTimer = nullptr;    // Display update Timer

};
#endif // MAINWINDOW_H
