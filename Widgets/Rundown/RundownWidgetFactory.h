#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"

#include "Global.h"

#include "Models/LibraryModel.h"

#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownWidgetFactory : public QWidget
{
    Q_OBJECT

    public:
        explicit RundownWidgetFactory() {}

        static RundownWidgetFactory& getInstance();

        AbstractRundownWidget* createWidget(const LibraryModel& model);
};
