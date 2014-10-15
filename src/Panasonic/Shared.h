#pragma once

#include <QtCore/QtGlobal>

#if defined(PANASONIC_LIBRARY)
    #define PANASONIC_EXPORT Q_DECL_EXPORT
#else
    #define PANASONIC_EXPORT Q_DECL_IMPORT
#endif
