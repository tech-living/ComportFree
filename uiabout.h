#ifndef UIABOUT_H
#define UIABOUT_H

#include <QWidget>

namespace Ui {
class UiAbout;
}

class UiAbout : public QWidget
{
    Q_OBJECT

public:
    explicit UiAbout(QWidget *parent = nullptr);
    ~UiAbout();

private:
    Ui::UiAbout *ui;
};

#endif // UIABOUT_H
