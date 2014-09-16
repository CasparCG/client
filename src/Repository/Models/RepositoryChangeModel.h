#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class REPOSITORY_EXPORT RepositoryChangeModel
{
    public:
        explicit RepositoryChangeModel(const QString& type, const QString& storyId, const QString& data = "");

        const QString& getType() const;
        const QString& getStoryId() const;
        const QString& getData() const;

    private:
        QString type;
        QString storyId;
        QString data;
};
