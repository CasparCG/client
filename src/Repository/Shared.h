#pragma once

#include <QtCore/QtGlobal>

#if defined(REPOSITORY_LIBRARY)
    #define REPOSITORY_EXPORT Q_DECL_EXPORT
#else
    #define REPOSITORY_EXPORT Q_DECL_IMPORT
#endif
