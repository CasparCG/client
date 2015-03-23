#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT AddActionItemEvent
{
    public:
        explicit AddActionItemEvent(const QString& action);

        const QString& getAction() const;

    private:
        QString action;
};
