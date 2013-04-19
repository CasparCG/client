#pragma once

#include <QtCore/QtGlobal>

#if defined(CASPAR_LIBRARY)
    #define CASPAR_EXPORT Q_DECL_EXPORT
#else
    #define CASPAR_EXPORT Q_DECL_IMPORT
#endif
