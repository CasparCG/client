#pragma once

#include <QtCore/QtGlobal>

#if defined(WIDGETS_LIBRARY)
    #define WIDGETS_EXPORT Q_DECL_EXPORT
#else
    #define WIDGETS_EXPORT Q_DECL_IMPORT
#endif
