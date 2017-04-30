#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <omkit/utils.h>
#include <omkit/omkit.h>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->appVersionLabel->setText(getVersion());
    ui->omkitVersionLabel->setText(OMKit::instance().getVersion());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
