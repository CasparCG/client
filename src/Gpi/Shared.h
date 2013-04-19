#pragma once

#include <QtCore/QtGlobal>

#if defined(GPI_LIBRARY)
    #define GPI_EXPORT Q_DECL_EXPORT
#else
    #define GPI_EXPORT Q_DECL_IMPORT
#endif
