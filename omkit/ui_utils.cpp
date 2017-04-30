#include "ui_utils.h"
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QProcessEnvironment>

bool createDirDialog(QWidget* parent, QString path, QString uiDirName)
{
    int answer = QMessageBox::question(parent, "Отсутствует папка",
                                       "Папка " + uiDirName + " не существует. Создать?");
    if (answer != QMessageBox::Yes)
        return false;
    QDir dir;
    if (!dir.mkpath(path)) {
        QMessageBox::warning(parent, "Ошибка при создании",
                             "Не удалось создать папку " + uiDirName + ". "
                             "Проверьте правильность написания пути.");
        return false;
    }
    return true;
}

bool showInExplorer(QString path)
{
    QString param;
    if (!QFileInfo(path).isDir())
        param = QLatin1String("/select,");
    param += QDir::toNativeSeparators(path);
    QString command = "explorer.exe " + param;
    return QProcess::startDetached(command);
}
