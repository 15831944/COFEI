#ifndef TEACHDEVICERUNLIB_GLOBAL_H
#define TEACHDEVICERUNLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TEACHDEVICERUNLIB_LIBRARY)
#  define TEACHDEVICERUNLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TEACHDEVICERUNLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TEACHDEVICERUNLIB_GLOBAL_H
