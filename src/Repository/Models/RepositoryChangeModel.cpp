#include "RepositoryChangeModel.h"

RepositoryChangeModel::RepositoryChangeModel(const QString& type, const QString& storyId, const QString& data)
    : type(type), storyId(storyId), data(data)
{
}

const QString& RepositoryChangeModel::getType() const
{
    return this->type;
}

const QString& RepositoryChangeModel::getStoryId() const
{
    return this->storyId;
}

const QString& RepositoryChangeModel::getData() const
{
    return this->data;
}
