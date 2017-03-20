#ifndef BASEUI_GLOBAL_H
#define BASEUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(USE_STATIC_LIB)
#define BASEUISHARED_EXPORT Q_DECL_EXPORT
#else
#if defined(BASEUI_LIBRARY)
#  define BASEUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define BASEUISHARED_EXPORT Q_DECL_IMPORT
#endif
#endif

#endif // BASEUI_GLOBAL_H
