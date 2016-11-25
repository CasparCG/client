#pragma once

#include "../../Shared.h"
#include "../AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT AutoCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AutoCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getStep() const;
        const QString& getSpeed() const;
        const QString& getTransition() const;
        bool getTriggerOnNext() const;

        void setStep(const QString& step);
        void setSpeed(const QString& speed);
        void setTransition(const QString& transition);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString step = "";
        QString speed = TriCaster::DEFAULT_SPEED;
        QString transition = TriCaster::DEFAULT_TRANSITION;
        bool triggerOnNext = TriCaster::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void stepChanged(const QString&);
        Q_SIGNAL void speedChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
