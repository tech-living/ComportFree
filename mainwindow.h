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
    void on_btnSend_clicked();
    void on_btnClearTxtBrower_clicked();
    void on_btnClosePort_clicked();

    void slotReadData();
    void slotProcessAndDisplayBuffer(); // QTimer::timeout() 시그널에 연결될 슬롯

private:
    Ui::MainWindow *ui;

    void loadPorts();
    QSerialPort *m_serialPort;

    QByteArray m_receivedBuffer;         // 수신 버퍼
    QTimer* m_displayTimer = nullptr;    // 디스플레이 업데이트 타이머

};
#endif // MAINWINDOW_H
