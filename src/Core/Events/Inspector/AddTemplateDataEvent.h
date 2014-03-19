#pragma once

#include "../../Shared.h"

#include <QtCore/QSharedPointer>

class CORE_EXPORT AddTemplateDataEvent
{
    public:
        explicit AddTemplateDataEvent(const QString& value, bool storedData);

        bool getStoredData() const;
        const QString& getValue() const;


    private:
        QString value;
        bool storedData;
};
