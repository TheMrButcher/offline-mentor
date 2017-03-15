#ifndef OMKIT_H
#define OMKIT_H

#include "omkit_global.h"

class OMKITSHARED_EXPORT OMKit
{
public:
    static OMKit& instance();

    QString getVersion();

private:
    OMKit();
};

#endif // OMKIT_H
