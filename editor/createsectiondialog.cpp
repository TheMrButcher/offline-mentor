#include "createsectiondialog.h"
#include "settings.h"
#include "ui_createsectiondialog.h"

#include <omkit/section.h>

#include <QMessageBox>
#include <QFileDialog>

CreateSectionDialog::CreateSectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateSectionDialog)
{
    ui->setupUi(this);
}

CreateSectionDialog::~CreateSectionDialog()
{
    delete ui;
}

Section CreateSectionDialog::result() const
{
    return section;
}

void CreateSectionDialog::accept()
{
    auto path = ui->pathEdit->text();
    if (!path.endsWith(".oms", Qt::CaseInsensitive))
        path.append(".oms");

    section = Section::createSection(path);
    section.name = ui->nameEdit->text();

    if (section.name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Имя раздела не указано.");
        return;
    }

    if (!section.save()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось создать раздел. "
                                             "Проверьте правильность написания пути.");
        return;
    }
    QDialog::accept();
}

void CreateSectionDialog::on_choosePathButton_clicked()
{
    QString prevPath = ui->pathEdit->text();
    QString startPath;
    if (!prevPath.isEmpty()) {
        QFileInfo fileInfo(prevPath);
        if (fileInfo.isDir()) {
            startPath = prevPath;
        } else {
            QString prevParentPath = fileInfo.absolutePath();
            if (QFileInfo(prevParentPath).isDir())
                startPath = prevParentPath;
        }
    }
    if (startPath.isEmpty())
        startPath = Settings::instance().safeLastDirectoryPath();

    QString path = QFileDialog::getSaveFileName(
                this, "Путь к файлу раздела",
                startPath,
                "Файл раздела (*.oms)");
    if (!path.isEmpty()) {
        ui->pathEdit->setText(path);
    }
}
