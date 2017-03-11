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

    QString userName() const;
    void setUserName(QString name);

signals:
    void login();

private slots:
    void on_enterButton_clicked();

private:
    Ui::LoginForm *ui;
};

#endif // LOGINFORM_H
