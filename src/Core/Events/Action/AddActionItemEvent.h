#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT AddActionItemEvent : public QEvent
{
    public:
        explicit AddActionItemEvent(const QString& action);

        const QString& getAction() const;

    private:
        QString action;
};
