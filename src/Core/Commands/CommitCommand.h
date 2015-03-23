#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

class QObject;

class CORE_EXPORT CommitCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit CommitCommand(QObject* parent = 0);
};
