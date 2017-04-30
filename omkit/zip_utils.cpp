#include "zip_utils.h"
#include <JlCompress.h>

bool compress(QString srcDirPath, QString dstPath)
{
    return JlCompress::compressDir(dstPath, srcDirPath, true);
}

bool extract(QString srcPath, QString dstDirPath)
{
    return !JlCompress::extractDir(srcPath, dstDirPath).empty();
}
