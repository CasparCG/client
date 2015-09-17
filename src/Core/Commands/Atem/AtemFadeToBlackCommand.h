#pragma once

#include "../../Shared.h"
#include "../AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QObject>
#include <QtCore/QString>

class QXmlStreamWriter;

class CORE_EXPORT AtemFadeToBlackCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AtemFadeToBlackCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        double getSpeed() const;
        bool getTriggerOnNext() const;
        const QString& getMixerStep() const;

        void setSpeed(double speed);
        void setTriggerOnNext(bool triggerOnNext);
        void setMixerStep(const QString& mixerStep);

    private:
        double speed = Atem::DEFAULT_SPEED;
        bool triggerOnNext = Atem::DEFAULT_TRIGGER_ON_NEXT;
        QString mixerStep = Atem::DEFAULT_MIXER_STEP;

        Q_SIGNAL void speedChanged(double);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void mixerStepChanged(const QString&);
};
