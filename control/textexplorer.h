#ifndef TEXTEXPLORER_H
#define TEXTEXPLORER_H

#include <QWidget>
#include <QDir>

class CaseImage;

namespace Ui {
class TextExplorer;
}

class TextExplorer : public QWidget
{
    Q_OBJECT

public:
    explicit TextExplorer(QWidget *parent = 0);
    ~TextExplorer();

    void setTitle(QString title);
    void setPlainText(QString text);
    bool load(QString path);
    bool load(QDir dir, QString fileName, const CaseImage& image);

private:
    Ui::TextExplorer *ui;
};

#endif // TEXTEXPLORER_H
