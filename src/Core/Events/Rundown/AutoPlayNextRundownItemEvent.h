#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

#include <QtGui/QWidget>

class CORE_EXPORT AutoPlayNextRundownItemEvent : public QEvent
{
    public:
        explicit AutoPlayNextRundownItemEvent(QWidget* widget);

        QWidget* getSource() const;

    private:
        QWidget* widget;
};
