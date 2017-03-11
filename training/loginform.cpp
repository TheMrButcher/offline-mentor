#include "loginform.h"
#include "ui_loginform.h"
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

void LoginForm::on_enterButton_clicked()
{
    QString name = ui->nameEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Пустое имя пользователя", "Пожалуйста, введите имя пользователя");
        return;
    }
}
