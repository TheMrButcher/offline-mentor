#include "settingsdialog.h"
#include "settings.h"
#include "ui_settingsdialog.h"
#include <omkit/ui_utils.h>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::initUI()
{
    const auto& settings = Settings::instance();
    ui->sectionsPathEdit->setText(settings.sectionsPath);
    ui->solutionsPathEdit->setText(settings.solutionsPath);
}

void SettingsDialog::accept()
{
    auto sectionsPath = ui->sectionsPathEdit->text();
    if (sectionsPath.isEmpty()) {
        QMessageBox::warning(this, "Неверные данные",
                             "Не указан путь к папке для кейсов.");
        return;
    }
    if (!QFileInfo(sectionsPath).isDir()) {
        if (!createDirDialog(this, sectionsPath, "для кейсов"))
            return;
    }

    auto solutionsPath = ui->solutionsPathEdit->text();
    if (!solutionsPath.isEmpty() && !QFileInfo(solutionsPath).isDir()) {
        if (!createDirDialog(this, solutionsPath, "для ответов"))
            return;
    }

    auto settings = Settings::instance();
    settings.sectionsPath = sectionsPath;
    settings.solutionsPath = solutionsPath;
    if (!settings.write()) {
        QMessageBox::warning(this, "Не удалось сохранить настройки",
                             "Не удалось перезаписать файл с настройками.");
        return;
    }
    Settings::instance() = settings;
    QDialog::accept();
}

void SettingsDialog::on_chooseSectionsPathButton_clicked()
{
    QString sectionsPath =
            QFileDialog::getExistingDirectory(this, "Путь к папке для кейсов",
                                              ui->sectionsPathEdit->text());
    if (!sectionsPath.isEmpty())
        ui->sectionsPathEdit->setText(sectionsPath);
}

void SettingsDialog::on_chooseSolutionsPathButton_clicked()
{
    QString solutionsPath =
            QFileDialog::getExistingDirectory(this, "Путь к папке для ответов",
                                              ui->solutionsPathEdit->text());
    if (!solutionsPath.isEmpty())
        ui->solutionsPathEdit->setText(solutionsPath);
}
