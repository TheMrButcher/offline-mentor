#ifndef TEXTEDITORPAGE_H
#define TEXTEDITORPAGE_H

#include <QWidget>
#include <QDir>

namespace Ui {
class TextEditorPage;
}

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

private:
    Ui::TextEditorPage *ui;
    QDir dir;
    QString myFileName;
};

#endif // TEXTEDITORPAGE_H
