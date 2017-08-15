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
    : horAlign(HorAlign::Left)
    , vertAlign(VertAlign::Top)
    , width(0)
    , height(0)
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
    image.width = jsonObject["width"].toInt(0);
    image.height = jsonObject["height"].toInt(0);
    return image;
}

QJsonObject CaseImage::toJson() const
{
    QJsonObject result;
    result["fileName"] = fileName;
    result["horAlign"] = HOR_ALIGN_NAMES.valueByFirst(horAlign);
    result["vertAlign"] = VERT_ALIGN_NAMES.valueByFirst(vertAlign);
    result["width"] = width;
    result["height"] = height;
    return result;
}

bool operator==(const CaseImage& image1, const CaseImage& image2)
{
    return image1.fileName == image2.fileName
            && image1.horAlign == image2.horAlign
            && image1.vertAlign == image2.vertAlign
            && image1.width == image2.width
            && image1.height == image2.height;
}


