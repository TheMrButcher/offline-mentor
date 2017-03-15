#include "html_utils.h"
#include <QFile>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextDocumentWriter>

QString readHTML(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return QString();

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (!Qt::mightBeRichText(str))
        return QString();
    return str;
}

bool writeHTML(QString fileName, QTextDocument* document)
{
    QTextDocumentWriter writer(fileName);
    return writer.write(document);
}
