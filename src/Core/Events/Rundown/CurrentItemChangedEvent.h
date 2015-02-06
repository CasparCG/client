#pragma once

#include "../../Shared.h"
#include "../../Commands/AbstractCommand.h"
#include "../../Models/LibraryModel.h"

#include <QtCore/QSharedPointer>

#include <QtWidgets/QTreeWidgetItem>

class CORE_EXPORT CurrentItemChangedEvent
{
    public:
        explicit CurrentItemChangedEvent(QTreeWidgetItem* current, QTreeWidgetItem* previous);

        QTreeWidgetItem* getCurrentItem() const;
        QTreeWidgetItem* getPreviousItem() const;

    private:
        QTreeWidgetItem* current;
        QTreeWidgetItem* previous;
};
