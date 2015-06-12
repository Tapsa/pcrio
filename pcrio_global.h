#ifndef PCRIO_GLOBAL_H
#define PCRIO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PCRIO_LIBRARY)
#  define PCRIOSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PCRIOSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PCRIO_GLOBAL_H
