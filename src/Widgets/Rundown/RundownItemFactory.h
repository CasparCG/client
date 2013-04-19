#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"

#include "Global.h"

#include "Models/LibraryModel.h"

#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownItemFactory : public QWidget
{
    Q_OBJECT

    public:
        explicit RundownItemFactory() {}

        static RundownItemFactory& getInstance();

        AbstractRundownWidget* createWidget(const LibraryModel& model);
};
