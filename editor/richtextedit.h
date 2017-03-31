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
    void applyFormat(const QTextCharFormat& format);

    // QTextEdit interface
protected:
    virtual void insertFromMimeData(const QMimeData* source) override;
};

#endif // RICHTEXTEDIT_H
