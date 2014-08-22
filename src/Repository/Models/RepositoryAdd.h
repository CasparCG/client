#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class REPOSITORY_EXPORT RepositoryAdd
{
    public:
        explicit RepositoryAdd(const QString& id, const QString& data);

        const QString& getId() const;
        const QString& getData() const;

    private:
        QString id;
        QString data;
};
