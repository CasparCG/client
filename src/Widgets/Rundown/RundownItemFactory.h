#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"

#include "Global.h"

#include "Models/LibraryModel.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class WIDGETS_EXPORT RundownItemFactory : public QWidget
{
    Q_OBJECT

    public:
        explicit RundownItemFactory() {}

        static RundownItemFactory& getInstance();

        AbstractRundownWidget* createWidget(const LibraryModel& model);
};
