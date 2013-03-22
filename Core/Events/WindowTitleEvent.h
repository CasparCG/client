#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT WindowTitleEvent : public QEvent
{
    public:
        explicit WindowTitleEvent(const QString& title);

        const QString& getTitle() const;

    private:
        QString title;
};
