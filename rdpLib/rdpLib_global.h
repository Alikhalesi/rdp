#ifndef RDPLIB_GLOBAL_H
#define RDPLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RDPLIB_LIBRARY)
#  define RDPLIB_EXPORT Q_DECL_EXPORT
#else
#  define RDPLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // RDPLIB_GLOBAL_H
