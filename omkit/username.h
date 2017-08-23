#ifndef USERNAME_H
#define USERNAME_H

#include "omkit_global.h"
#include <QString>

class QValidator;

OMKITSHARED_EXPORT QValidator* firstNameValidator();
OMKITSHARED_EXPORT QValidator* surnameValidator();
OMKITSHARED_EXPORT QValidator* userNameValidator();

OMKITSHARED_EXPORT bool isValidFirstName(QString firstName);
OMKITSHARED_EXPORT bool isValidSurname(QString surname);
OMKITSHARED_EXPORT bool isValidUserName(QString userName);

#endif // USERNAME_H
