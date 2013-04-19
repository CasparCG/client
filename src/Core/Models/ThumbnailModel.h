#pragma once

#include "../Shared.h"

#include <QtCore/QObject>
#include <QtCore/QString>

class CORE_EXPORT ThumbnailModel
{
    public:
        explicit ThumbnailModel() { }
        explicit ThumbnailModel(int id, const QString& data, const QString& timestamp, const QString& size, const QString& name, const QString& address);

        int getId() const;
        const QString& getData() const;
        const QString& getTimestamp() const;
        const QString& getSize() const;
        const QString& getName() const;
        const QString& getAddress() const;

    private:
        int id;
        QString data;
        QString timestamp;
        QString size;
        QString name;
        QString address;
};
