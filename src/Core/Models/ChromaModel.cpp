#include "ChromaModel.h"

ChromaModel::ChromaModel(int id, const QString& value)
    : id(id), value(value)
{
}

int ChromaModel::getId() const
{
    return this->id;
}

const QString& ChromaModel::getValue() const
{
    return this->value;
}
