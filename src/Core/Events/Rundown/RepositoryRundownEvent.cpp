#include "RepositoryRundownEvent.h"

#include "Global.h"

RepositoryRundownEvent::RepositoryRundownEvent(bool repositoryRundown)
    : repositoryRundown(repositoryRundown)
{
}

bool RepositoryRundownEvent::getRepositoryRundown() const
{
    return this->repositoryRundown;
}
