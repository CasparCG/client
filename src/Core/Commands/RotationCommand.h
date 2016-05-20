#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT RotationCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit RotationCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        bool getTriggerOnNext() const;
        float getRotation() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getDefer() const;

        void setRotation(float rotation);
        void setTransitionDuration(int transitionDuration);
        void setTriggerOnNext(bool triggerOnNext);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        float rotation = Mixer::DEFAULT_ROTATION;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        bool triggerOnNext = Rotation::DEFAULT_TRIGGER_ON_NEXT;
        bool defer = Mixer::DEFAULT_DEFER;

        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void rotationChanged(float);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
