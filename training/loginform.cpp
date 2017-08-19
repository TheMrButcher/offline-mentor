#include "loginform.h"
#include "ui_loginform.h"
#include "settings.h"
#include "group_utils.h"
#include "user_utils.h"

#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    QRegExp validFirstNameOrSurname("[-а-яА-Я ]*");
    ui->firstNameEdit->setValidator(new QRegExpValidator(validFirstNameOrSurname));
    ui->surnameEdit->setValidator(new QRegExpValidator(validFirstNameOrSurname));

    QRegExp validUserName("[-а-яА-Я a-zA-Z_0-9]*");
    ui->userNameEdit->setValidator(new QRegExpValidator(validUserName));

    ui->customLoginWidget->hide();
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::init()
{
    const auto& settings = Settings::instance();
    if (settings.loginType == LoginType::Login) {
        ui->stackedWidget->setCurrentWidget(ui->userNamePage);
        ui->userNameEdit->setText(settings.lastLogin);
    } else {
        ui->stackedWidget->setCurrentWidget(ui->firstNameAndSurnamePage);
        ui->firstNameEdit->setText(settings.lastFirstName);
        ui->surnameEdit->setText(settings.lastSurname);
    }

    if (settings.loginType != LoginType::OnlyFromGroup) {
        hasCustomLoginMethod = true;
        ui->groupNameComboBox->addItem("* Вне списка *");
    }

    const auto& groups = getGroups();
    if (groups.isEmpty()) {
        ui->groupsWidget->hide();
        ui->noFirstNameAndSurnameInListWidget->hide();
        ui->noUserNameInListWidget->hide();
        ui->customLoginWidget->show();
    } else {
        for (const auto& group : groups)
            ui->groupNameComboBox->addItem(group.name, group.id);

        ui->customLoginWidget->hide();
        if (settings.loginType == LoginType::OnlyFromGroup)
            ui->notInListWidget->hide();
    }
    validate();
}

QString LoginForm::userName() const
{
    return Settings::instance().lastLogin;
}

void LoginForm::validate()
{
    if (ui->customLoginWidget->isVisible()) {
        if (ui->stackedWidget->currentWidget() == ui->firstNameAndSurnamePage) {
            ui->enterButton->setEnabled(!ui->firstNameEdit->text().isEmpty()
                                        && !ui->surnameEdit->text().isEmpty());
            return;
        }
        if (ui->stackedWidget->currentWidget() == ui->userNamePage) {
            ui->enterButton->setEnabled(!ui->userNameEdit->text().isEmpty());
            return;
        }
        ui->enterButton->setEnabled(false);
        return;
    }

    if (ui->groupsWidget->isVisible()) {
        ui->enterButton->setEnabled(ui->userNameComboBox->currentIndex() > 0);
        return;
    }
    ui->enterButton->setEnabled(false);
    return;
}

void LoginForm::on_enterButton_clicked()
{
    auto& settings = Settings::instance();
    if (ui->customLoginWidget->isVisible()) {
        if (ui->stackedWidget->currentWidget() == ui->firstNameAndSurnamePage) {
            QString firstName = ui->firstNameEdit->text().trimmed();
            QString surname = ui->surnameEdit->text().trimmed();
            if (firstName.isEmpty()) {
                QMessageBox::warning(this, "Пустое имя",
                                     "Пожалуйста, введите имя.");
                return;
            }
            if (surname.isEmpty()) {
                QMessageBox::warning(this, "Пустая фамилия",
                                     "Пожалуйста, введите фамилию.");
                return;
            }
            QString name = surname + " " + firstName;
            if (name.length() > 201) {
                QMessageBox::warning(this, "Слишком длинное имя пользователя",
                                     "Пожалуйста, введите корректные фамилию и имя длиной "
                                     "суммарно не больше 200 символов.");
                return;
            }
            settings.setLastLogin(firstName, surname, name);
            emit login();
            return;
        }
        if (ui->stackedWidget->currentWidget() == ui->userNamePage) {
            QString name = ui->userNameEdit->text().trimmed();
            if (name.isEmpty()) {
                QMessageBox::warning(this, "Пустое имя пользователя",
                                     "Пожалуйста, введите логин.");
                return;
            }
            if (name.length() > 200) {
                QMessageBox::warning(this, "Слишком длинное имя пользователя",
                                     "Пожалуйста, введите корректный логин длиной "
                                     "не больше 200 символов.");
                return;
            }
            settings.setLastLogin(name);
            emit login();
            return;
        }
        QMessageBox::critical(this, "Некорректное состояние",
                              "Пожалуйста, перезапустите приложение.");
        return;
    }

    if (ui->groupsWidget->isVisible()) {
        if (ui->userNameComboBox->currentIndex() == 0) {
            QMessageBox::warning(this, "Имя пользователя не выбрано",
                                 "Пожалуйста, выберите логин из списка.");
            return;
        }
        settings.setLastLogin(ui->userNameComboBox->currentText());
        emit login();
        return;
    }
    QMessageBox::critical(this, "Некорректное состояние",
                          "Пожалуйста, перезапустите приложение.");
}

void LoginForm::on_notInListButton_clicked()
{
    ui->groupNameComboBox->setCurrentIndex(1);
}

void LoginForm::on_groupNameComboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->customLoginWidget->hide();
        ui->userNameComboBox->setCurrentIndex(0);
        ui->userNameComboBox->setEnabled(false);
        validate();
        return;
    }

    if (index == 1 && hasCustomLoginMethod) {
        ui->customLoginWidget->show();
        ui->userNameComboBox->setCurrentIndex(0);
        ui->userNameComboBox->setEnabled(false);
        validate();
        return;
    }

    ui->customLoginWidget->hide();
    ui->userNameComboBox->setCurrentIndex(0);
    ui->userNameComboBox->setEnabled(true);
    for (int i = ui->userNameComboBox->count() - 1; i > 0; --i)
        ui->userNameComboBox->removeItem(i);

    const auto& group = getGroup(ui->groupNameComboBox->itemData(index).toUuid());
    for (const auto& userName : group.sortedUserNames)
        ui->userNameComboBox->addItem(userName);
    validate();
}

void LoginForm::on_userNameComboBox_currentIndexChanged(int)
{
    validate();
}

void LoginForm::on_surnameEdit_textChanged(const QString &)
{
    validate();
}

void LoginForm::on_firstNameEdit_textChanged(const QString &)
{
    validate();
}

void LoginForm::on_userNameEdit_textChanged(const QString &)
{
    validate();
}
