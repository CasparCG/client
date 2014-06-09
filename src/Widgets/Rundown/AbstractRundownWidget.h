#pragma once

#include "../Shared.h"

#include "Global.h"

#include "Commands/AbstractCommand.h"
#include "Commands/AbstractProperties.h"
#include "Models/LibraryModel.h"

#include <QtCore/QString>

class WIDGETS_EXPORT AbstractRundownWidget : public AbstractProperties
{
    public:
        virtual ~AbstractRundownWidget() {}

        virtual AbstractRundownWidget* clone() = 0;

        virtual bool isGroup() const = 0;
        virtual bool isInGroup() const = 0;

        virtual AbstractCommand* getCommand() = 0;
        virtual LibraryModel* getLibraryModel() = 0;

        virtual void setActive(bool active) = 0;
        virtual void setInGroup(bool inGroup) = 0;
        virtual void setColor(const QString& color) = 0;
        virtual QString getColor() const = 0;

        virtual void setCompactView(bool compactView) = 0;

        virtual void setExpanded(bool expanded) = 0;

        virtual void clearDelayedCommands() = 0;

        virtual void setUsed(bool used) = 0;
};
