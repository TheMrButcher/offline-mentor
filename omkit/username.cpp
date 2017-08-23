#include "username.h"
#include <QRegExp>
#include <QRegExpValidator>

namespace {
const QRegExp VALID_FIRST_NAME_OR_SURNAME("[-а-яА-Я0-9 ]*");
const QRegExp VALID_USER_NAME("[-а-яА-Я a-zA-Z_0-9]*");
} // namespace

QValidator* firstNameValidator()
{
    return new QRegExpValidator(VALID_FIRST_NAME_OR_SURNAME);
}

QValidator* surnameValidator()
{
    return new QRegExpValidator(VALID_FIRST_NAME_OR_SURNAME);
}

QValidator* userNameValidator()
{
    return new QRegExpValidator(VALID_USER_NAME);
}

bool isValidFirstName(QString firstName)
{
    return VALID_FIRST_NAME_OR_SURNAME.exactMatch(firstName);
}

bool isValidSurname(QString surname)
{
    return VALID_FIRST_NAME_OR_SURNAME.exactMatch(surname);
}

bool isValidUserName(QString userName)
{
    return VALID_FIRST_NAME_OR_SURNAME.exactMatch(userName);
}
