#ifndef TEXTEDITORPAGE_H
#define TEXTEDITORPAGE_H

#include <QWidget>
#include <QDir>

namespace Ui {
class TextEditorPage;
}

class RichTextEdit;

class TextEditorPage : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditorPage(QWidget *parent = 0);
    ~TextEditorPage();

    void setTitle(QString title);
    void setFilePath(QDir dir, QString fileName);
    QString fileName() const;
    bool save();
    bool load();
    bool removeFile();
    RichTextEdit* textEdit();

private:
    Ui::TextEditorPage *ui;
    RichTextEdit* myTextEdit;
    QDir dir;
    QString myFileName;
};

#endif // TEXTEDITORPAGE_H
