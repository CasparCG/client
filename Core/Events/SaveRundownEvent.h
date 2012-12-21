#pragma once

#include "../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT SaveRundownEvent : public QEvent
{
    public:
        explicit SaveRundownEvent(const QString& path);

        const QString& getPath() const;

    private:
        QString path;
};
