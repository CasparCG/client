#pragma once

#include <QtCore/QtGlobal>

#if defined(SONY_LIBRARY)
    #define SONY_EXPORT Q_DECL_EXPORT
#else
    #define SONY_EXPORT Q_DECL_IMPORT
#endif
