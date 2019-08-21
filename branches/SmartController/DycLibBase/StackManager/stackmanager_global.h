#ifndef STACKMANAGER_GLOBAL_H
#define STACKMANAGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(STACKMANAGER_LIBRARY)
#  define STACKMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define STACKMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // STACKMANAGER_GLOBAL_H
