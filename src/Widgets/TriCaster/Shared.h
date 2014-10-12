#pragma once

#include <QtCore/QtGlobal>

#if defined(TRICASTER_LIBRARY)
    #define TRICASTER_EXPORT Q_DECL_EXPORT
#else
    #define TRICASTER_EXPORT Q_DECL_IMPORT
#endif
