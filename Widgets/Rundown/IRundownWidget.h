#pragma once

#include "../Shared.h"

#include "Global.h"

#include "Commands/ICommand.h"
#include "Models/LibraryModel.h"

#include <QtCore/QString>

class WIDGETS_EXPORT IRundownWidget
{
    public:
        virtual ~IRundownWidget() {}

        virtual IRundownWidget* clone() = 0;

        virtual bool isGroup() const = 0;

        virtual ICommand* getCommand() = 0;
        virtual LibraryModel* getLibraryModel() = 0;

        virtual void setActive(bool active) = 0;
        virtual void setInGroup(bool inGroup) = 0;
        virtual void setColor(const QString& color) = 0;

        virtual void setExpanded(bool expanded) {}
};
