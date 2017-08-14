#ifndef CASEIMAGE_H
#define CASEIMAGE_H

#include "omkit_global.h"

#include <QString>
#include <QJsonObject>

class OMKITSHARED_EXPORT CaseImage
{
public:
    CaseImage();

    bool isEmpty() const { return fileName.isEmpty(); }

    static CaseImage fromJson(const QJsonObject& jsonObject);
    QJsonObject toJson() const;

    enum class HorAlign {
        Left,
        Center,
        Right
    };

    enum class VertAlign {
        Top,
        Bottom
    };

    QString fileName;
    HorAlign horAlign;
    VertAlign vertAlign;
};

#endif // CASEIMAGE_H
