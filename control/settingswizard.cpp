#include "settingswizard.h"
#include "ui_settingswizard.h"
#include "settings.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDir>

namespace {
bool makeDir(QWidget* parent, QDir dir, QString path, QString nameUI)
{
    QFileInfo fileInfo(dir, path);
    bool needCreate = true;
    if (fileInfo.exists()) {
        if (fileInfo.isDir()) {
            needCreate = false;
        } else {
            if (!dir.remove(path)) {
                QMessageBox::warning(parent, "Ошибка при сохранении",
                                     "Невозможно создать папку " + nameUI + ".");
                return false;
            }
        }
    }
    if (needCreate) {
        if (!dir.mkdir(path)) {
            QMessageBox::warning(parent, "Ошибка при сохранении",
                                 "Невозможно создать папку " + nameUI + ".");
            return false;
        }
    }
    return true;
}

bool ensureNotDir(QWidget* parent, QDir dir, QString path, QString nameUI)
{
    QFileInfo fileInfo(dir, path);
    if (fileInfo.exists()) {
        if (fileInfo.isDir()) {
            QDir dirToRemove(dir.absoluteFilePath(path));
            if (!dirToRemove.removeRecursively()) {
                QMessageBox::warning(parent, "Ошибка при сохранении",
                                     "Невозможно создать файл " + nameUI + ".");
                return false;
            }
        }
    }
    return true;
}
}

SettingsWizard::SettingsWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWizard)
{
    ui->setupUi(this);
}

SettingsWizard::~SettingsWizard()
{
    delete ui;
}

void SettingsWizard::restart()
{
    prevPages.clear();
    ui->stackedWidget->setCurrentWidget(ui->introPage);
    updateButtons();
}

void SettingsWizard::closeEvent(QCloseEvent* event)
{
    int answer = QMessageBox::question(
                this, "Подтверждение",
                "Процесс настройки не завершен. Вы уверены, "
                "что хотите его прервать?");
    if (answer == QMessageBox::Yes)
        QDialog::closeEvent(event);
    else
        event->ignore();
}

void SettingsWizard::goTo(QWidget* nextPage)
{
    prevPages.append(ui->stackedWidget->currentWidget());
    ui->stackedWidget->setCurrentWidget(nextPage);
    updateButtons();
}

void SettingsWizard::updateButtons()
{
    if (ui->stackedWidget->currentWidget() == ui->finishPage) {
        ui->nextButton->setText("Завершить");
    } else {
        ui->nextButton->setText("Далее");
    }

    ui->prevButton->setEnabled(!prevPages.isEmpty());
}

bool SettingsWizard::createCommonDirectory(QString path)
{
    if (path.isEmpty()) {
        QMessageBox::warning(this, "Неверные данные",
                             "Введите путь к общей папке.");
        return false;
    }
    QFileInfo fileInfo(path);
    if (fileInfo.isDir()) {
        QDir dir(path);
        if (dir.exists("sections") || dir.exists("solutions") || dir.exists("Groups.json")) {
            auto answer = QMessageBox::question(
                        this, "Папка не пуста",
                        "Выбранная папка уже существует и содержит другие файлы. "
                        "Вы уверены, что хотите использовать выбранную папку как общую?");
            if (answer != QMessageBox::Yes)
                return false;
        }
    } else {
        if (!QDir().mkpath(path)) {
            QMessageBox::warning(this, "Ошибка при сохранении",
                                 "Невозможно создать общую папку. Проверьте правильность "
                                 "написания пути.");
            return false;
        }
    }
    return true;
}

bool SettingsWizard::applySettings()
{
    auto& settings = Settings::instance();
    if (ui->homeOption->isChecked()) {
        if (!makeDir(this, QDir(), "sections", "для разделов"))
            return false;
        settings.sectionsPath = QDir().absoluteFilePath("sections");
        settings.solutionsPath = "";
        settings.groupsPath = "";
        settings.write();
        return true;
    }

    if (ui->networkOption->isChecked()) {
        QDir dir(ui->pathEdit->text());
        if (!dir.exists()) {
            QMessageBox::warning(this, "Ошибка при сохранении",
                                 "Выбранная общая папка отсутствует. "
                                 "Попробуйте выбрать заново.");
            return false;
        }

        if (!makeDir(this, dir, "sections", "для разделов")
            || !makeDir(this, dir, "solutions", "для ответов")
            || !ensureNotDir(this, dir, "Groups.json", "со списком групп"))
            return false;

        settings.sectionsPath = dir.absoluteFilePath("sections");
        settings.solutionsPath = dir.absoluteFilePath("solutions");
        settings.groupsPath = dir.absoluteFilePath("Groups.json");
        settings.write();
        return true;
    }

    return false;
}

void SettingsWizard::on_cancelButton_clicked()
{
    close();
}

void SettingsWizard::on_prevButton_clicked()
{
    auto page = prevPages.back();
    prevPages.pop_back();
    ui->stackedWidget->setCurrentWidget(page);
    updateButtons();
}

void SettingsWizard::on_nextButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == ui->introPage) {
        goTo(ui->usageSelectionPage);
        return;
    }

    if (ui->stackedWidget->currentWidget() == ui->usageSelectionPage) {
        if (ui->networkOption->isChecked())
            goTo(ui->networkPathSelectionPage);
        if (ui->homeOption->isChecked())
            goTo(ui->finishPage);
        return;
    }

    if (ui->stackedWidget->currentWidget() == ui->networkPathSelectionPage) {
        QString path = ui->pathEdit->text();
        if (createCommonDirectory(path))
            goTo(ui->finishPage);
        return;
    }

    if (ui->stackedWidget->currentWidget() == ui->finishPage) {
        if (applySettings())
            accept();
        return;
    }
}

void SettingsWizard::on_choosePathButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(
                this, "Путь к папке", ui->pathEdit->text());
    if (!path.isEmpty())
        ui->pathEdit->setText(path);
}
