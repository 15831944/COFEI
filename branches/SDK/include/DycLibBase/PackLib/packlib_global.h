#ifndef PACKLIB_GLOBAL_H
#define PACKLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PACKLIB_LIBRARY)
#  define PACKLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PACKLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PACKLIB_GLOBAL_H
