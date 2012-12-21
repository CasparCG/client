#pragma once

#include "../Shared.h"
#include "../Commands/ICommand.h"
#include "../Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

class CORE_EXPORT LibraryItemSelectedEvent : public QEvent
{
    public:
        explicit LibraryItemSelectedEvent(ICommand* command, LibraryModel* model);

        ICommand* getCommand() const;
        LibraryModel* getLibraryModel() const;

    private:
        ICommand* command;
        LibraryModel* model;
};
