#pragma once

#include <QtCore/QtGlobal>

#if defined(WEB_LIBRARY)
    #define WEB_EXPORT Q_DECL_EXPORT
#else
    #define WEB_EXPORT Q_DECL_IMPORT
#endif
