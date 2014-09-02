#pragma once

#include "../../Shared.h"

#include <QtCore/QSharedPointer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class CORE_EXPORT AutoPlayNextRundownItemEvent
{
    public:
        explicit AutoPlayNextRundownItemEvent(QWidget* widget);

        QWidget* getSource() const;

    private:
        QWidget* widget;
};
