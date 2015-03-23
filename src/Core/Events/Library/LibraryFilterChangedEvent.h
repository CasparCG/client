#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT LibraryFilterChangedEvent
{
    public:
        explicit LibraryFilterChangedEvent(const QString& filter);

        const QString& getFilter() const;

    private:
        QString filter;
};
