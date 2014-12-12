#pragma once

#include "../../Shared.h"

#include <QtCore/QSharedPointer>

#include <QtWidgets/QWidget>

class CORE_EXPORT AutoPlayRundownItemEvent
{
    public:
        explicit AutoPlayRundownItemEvent(QWidget* widget);

        QWidget* getSource() const;

    private:
        QWidget* widget;
};
