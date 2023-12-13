#pragma once

#include "../Shared.h"

#include <QtCore/QString>

#ifndef WIDGETS_REPOSITORY_REPOSITORY_CHANGE_MODEL
#define WIDGETS_REPOSITORY_REPOSITORY_CHANGE_MODEL

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

#endif // WIDGETS_REPOSITORY_REPOSITORY_CHANGE_MODEL
