#pragma once

#include "../../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT SaveRundownEvent
{
    public:
        explicit SaveRundownEvent(bool saveAs);

        bool getSaveAs() const;

    private:
        bool saveAs;
};
