#ifndef RICHTEXTEDIT_H
#define RICHTEXTEDIT_H

#include <QTextEdit>

class RichTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit RichTextEdit(QWidget *parent = Q_NULLPTR);

public slots:
    void clearFormat();

    // QTextEdit interface
protected:
    virtual void insertFromMimeData(const QMimeData* source) override;

private:
    void applyFormat(const QTextCharFormat& format);
};

#endif // RICHTEXTEDIT_H
