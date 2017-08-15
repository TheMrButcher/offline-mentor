#ifndef IMAGEINSERTIONDIALOG_H
#define IMAGEINSERTIONDIALOG_H

#include <omkit/caseimage.h>

#include <QDialog>
#include <QDir>
#include <QPixmap>

namespace Ui {
class ImageInsertionDialog;
}

class ImageInsertionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageInsertionDialog(QWidget *parent = 0);
    ~ImageInsertionDialog();

    void init(QDir sectionDir, const CaseImage& image);
    CaseImage result() const;

    // QDialog interface
public slots:
    virtual void accept() override;

private:
    void resetImage();
    bool setImage(QDir dir, QString fileName, int width, int height);
    void updateSizes();
    void updateRatio();
    void setOptionsEnabled(bool enabled);
    void updatePreview(QPixmap pixmap, int width, int height);

private slots:
    void on_chooseButton_clicked();
    void on_removeButton_clicked();
    void on_widthBox_valueChanged(int width);
    void on_heightBox_valueChanged(int height);
    void on_lockButton_clicked();

private:
    Ui::ImageInsertionDialog *ui;
    QDir sectionDir;
    QPixmap originalPixmap;
    bool isImageCopyNeeded;
    bool ignoreSizeChanges = true;
    float ratio;
};

#endif // IMAGEINSERTIONDIALOG_H
