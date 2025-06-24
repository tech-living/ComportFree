#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStatusBar>

#include "uibasic.h"
#include "uisetting.h"
#include "uiabout.h"

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

private:
    Ui::MainWindow *ui;

    QStatusBar *m_statusBar;

    QString m_strLogData;

    UiBasic *m_uiBasic;
    UiSetting *m_uiSetting;
    UiAbout *m_uiAbout;

};
#endif // MAINWINDOW_H
