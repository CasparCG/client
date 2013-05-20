#pragma once

#include <QtCore/QtGlobal>

#if defined(OSC_LIBRARY)
    #define OSC_EXPORT Q_DECL_EXPORT
#else
    #define OSC_EXPORT Q_DECL_IMPORT
#endif
