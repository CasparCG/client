#pragma once

#include "../../Shared.h"

#include "Global.h"

#include <QtCore/QString>

#include <QtGui/QTreeWidgetItem>

class CORE_EXPORT ExecuteRundownItemEvent
{
    public:
        explicit ExecuteRundownItemEvent(Playout::PlayoutType::Type type, QTreeWidgetItem* item, const QString& address = "");

        Playout::PlayoutType::Type getType() const;
        QTreeWidgetItem* getItem() const;
        QString getAddress() const;

    private:
        Playout::PlayoutType::Type type;

        QTreeWidgetItem* item;
        QString address;
};
