#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT GpiModel
{
    public:
        explicit GpiModel(int id, const QString& trigger, const QString& action);

        int getId() const;
        const QString& getTrigger() const;
        const QString& getAction() const;

    private:
        int id;
        QString trigger;
        QString action;
};


