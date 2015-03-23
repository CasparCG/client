#pragma once

#include "../../Shared.h"
#include "../../Models/LibraryModel.h"

#include <QtCore/QSharedPointer>

class CORE_EXPORT AddRudnownItemEvent
{
    public:
        explicit AddRudnownItemEvent(const LibraryModel& model);

        LibraryModel getLibraryModel() const;

    private:
        LibraryModel model;
};
