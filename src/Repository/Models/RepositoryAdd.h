#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class REPOSITORY_EXPORT RepositoryAdd
{
    public:
        explicit RepositoryAdd(const QString& storyId, const QString& data);

        const QString& getStoryId() const;
        const QString& getData() const;

    private:
        QString storyId;
        QString data;
};
