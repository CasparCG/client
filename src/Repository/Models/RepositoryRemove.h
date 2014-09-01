#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class REPOSITORY_EXPORT RepositoryRemove
{
    public:
        explicit RepositoryRemove(const QString& storyId);

        const QString& getStoryId() const;

    private:
        QString storyId;
};
