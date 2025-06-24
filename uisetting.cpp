#include "uisetting.h"
#include "ui_uisetting.h"

UiSetting::UiSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiSetting)
{
    ui->setupUi(this);
}

UiSetting::~UiSetting()
{
    delete ui;
}
