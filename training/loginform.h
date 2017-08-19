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

    void init();
    QString userName() const;

signals:
    void login();

private slots:
    void validate();

    void on_enterButton_clicked();
    void on_notInListButton_clicked();
    void on_groupNameComboBox_currentIndexChanged(int index);
    void on_userNameComboBox_currentIndexChanged(int);
    void on_surnameEdit_textChanged(const QString &);
    void on_firstNameEdit_textChanged(const QString &);
    void on_userNameEdit_textChanged(const QString &);

private:
    Ui::LoginForm *ui;

    bool hasCustomLoginMethod = false;
};

#endif // LOGINFORM_H
