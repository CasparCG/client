#include "RepositoryAdd.h"

RepositoryAdd::RepositoryAdd(const QString& id, const QString& data)
    : id(id), data(data)
{
}

const QString& RepositoryAdd::getId() const
{
    return this->id;
}

const QString& RepositoryAdd::getData() const
{
    return this->data;
}
