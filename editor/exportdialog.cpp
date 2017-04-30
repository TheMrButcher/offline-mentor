#include "exportdialog.h"
#include "settings.h"
#include "section_utils.h"
#include "ui_exportdialog.h"
#include <omkit/utils.h>
#include <omkit/zip_utils.h>
#include <omkit/ui_utils.h>
#include <QFileDialog>
#include <QTemporaryDir>
#include <QMessageBox>

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::initUI()
{
    ui->pathEdit->setText(Settings::instance().safeLastDirectoryPath() + "/Разделы.zip");
    ui->allCheckBox->setChecked(false);

    auto sortedSections = getSections();
    qSort(sortedSections.begin(), sortedSections.end(),
          [](const Section& s1, const Section& s2) { return s1.name < s2.name; });
    ui->listWidget->clear();
    foreach (const auto& section, sortedSections) {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setText(section.name);
        item->setData(Qt::UserRole, section.id);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

void ExportDialog::accept()
{
    int sectionsNum = 0;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        if (item->checkState() == Qt::Checked)
            sectionsNum++;
    }
    if (sectionsNum == 0) {
        QMessageBox::warning(this, "Неверные данные",
                             "Не выбран ни один раздел. Отметьте разделы, которые нужно "
                             "экспортировать в архив.");
        return;
    }

    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        QMessageBox::warning(this, "Ошибка при сохранении",
                             "Невозможно создать временную папку для разделов.");
        return;
    }

    QHash<QUuid, Section> sectionMap;
    foreach (const auto& section, getSections())
        sectionMap[section.id] = section;

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        if (item->checkState() != Qt::Checked)
            continue;
        const auto& section = sectionMap[item->data(Qt::UserRole).toUuid()];
        QFileInfo sectionSrcFileInfo(section.path);
        QString sectionDstPath = getNewDir(tempDir.path(), sectionSrcFileInfo.baseName());
        QDir sectionDstDir(sectionDstPath);
        QString sectionDstFileName = sectionDstDir.absoluteFilePath(
                    sectionSrcFileInfo.fileName());
        if (!section.saveAs(sectionDstFileName).isValid()) {
            QMessageBox::warning(this, "Ошибка при сохранении",
                                 "Не удалось записать один из разделов.");
            return;
        }
    }

    QString path = ui->pathEdit->text();
    if (!compress(tempDir.path(), path)) {
        QMessageBox::warning(this, "Ошибка при сохранении",
                             "Не удалось создать архив с разделами. Проверьте правильность"
                             " написания пути.");
        return;
    }
    showInExplorer(path);
    Settings::instance().updateLastDirectoryPath(path);
    QDialog::accept();
}

void ExportDialog::on_choosePathButton_clicked()
{
    QString path = QFileDialog::getSaveFileName(
                this, "Путь к архиву", ui->pathEdit->text(), "Архив (*.zip)");
    if (!path.isEmpty()) {
        ui->pathEdit->setText(path);
        Settings::instance().updateLastDirectoryPath(path);
    }
}

void ExportDialog::on_allCheckBox_clicked()
{
    Qt::CheckState checkState = ui->allCheckBox->isChecked()
            ? Qt::Checked : Qt::Unchecked;
    for (int i = 0; i < ui->listWidget->count(); ++i)
        ui->listWidget->item(i)->setCheckState(checkState);
}
