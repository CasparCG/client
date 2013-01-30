#pragma once

#include "../Shared.h"
#include "AbstractProperties.h"

#include <QtCore/QObject>

class CORE_EXPORT AbstractCommand : public QObject, public AbstractProperties
{
    Q_OBJECT

    public:
        virtual ~AbstractCommand();

        virtual int getDelay() const = 0;
        virtual int getChannel() const = 0;
        virtual int getVideolayer() const = 0;
        virtual bool getAllowGpi() const = 0;

        virtual void setDelay(int delay) = 0;
        virtual void setChannel(int channel) = 0;
        virtual void setVideolayer(int videoLayer) = 0;
        virtual void setAllowGpi(bool allowGpi) = 0;

    protected:
        explicit AbstractCommand(QObject* parent = 0);
};
