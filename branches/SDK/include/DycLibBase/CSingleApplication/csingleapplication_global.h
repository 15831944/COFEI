#ifndef CSINGLEAPPLICATION_GLOBAL_H
#define CSINGLEAPPLICATION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CSINGLEAPPLICATION_LIBRARY)
#  define CSINGLEAPPLICATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CSINGLEAPPLICATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CSINGLEAPPLICATION_GLOBAL_H
