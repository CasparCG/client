#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

class CORE_EXPORT AddTemplateDataEvent : public QEvent
{
    public:
        explicit AddTemplateDataEvent(const QString& value, bool storedData);

        bool getStoredData();
        const QString& getValue();


    private:
        QString value;
        bool storedData;
};
