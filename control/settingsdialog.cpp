#include "settingsdialog.h"
#include "settings.h"
#include "section_utils.h"
#include "solution_utils.h"
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

    connect(ui->startWizardButton, SIGNAL(clicked()),
            this, SIGNAL(requestedSettingsWizard()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::initUI()
{
    const auto& settings = Settings::instance();
    if (isInited) {
        if (ui->sectionsPathEdit->text() != settings.sectionsPath)
            saveSections();
        if (ui->solutionsPathEdit->text() != settings.solutionsPath)
            saveLocalSolutionsToRemoteDir();
        if (ui->groupsPathEdit->text() != settings.groupsPath)
            emit groupsPathChanged();
    }
    ui->sectionsPathEdit->setText(settings.sectionsPath);
    ui->solutionsPathEdit->setText(settings.solutionsPath);
    ui->groupsPathEdit->setText(settings.groupsPath);
    isInited = true;
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

    auto groupsPath = ui->groupsPathEdit->text();
    if (!QFileInfo(groupsPath).dir().exists()) {
        if (!createDirDialog(this, QFileInfo(groupsPath).dir().absolutePath(),
                             "для файла со списком групп"))
            return;
    }

    auto settings = Settings::instance();
    settings.sectionsPath = sectionsPath;
    settings.solutionsPath = solutionsPath;
    settings.groupsPath = groupsPath;
    if (!settings.write()) {
        QMessageBox::warning(this, "Не удалось сохранить настройки",
                             "Не удалось перезаписать файл с настройками.");
        return;
    }
    bool didSectionsPathChanged =
            Settings::instance().sectionsPath != settings.sectionsPath;
    bool didSolutionsPathChanged =
            Settings::instance().solutionsPath != settings.solutionsPath;
    bool didGroupsPathChanged =
            Settings::instance().groupsPath != settings.groupsPath;
    Settings::instance() = settings;
    if (didSectionsPathChanged)
        saveSections();
    if (didSolutionsPathChanged)
        saveLocalSolutionsToRemoteDir();
    if (didGroupsPathChanged)
        emit groupsPathChanged();
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

void SettingsDialog::on_chooseGroupsPathButton_clicked()
{
    QString groupsPath =
            QFileDialog::getSaveFileName(this, "Путь к файлу со списком групп",
                                         ui->groupsPathEdit->text(), "JSON-файл (*.json)");
    if (!groupsPath.isEmpty())
        ui->groupsPathEdit->setText(groupsPath);
}
