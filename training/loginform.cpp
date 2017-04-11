#include "loginform.h"
#include "ui_loginform.h"
#include "settings.h"

#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

QString LoginForm::userName() const
{
    return ui->nameEdit->text();
}

void LoginForm::setUserName(QString name)
{
    ui->nameEdit->setText(name);
}

void LoginForm::on_enterButton_clicked()
{
    QString name = ui->nameEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Пустое имя пользователя",
                             "Пожалуйста, введите имя пользователя");
        return;
    }

    auto& settings = Settings::instance();
    settings.lastLogin = name;
    settings.write();

    emit login();
}
