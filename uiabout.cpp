#include "uiabout.h"
#include "ui_uiabout.h"
#include <QDateTime>

#include "GlobalVariable.h"

UiAbout::UiAbout(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiAbout)
{
    ui->setupUi(this);

    extern const QString strVerision;
    const QString strDate = QDateTime::currentDateTime().toString("yyyy.mm.dd");

    QString htmlContent = "<h1>ComportFree</h1>"
                          "<p><b>Copyright: </b> tech-living </p>"
                          "<p><b>Version: </b>" + strVerision + "</p>"
                                          "<p><b>Built on : </b>" + strDate +
                          "<p><b>github:</b> <a href=\"https://github.com/tech-living/ComportFree\">tech-living/ComportFree</a></p>";


    ui->textBrowser->setHtml(htmlContent);
    ui->textBrowser->setReadOnly(true);
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setFrameShape(QFrame::NoFrame); // 프레임 제거
}

UiAbout::~UiAbout()
{
    delete ui;
}
