#include "totalpage.h"
#include "user_utils.h"
#include "ui_totalpage.h"
#include <omkit/section.h>
#include <omkit/html_utils.h>
#include <QFileDialog>

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
    sectionPath = section.path;
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
    ui->stackedWidget->setCurrentWidget(ui->createArchiveWidget);
}

void TotalPage::on_archiveButton_clicked()
{
    QString zipArchiveName = userName() + " " + QFileInfo(sectionPath).baseName() + ".zip";
    QString startPath = QDir::homePath() + "/" + zipArchiveName;
    QString path = QFileDialog::getSaveFileName(
                this, "Путь к архиву", startPath, "Архив (*.zip)");
    if (!path.isEmpty())
        emit requestedArchiveSave(path);
}
