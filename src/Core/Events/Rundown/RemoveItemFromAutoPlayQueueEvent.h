#pragma once

#include "../../Shared.h"
#include "../../Commands/AbstractCommand.h"
#include "../../Models/LibraryModel.h"

#include <QtCore/QSharedPointer>

#include <QtGui/QTreeWidgetItem>

class CORE_EXPORT RemoveItemFromAutoPlayQueueEvent
{
    public:
        explicit RemoveItemFromAutoPlayQueueEvent(QTreeWidgetItem* item);

        QTreeWidgetItem* getItem() const;

    private:
        QTreeWidgetItem* item;
};
