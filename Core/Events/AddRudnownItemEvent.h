#pragma once

#include "../Shared.h"
#include "../Models/LibraryModel.h"

#include <QtCore/QEvent>
#include <QtCore/QSharedPointer>

class CORE_EXPORT AddRudnownItemEvent : public QEvent
{
    public:
        explicit AddRudnownItemEvent(const LibraryModel& model);

        LibraryModel getLibraryModel();

    private:
        LibraryModel model;
};
