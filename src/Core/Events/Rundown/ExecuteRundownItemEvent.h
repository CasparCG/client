#pragma once

#include "../../Shared.h"

#include "Global.h"

#include <QtCore/QString>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QTreeWidgetItem>
#else
#include <QtGui/QTreeWidgetItem>
#endif

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
