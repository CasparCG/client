#pragma once

#include <QtCore/QtGlobal>

#if defined(SCHEDULE_LIBRARY)
    #define SCHEDULE_EXPORT Q_DECL_EXPORT
#else
    #define SCHEDULE_EXPORT Q_DECL_IMPORT
#endif
