#pragma once

#include "../../Shared.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

class CORE_EXPORT SaveRundownEvent : public QEvent
{
    public:
        explicit SaveRundownEvent(bool saveAs);

        bool getSaveAs() const;

    private:
        bool saveAs;
};
