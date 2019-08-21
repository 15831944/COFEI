#ifndef LCSPLIB_GLOBAL_H
#define LCSPLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LCSPLIB_LIBRARY)
#  define LCSPLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LCSPLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LCSPLIB_GLOBAL_H
