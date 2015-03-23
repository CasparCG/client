#pragma once

#include "../../Shared.h"
#include "../../Commands/AbstractCommand.h"
#include "../../Models/LibraryModel.h"

#include <QtCore/QSharedPointer>

class CORE_EXPORT LibraryItemSelectedEvent
{
    public:
        explicit LibraryItemSelectedEvent(AbstractCommand* command, LibraryModel* model);

        AbstractCommand* getCommand() const;
        LibraryModel* getLibraryModel() const;

    private:
        AbstractCommand* command;
        LibraryModel* model;
};
