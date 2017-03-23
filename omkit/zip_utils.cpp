#include "zip_utils.h"
#include <JlCompress.h>

bool compress(QString srcDirPath, QString dstPath)
{
    return JlCompress::compressDir(dstPath, srcDirPath, true);
}
