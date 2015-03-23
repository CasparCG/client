#pragma once

#include "../../Shared.h"

#include <QtCore/QSharedPointer>

#include <QtWidgets/QWidget>

class CORE_EXPORT AutoPlayNextRundownItemEvent
{
    public:
        explicit AutoPlayNextRundownItemEvent(QWidget* widget);

        QWidget* getSource() const;

    private:
        QWidget* widget;
};
