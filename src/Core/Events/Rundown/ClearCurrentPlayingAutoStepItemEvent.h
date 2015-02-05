#pragma once

#include "../../Shared.h"
#include "../../Commands/AbstractCommand.h"
#include "../../Models/LibraryModel.h"

#include <QtCore/QSharedPointer>

#include <QtWidgets/QTreeWidgetItem>

class CORE_EXPORT ClearCurrentPlayingAutoStepItemEvent
{
    public:
        explicit ClearCurrentPlayingAutoStepItemEvent(QTreeWidgetItem* item);

        QTreeWidgetItem* getItem() const;

    private:
        QTreeWidgetItem* item;
};
