#pragma once

#include "../../Shared.h"
#include "../../Commands/AbstractCommand.h"
#include "../../Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

#include <QtGui/QWidget>

class CORE_EXPORT RundownItemSelectedEvent : public QEvent
{
    public:
        explicit RundownItemSelectedEvent(AbstractCommand* command, LibraryModel* model, QWidget* source = NULL, QWidget* parent = NULL);

        AbstractCommand* getCommand() const;
        LibraryModel* getLibraryModel() const;
        QWidget* getSource() const;
        QWidget* getParent() const;

    private:
        AbstractCommand* command;
        LibraryModel* model;
        QWidget* source;
        QWidget* parent;
};
