#include "ChromaModel.h"

ChromaModel::ChromaModel(int id, const QString& key)
    : id(id), key(key)
{
}

int ChromaModel::getId() const
{
    return this->id;
}

const QString& ChromaModel::getKey() const
{
    return this->key;
}
