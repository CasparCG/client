#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

class QObject;

class CORE_EXPORT ResetCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit ResetCommand(QObject* parent = 0);
};
