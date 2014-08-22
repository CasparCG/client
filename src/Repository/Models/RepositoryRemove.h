#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class REPOSITORY_EXPORT RepositoryRemove
{
    public:
        explicit RepositoryRemove(const QString& id);

        const QString& getId() const;

    private:
        QString id;
};
