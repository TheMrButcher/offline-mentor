#ifndef TEXTEDITORPAGE_H
#define TEXTEDITORPAGE_H

#include <QWidget>

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

private:
    Ui::TextEditorPage *ui;
};

#endif // TEXTEDITORPAGE_H
