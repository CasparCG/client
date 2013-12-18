#pragma once

#include "../../Shared.h"

#include "Global.h"

#include <QtCore/QEvent>
#include <QtCore/QString>

#include <QtGui/QTreeWidgetItem>

class CORE_EXPORT ExecuteRundownItemEvent : public QEvent
{
    public:
        explicit ExecuteRundownItemEvent(Playout::PlayoutType::Type type, QTreeWidgetItem* item);

        Playout::PlayoutType::Type getType() const;
        QTreeWidgetItem* getItem() const;

    private:
        Playout::PlayoutType::Type type;
        QTreeWidgetItem* item;
};
