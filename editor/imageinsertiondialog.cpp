#include "imageinsertiondialog.h"
#include "ui_imageinsertiondialog.h"
#include "settings.h"
#include <omkit/utils.h>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>

ImageInsertionDialog::ImageInsertionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageInsertionDialog)
{
    ui->setupUi(this);
}

ImageInsertionDialog::~ImageInsertionDialog()
{
    delete ui;
}

void ImageInsertionDialog::init(QDir sectionDir, const CaseImage& image)
{
    this->sectionDir = sectionDir;
    isImageCopyNeeded = false;
    if (image.isEmpty()) {
        resetImage();
        return;
    }
    if (!setImage(sectionDir, image.fileName, image.width, image.height)) {
        resetImage();
        return;
    }
    updateSizes();
    updateRatio();

    switch (image.horAlign) {
    case CaseImage::HorAlign::Left: ui->alignLeftButton->setChecked(true); break;
    case CaseImage::HorAlign::Center: ui->alignCenterButton->setChecked(true); break;
    case CaseImage::HorAlign::Right: ui->alignRightButton->setChecked(true); break;
    }
    switch (image.vertAlign) {
    case CaseImage::VertAlign::Top: ui->alignTopButton->setChecked(true); break;
    case CaseImage::VertAlign::Bottom: ui->alignBottomButton->setChecked(true); break;
    }
    ui->lockButton->setChecked(true);
}

CaseImage ImageInsertionDialog::result() const
{
    CaseImage image;
    image.fileName = ui->nameEdit->text();
    if (ui->alignLeftButton->isChecked())
        image.horAlign = CaseImage::HorAlign::Left;
    if (ui->alignCenterButton->isChecked())
        image.horAlign = CaseImage::HorAlign::Center;
    if (ui->alignRightButton->isChecked())
        image.horAlign = CaseImage::HorAlign::Right;
    if (ui->alignTopButton->isChecked())
        image.vertAlign = CaseImage::VertAlign::Top;
    if (ui->alignBottomButton->isChecked())
        image.vertAlign = CaseImage::VertAlign::Bottom;
    image.width = ui->widthBox->value();
    image.height = ui->heightBox->value();
    return image;
}

void ImageInsertionDialog::accept()
{
    if (isImageCopyNeeded) {
        auto newPath = getNewFileName(sectionDir.absolutePath(), ui->nameEdit->text());
        if (newPath.isEmpty()) {
            QMessageBox::warning(this, "Ошибка при сохранении файла",
                                 "Невозможно сохранить изображение в раздел");
            return;
        }
        if (!QFile::copy(ui->nameEdit->text(), newPath)) {
            QMessageBox::warning(this, "Ошибка при сохранении файла",
                                 "Невозможно сохранить изображение в раздел");
            return;
        }
        ui->nameEdit->setText(QFileInfo(newPath).fileName());
    }
    QDialog::accept();
}

void ImageInsertionDialog::resetImage()
{
    ui->nameEdit->setText("");
    setOptionsEnabled(false);
    ui->imagePreview->setText("Нет изображения");
    ui->imagePreview->setPixmap(QPixmap());
}

bool ImageInsertionDialog::setImage(QDir dir, QString fileName, int width, int height)
{
    QPixmap pixmap(dir.absoluteFilePath(fileName));
    if (pixmap.isNull())
        return false;
    originalPixmap = pixmap;
    setOptionsEnabled(true);
    ui->imagePreview->setText("");
    updatePreview(pixmap, width, height);
    ui->nameEdit->setText(fileName);
    return true;
}

void ImageInsertionDialog::updateSizes()
{
    const QPixmap* pixmap = ui->imagePreview->pixmap();
    if (!pixmap || pixmap->isNull())
        return;
    ignoreSizeChanges = true;
    ui->widthBox->setValue(pixmap->width());
    ui->heightBox->setValue(pixmap->height());
    ignoreSizeChanges = false;
}

void ImageInsertionDialog::updateRatio()
{
    const QPixmap* pixmap = ui->imagePreview->pixmap();
    if (!pixmap || pixmap->isNull()) {
        resetImage();
        return;
    }
    ratio = static_cast<float>(pixmap->width()) / pixmap->height();
}

void ImageInsertionDialog::setOptionsEnabled(bool enabled)
{
    ui->removeButton->setEnabled(enabled);
    ui->sizeBox->setEnabled(enabled);
    ui->horizontalAlignBox->setEnabled(enabled);
    ui->verticalAlignBox->setEnabled(enabled);
}

void ImageInsertionDialog::updatePreview(QPixmap pixmap, int width, int height)
{
    if (width != 0 && height != 0) {
        if (pixmap.width() != width || pixmap.height() != height)
            pixmap = pixmap.scaled(width, height,
                                   Qt::IgnoreAspectRatio,
                                   Qt::SmoothTransformation);
    }
    ui->imagePreview->setPixmap(pixmap);
}

void ImageInsertionDialog::on_chooseButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this, "Путь к изображению",
                Settings::instance().lastDirectoryPath,
                "Изображение (*.png *.jpg *.bmp)");
    if (!path.isEmpty()) {
        Settings::instance().updateLastDirectoryPath(path);
        ui->nameEdit->setText(path);
        if (!setImage(QDir(), path, 0, 0)) {
            QMessageBox::warning(this, "Ошибка при открытии файла",
                                 "Невозможно прочесть файл с изображением");
            return;
        }
        isImageCopyNeeded = true;
        updateSizes();
        updateRatio();
        ui->lockButton->setChecked(true);
    }
}

void ImageInsertionDialog::on_removeButton_clicked()
{
    resetImage();
}

void ImageInsertionDialog::on_widthBox_valueChanged(int width)
{
    if (ignoreSizeChanges)
        return;
    bool updatedImage = false;
    if (ui->lockButton->isChecked()) {
        int height = static_cast<int>(width / ratio + 0.5f);
        if (ui->heightBox->value() != height) {
            ui->heightBox->setValue(height);
            updatedImage = true;
        }
    }
    if (!updatedImage)
        updatePreview(originalPixmap, ui->widthBox->value(), ui->heightBox->value());
}

void ImageInsertionDialog::on_heightBox_valueChanged(int height)
{
    if (ignoreSizeChanges)
        return;
    bool updatedImage = false;
    if (ui->lockButton->isChecked()) {
        int width = static_cast<int>(height * ratio + 0.5f);
        if (ui->widthBox->value() != width) {
            ui->widthBox->setValue(width);
            updatedImage = true;
        }
    }
    if (!updatedImage)
        updatePreview(originalPixmap, ui->widthBox->value(), ui->heightBox->value());
}

void ImageInsertionDialog::on_lockButton_clicked()
{
    if (ui->lockButton->isChecked())
        updateRatio();
}
