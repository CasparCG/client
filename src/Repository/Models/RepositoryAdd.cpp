#include "RepositoryAdd.h"

RepositoryAdd::RepositoryAdd(const QString& storyId, const QString& data)
    : storyId(storyId), data(data)
{
}

const QString& RepositoryAdd::getStoryId() const
{
    return this->storyId;
}

const QString& RepositoryAdd::getData() const
{
    return this->data;
}
