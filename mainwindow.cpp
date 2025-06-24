#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "GlobalVariable.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    extern const QString strVerision;

    setWindowTitle("ComportFree " + strVerision);
    setGeometry(300, 100, 475, 315);

    //tabWidget
    ui->ui_mainLayout->setContentsMargins(0, 0, 0, 0); // Remove margin
    ui->ui_mainLayout->setSpacing(0);
    ui->ui_mainLayout->addWidget(ui->tabWidget);

    //ui->tab_basic->setLayout(ui->vLayout_basic_main);
    //ui->vLayout_basic_main->setContentsMargins(0, 15, 0, 15);

    //Basic Widget
    ui->tab_basic->setLayout(ui->vLayout_basic_main);
    m_uiBasic = new UiBasic(ui->tab_basic);
    ui->vLayout_basic_main->addWidget(m_uiBasic);


    //Setting Widget
    ui->tab_setting->setLayout(ui->vLayout_setting_main);
    m_uiSetting = new UiSetting(ui->tab_setting);
    ui->vLayout_setting_main->addWidget(m_uiSetting);


    //About Widget
    ui->tab_about->setLayout(ui->vLayout_about_main);
    m_uiAbout = new UiAbout(ui->tab_about);
    ui->vLayout_about_main->addWidget(m_uiAbout);

    ui->tabWidget->setCurrentIndex(0);

    //Create QStatusBar instance directly and add it to the layout
    m_statusBar = new QStatusBar(this); // Specify the parent widget
    ui->ui_mainLayout->addWidget(m_statusBar);  // Add to the bottom of VBoxLayout

    // Apply style to QStatusBar as well.
    m_statusBar->setStyleSheet(
        "QStatusBar { "
        "   background-color: #e0e0e0; "
        "   border-top: 1px solid #c0c0c0; "
        "   padding: 0px; "
        "   border: 1px solid block; " // Check QStatusBar's own boundaries
        "}"
        "QStatusBar::item { "
        "   border: none; "
        "}"
        );

    m_statusBar->showMessage(tr("Ready"), 0);

}


MainWindow::~MainWindow()
{
    delete ui;
}


























