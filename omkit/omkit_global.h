#ifndef OMKIT_GLOBAL_H
#define OMKIT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OMKIT_LIBRARY)
#  define OMKITSHARED_EXPORT Q_DECL_EXPORT
#else
#  define OMKITSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // OMKIT_GLOBAL_H
