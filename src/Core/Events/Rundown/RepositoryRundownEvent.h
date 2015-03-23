#pragma once

#include "../../Shared.h"

class CORE_EXPORT RepositoryRundownEvent
{
    public:
        explicit RepositoryRundownEvent(bool repositoryRundown);

        bool getRepositoryRundown() const;

    private:
        bool repositoryRundown;
};
