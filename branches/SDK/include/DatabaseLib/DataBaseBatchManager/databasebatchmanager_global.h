#ifndef DATABASEBATCHMANAGER_GLOBAL_H
#define DATABASEBATCHMANAGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DATABASEBATCHMANAGER_LIBRARY)
#  define DATABASEBATCHMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATABASEBATCHMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DATABASEBATCHMANAGER_GLOBAL_H
