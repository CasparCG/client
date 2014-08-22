#include "RepositoryRemove.h"

RepositoryRemove::RepositoryRemove(const QString& id)
    : id(id)
{
}

const QString& RepositoryRemove::getId() const
{
    return this->id;
}
