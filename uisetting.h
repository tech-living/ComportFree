#ifndef UISETTING_H
#define UISETTING_H

#include <QWidget>

namespace Ui {
class UiSetting;
}

class UiSetting : public QWidget
{
    Q_OBJECT

public:
    explicit UiSetting(QWidget *parent = nullptr);
    ~UiSetting();

private:
    Ui::UiSetting *ui;
};

#endif // UISETTING_H
