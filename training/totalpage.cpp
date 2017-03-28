#include "totalpage.h"
#include "ui_totalpage.h"
#include <omkit/section.h>
#include <omkit/html_utils.h>

TotalPage::TotalPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TotalPage)
{
    ui->setupUi(this);

    connect(ui->transferButton, SIGNAL(clicked()), this, SIGNAL(requestedTransfer()));
}

TotalPage::~TotalPage()
{
    delete ui;
}

void TotalPage::load(const Section& section)
{
    if (section.totalFileName.isEmpty()) {
        ui->totalWidget->hide();
        return;
    }

    QDir sectionDir = section.dir();
    QString totalHTML = readHTML(sectionDir.absoluteFilePath(section.totalFileName));
    if (totalHTML.isEmpty()) {
        ui->totalWidget->hide();
        return;
    }
    ui->totalBrowser->setHtml(totalHTML);
    if (ui->totalBrowser->document()->isEmpty())
        ui->totalWidget->hide();
}

void TotalPage::setSuccess()
{
    ui->stackedWidget->setCurrentWidget(ui->successWidget);
}

void TotalPage::setTransferError()
{
    ui->stackedWidget->setCurrentWidget(ui->transferErrorWidget);
}

void TotalPage::setUnknownState()
{
    ui->stackedWidget->setCurrentWidget(ui->unknownStateWidget);
}
