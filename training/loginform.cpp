#include "loginform.h"
#include "ui_loginform.h"
#include "settings.h"

#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    QRegExp validName("[-а-яА-Я ]*");
    ui->nameEdit->setValidator(new QRegExpValidator(validName));
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
    QString name = ui->nameEdit->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Пустое имя пользователя",
                             "Пожалуйста, введите имя пользователя.");
        return;
    }
    if (name.length() > 200) {
        QMessageBox::warning(this, "Слишком длинное имя пользователя",
                             "Пожалуйста, введите корректное имя пользователя длиной "
                             "не больше 200 символов.");
        return;
    }

    auto& settings = Settings::instance();
    settings.lastLogin = name;
    settings.write();

    emit login();
}
