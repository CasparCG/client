#include "RepositoryRemove.h"

RepositoryRemove::RepositoryRemove(const QString& storyId)
    : storyId(storyId)
{
}

const QString& RepositoryRemove::getStoryId() const
{
    return this->storyId;
}
