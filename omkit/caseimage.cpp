#include "caseimage.h"
#include "smallbimap.h"

namespace {
const SmallBimap<CaseImage::HorAlign, QString> HOR_ALIGN_NAMES(
        { CaseImage::HorAlign::Left, CaseImage::HorAlign::Center, CaseImage::HorAlign::Right },
        { "Left", "Center", "Right" });
const SmallBimap<CaseImage::VertAlign, QString> VERT_ALIGN_NAMES(
        { CaseImage::VertAlign::Top, CaseImage::VertAlign::Bottom },
        { "Top", "Bottom" });
}

CaseImage::CaseImage()
{
}

CaseImage CaseImage::fromJson(const QJsonObject& jsonObject)
{
    CaseImage image;
    image.fileName = jsonObject["fileName"].toString("");
    image.horAlign = HOR_ALIGN_NAMES.valueBySecondOr(
                jsonObject["horAlign"].toString(), CaseImage::HorAlign::Left);
    image.vertAlign = VERT_ALIGN_NAMES.valueBySecondOr(
                jsonObject["vertAlign"].toString(), CaseImage::VertAlign::Top);
    return image;
}

QJsonObject CaseImage::toJson() const
{
    QJsonObject result;
    result["fileName"] = fileName;
    result["horAlign"] = HOR_ALIGN_NAMES.valueByFirst(horAlign);
    result["vertAlign"] = VERT_ALIGN_NAMES.valueByFirst(vertAlign);
    return result;
}


