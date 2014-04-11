#pragma once

#include <QtCore/QtGlobal>

#if defined(ATEM_LIBRARY)
    #define ATEM_EXPORT Q_DECL_EXPORT
#else
    #define ATEM_EXPORT Q_DECL_IMPORT
#endif
