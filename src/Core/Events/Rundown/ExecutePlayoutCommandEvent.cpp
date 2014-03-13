#include "ExecutePlayoutCommandEvent.h"

ExecutePlayoutCommandEvent::ExecutePlayoutCommandEvent(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers)
    : type(type), key(key), modifiers(modifiers)
{
}

QEvent::Type ExecutePlayoutCommandEvent::getType() const
{
    return this->type;
}

int ExecutePlayoutCommandEvent::getKey() const
{
    return this->key;
}

Qt::KeyboardModifiers ExecutePlayoutCommandEvent::getModifiers() const
{
    return this->modifiers;
}
