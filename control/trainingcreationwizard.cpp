#include "trainingcreationwizard.h"
#include "section_utils.h"
#include "group_utils.h"
#include "settings.h"
#include "ui_trainingcreationwizard.h"
#include <omkit/utils.h>
#include <omkit/trainingsettings.h>
#include <omkit/zip_utils.h>
#include <omkit/ui_utils.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QTemporaryDir>

namespace {
const QString TRAINING_SRC_DIR = "bin/training_files";
}

TrainingCreationWizard::TrainingCreationWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrainingCreationWizard)
{
    ui->setupUi(this);

    connect(ui->createGroupButton, SIGNAL(clicked()),
            this, SIGNAL(groupCreationRequested()));
}

TrainingCreationWizard::~TrainingCreationWizard()
{
    delete ui;
}

void TrainingCreationWizard::restart()
{
    ui->pathEdit->setText(Settings::instance().lastPath);
    prevPages.clear();
    ui->stackedWidget->setCurrentWidget(ui->usageSelectionPage);

    bool isNetworkSupported = Settings::instance().isNetworkSupported();
    ui->networkOption->setEnabled(isNetworkSupported);
    ui->networkOptionLabel->setEnabled(isNetworkSupported);
    ui->noNetworkLabel->setVisible(!isNetworkSupported);
    if (!isNetworkSupported)
        ui->homeOption->setChecked(true);

    updateButtons();

    ui->sectionsListWidget->clear();
    for (const auto& section : getSortedSections()) {
        QListWidgetItem* item = new QListWidgetItem(ui->sectionsListWidget);
        item->setText(section.name);
        item->setData(Qt::UserRole, section.id);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    ui->groupsListWidget->clear();
    for (const auto& group : getGroups()) {
        QListWidgetItem* item = new QListWidgetItem(ui->groupsListWidget);
        item->setText(group.name);
        item->setData(Qt::UserRole, group.id);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void TrainingCreationWizard::onGroupAdded(const QUuid& id)
{
    const auto& group = getGroup(id);
    QListWidgetItem* item = new QListWidgetItem(ui->groupsListWidget);
    item->setText(group.name);
    item->setData(Qt::UserRole, group.id);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
}

void TrainingCreationWizard::on_prevButton_clicked()
{
    auto page = prevPages.back();
    prevPages.pop_back();
    ui->stackedWidget->setCurrentWidget(page);
    updateButtons();
}

void TrainingCreationWizard::on_nextButton_clicked()
{
    if (ui->stackedWidget->currentWidget() == ui->usageSelectionPage) {
        if (ui->networkOption->isChecked()) {
            ui->emptyGroupsPathLabel->setVisible(Settings::instance().groupsPath.isEmpty());
            goTo(ui->loginSelectionPage);
        }
        if (ui->homeOption->isChecked()) {
            ui->emptyGroupsPathLabel->setVisible(false);
            goTo(ui->sectionsSelectionPage);
        }
        return;
    }

    if (ui->stackedWidget->currentWidget() == ui->sectionsSelectionPage) {
        int sectionsNum = 0;
        for (int i = 0; i < ui->sectionsListWidget->count(); ++i) {
            QListWidgetItem* item = ui->sectionsListWidget->item(i);
            if (item->checkState() == Qt::Checked)
                sectionsNum++;
        }
        if (sectionsNum == 0) {
            QMessageBox::warning(this, "Неверные данные",
                                 "Не выбран ни один раздел. Отметьте разделы, которые нужно "
                                 "включить в тренажер.");
            return;
        }
        goTo(ui->loginSelectionPage);
        return;
    }

    if (ui->stackedWidget->currentWidget() == ui->loginSelectionPage) {
        if (!ui->groupsBox->isChecked() && !ui->customLoginsBox->isChecked()) {
            QMessageBox::warning(this, "Неверные данные",
                                 "Не выбран ни один вариант системы входа пользователей. "
                                 "Пользователи не смогут зайти в тренажер.");
            return;
        }

        if (ui->groupsBox->isChecked())
            goTo(ui->groupsSelectionPage);
        else
            goTo(ui->saveTypeSelectionPage);
        return;
    }

    if (ui->stackedWidget->currentWidget() == ui->groupsSelectionPage) {
        if (ui->selectGroupsButton->isChecked()) {
            int groupsNum = 0;
            for (int i = 0; i < ui->groupsListWidget->count(); ++i) {
                QListWidgetItem* item = ui->groupsListWidget->item(i);
                if (item->checkState() == Qt::Checked)
                    groupsNum++;
            }
            if (groupsNum == 0) {
                auto answer = QMessageBox::question(
                            this, "Не выбраны группы", "Не выбрана ни одна группа. "
                                                       "Вы уверены, что хотите продолжить?");
                if (answer == QMessageBox::Yes)
                    goTo(ui->saveTypeSelectionPage);
                return;
            }
        }
        goTo(ui->saveTypeSelectionPage);
        return;
    }

    if (ui->stackedWidget->currentWidget() == ui->saveTypeSelectionPage) {
        if (ui->archiveOption->isChecked()) {
            ui->fileSuggestionLabel->show();
            ui->dirSuggestionLabel->hide();
            if (QFileInfo(ui->pathEdit->text()).isDir())
                ui->pathEdit->setText(ui->pathEdit->text() + "/Тренажер.zip");
            goTo(ui->savePathSelectionPage);
        }
        if (ui->folderOption->isChecked()) {
            ui->fileSuggestionLabel->hide();
            ui->dirSuggestionLabel->show();
            goTo(ui->savePathSelectionPage);
        }
        return;
    }

    if (ui->stackedWidget->currentWidget() == ui->savePathSelectionPage) {
        if (ui->archiveOption->isChecked()) {
            QString path = ui->pathEdit->text();
            Settings::instance().updateLastPath(QFileInfo(path).absolutePath());
            QTemporaryDir tempDir;
            if (!tempDir.isValid()) {
                QMessageBox::warning(this, "Ошибка при сохранении",
                                     "Невозможно создать временную папку для формирования тренажера.");
                return;
            }
            if (saveDirectory(tempDir.path())) {
                if (compress(tempDir.path(), path)) {
                    showInExplorer(path);
                    accept();
                    return;
                }
                QMessageBox::warning(this, "Ошибка при сохранении",
                                     "Не удалось создать архив с тренажером.");
                return;
            }
        }

        if (ui->folderOption->isChecked()) {
            QString path = ui->pathEdit->text();
            Settings::instance().updateLastPath(path);
            if (saveDirectory(path)) {
                showInExplorer(path);
                accept();
            }
            return;
        }
    }
}

void TrainingCreationWizard::on_choosePathButton_clicked()
{
    if (ui->archiveOption->isChecked()) {
        QString path = QFileDialog::getSaveFileName(
                    this, "Путь к архиву", ui->pathEdit->text(), "Архив (*.zip)");
        if (!path.isEmpty()) {
            ui->pathEdit->setText(path);
            Settings::instance().updateLastPath(QFileInfo(path).absolutePath());
        }
    }

    if (ui->folderOption->isChecked()) {
        QString path = QFileDialog::getExistingDirectory(
                    this, "Путь к папке", ui->pathEdit->text());
        if (!path.isEmpty()) {
            ui->pathEdit->setText(path);
            Settings::instance().updateLastPath(path);
        }
    }
}

void TrainingCreationWizard::goTo(QWidget* nextPage)
{
    prevPages.append(ui->stackedWidget->currentWidget());
    ui->stackedWidget->setCurrentWidget(nextPage);
    updateButtons();
}

void TrainingCreationWizard::updateButtons()
{
    if (ui->stackedWidget->currentWidget() == ui->savePathSelectionPage) {
        ui->nextButton->setText("Завершить");
    } else {
        ui->nextButton->setText("Далее");
    }

    ui->prevButton->setEnabled(!prevPages.isEmpty());
}

bool TrainingCreationWizard::saveDirectory(QString path)
{
    if (path.isEmpty()) {
        QMessageBox::warning(this, "Неверные данные",
                             "Введите путь к папке с тренажером.");
        return false;
    }
    QFileInfo fileInfo(path);
    if (fileInfo.isDir()) {
        QDir dir(path);
        if (!dir.entryList(QStringList("*"), QDir::AllEntries | QDir::NoDotAndDotDot).empty()) {
            auto answer = QMessageBox::question(
                        this, "Папка не пуста",
                        "Выбранная папка уже существует и содержит другие файлы. "
                        "Вы уверены, что хотите сохранить тренажер в выбранной папке?");
            if (answer != QMessageBox::Yes)
                return false;
        }
    } else {
        if (!QDir().mkpath(path)) {
            QMessageBox::warning(this, "Ошибка при сохранении",
                                 "Невозможно создать папку для тренажера.");
            return false;
        }
    }

    QDir srcDir(TRAINING_SRC_DIR);
    QDir dstDir(path);
    if (!copyDir(srcDir, dstDir)) {
        QMessageBox::warning(this, "Ошибка при сохранении",
                             "Не удалось скопировать программу-тренажер и ее зависимости.");
        return false;
    }

    const auto& settings = Settings::instance();
    TrainingSettings trainingSettings(dstDir.absoluteFilePath("Settings.json"));
    bool saveGroupsToPackage = true;
    if (ui->networkOption->isChecked()) {
        trainingSettings.solutionsPath = settings.solutionsPath;
        trainingSettings.sectionsPath = settings.sectionsPath;
        if (!settings.groupsPath.isEmpty()) {
            saveGroupsToPackage = false;
            trainingSettings.groupsPath = settings.groupsPath;
        }
    } else {
        trainingSettings.solutionsPath = "";
        trainingSettings.sectionsPath = "sections";

        if (!dstDir.mkdir(trainingSettings.sectionsPath)) {
            QMessageBox::warning(this, "Ошибка при сохранении",
                                 "Не удалось создать папку для разделов.");
            return false;
        }

        const auto& sections = getSections();
        auto sectionsDstPath = dstDir.absoluteFilePath(trainingSettings.sectionsPath);
        for (int i = 0; i < ui->sectionsListWidget->count(); ++i) {
            QListWidgetItem* item = ui->sectionsListWidget->item(i);
            if (item->checkState() != Qt::Checked)
                continue;
            const auto& section = sections[item->data(Qt::UserRole).toUuid()];
            QFileInfo sectionSrcFileInfo(section.path);
            QString sectionDstPath = getNewDir(sectionsDstPath, sectionSrcFileInfo.baseName());
            QDir sectionDstDir(sectionDstPath);
            QString sectionDstFileName = sectionDstDir.absoluteFilePath(
                        sectionSrcFileInfo.fileName());
            if (!section.saveAs(sectionDstFileName).isValid()) {
                QMessageBox::warning(this, "Ошибка при сохранении",
                                     "Не удалось записать разделы тренажера.");
                return false;
            }
        }
    }

    if (ui->groupsBox->isChecked()) {
        if (ui->selectGroupsButton->isChecked()) {
            trainingSettings.areAllGroupsAllowed = false;
            for (int i = 0; i < ui->groupsListWidget->count(); ++i) {
                QListWidgetItem* item = ui->groupsListWidget->item(i);
                if (item->checkState() == Qt::Checked)
                    trainingSettings.customGroups.append(item->data(Qt::UserRole).toUuid());
            }
        } else {
            trainingSettings.areAllGroupsAllowed = true;
        }
    } else {
        saveGroupsToPackage = false;
        trainingSettings.groupsPath = "";
        trainingSettings.areAllGroupsAllowed = false;
        trainingSettings.customGroups.clear();
    }

    if (ui->customLoginsBox->isChecked()) {
        if (ui->anyLoginButton->isChecked())
            trainingSettings.loginType = LoginType::Login;
        if (ui->firstNameAndSurnameButton->isChecked())
            trainingSettings.loginType = LoginType::FirstNameAndSurname;
    } else {
        trainingSettings.loginType = LoginType::OnlyFromGroup;
    }

    if (saveGroupsToPackage) {
        if (!dstDir.mkdir("localData")) {
            QMessageBox::warning(this, "Ошибка при сохранении",
                                 "Не удалось записать список групп.");
            return false;
        }
        auto localDataDir = dstDir;
        localDataDir.cd("localData");
        auto groups = getGroups();
        if (ui->selectGroupsButton->isChecked()) {
            auto groupSet = QSet<QUuid>::fromList(trainingSettings.customGroups);
            QList<Group> filteredGroups;
            for (const auto& group : groups) {
                if (groupSet.contains(group.id))
                    filteredGroups.append(group);
            }
            groups.swap(filteredGroups);
        }
        if (!Group::save(groups, localDataDir.absoluteFilePath("Groups.json"))) {
            QMessageBox::warning(this, "Ошибка при сохранении",
                                 "Не удалось записать список групп.");
            return false;
        }
    }

    if (!trainingSettings.write()) {
        QMessageBox::warning(this, "Ошибка при сохранении",
                             "Не удалось записать настройки тренажера.");
        return false;
    }
    return true;
}

void TrainingCreationWizard::on_selectGroupsButton_toggled(bool checked)
{
    ui->groupsListWidget->setEnabled(checked);
}

void TrainingCreationWizard::on_customLoginsBox_toggled(bool checked)
{
    ui->firstNameAndSurnameButton->setEnabled(checked);
    ui->firstNameAndSurnameLabel->setEnabled(checked);
    ui->anyLoginButton->setEnabled(checked);
    ui->anyLoginLabel->setEnabled(checked);
}
