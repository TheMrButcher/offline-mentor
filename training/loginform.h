#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>

namespace Ui {
class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

    void setUserName(QString name);

private slots:
    void on_enterButton_clicked();

private:
    Ui::LoginForm *ui;
};

#endif // LOGINFORM_H
