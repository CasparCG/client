#pragma once

#include <QtCore/QtGlobal>

#if defined(SPYDER_LIBRARY)
    #define SPYDER_EXPORT Q_DECL_EXPORT
#else
    #define SPYDER_EXPORT Q_DECL_IMPORT
#endif
