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

    void slotReadData();
    void slotProcessAndDisplayBuffer(); // QTimer::timeout() 시그널에 연결될 슬롯

private:
    Ui::MainWindow *ui;

    QStatusBar *m_statusBar; // QStatusBar 인스턴스 선언


    void loadPorts();
    QSerialPort *m_serialPort;

    QByteArray m_receivedBuffer;         // 수신 버퍼
    QTimer* m_displayTimer = nullptr;    // 디스플레이 업데이트 타이머

};
#endif // MAINWINDOW_H
