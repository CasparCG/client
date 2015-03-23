#include "DeleteRundownEvent.h"

#include "Global.h"

DeleteRundownEvent::DeleteRundownEvent(int index)
    : index(index)
{
}

int DeleteRundownEvent::getIndex() const
{
    return this->index;
}
