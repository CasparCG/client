#pragma once

#include "../../Shared.h"
#include "../../Commands/AbstractCommand.h"
#include "../../Models/LibraryModel.h"

#include <QtCore/QSharedPointer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QTreeWidgetItem>
#else
#include <QtGui/QTreeWidgetItem>
#endif

class CORE_EXPORT RemoveItemFromAutoPlayQueueEvent
{
    public:
        explicit RemoveItemFromAutoPlayQueueEvent(QTreeWidgetItem* item);

        QTreeWidgetItem* getItem() const;

    private:
        QTreeWidgetItem* item;
};
