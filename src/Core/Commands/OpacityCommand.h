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

class CORE_EXPORT OpacityCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit OpacityCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        float getOpacity() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getTriggerOnNext() const;
        bool getDefer() const;

        void setOpacity(float opacity);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setTriggerOnNext(bool triggerOnNext);
        void setDefer(bool defer);

    private:
        float opacity = Mixer::DEFAULT_OPACITY;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        bool triggerOnNext = Opacity::DEFAULT_TRIGGER_ON_NEXT;
        bool defer = Mixer::DEFAULT_DEFER;

        Q_SIGNAL void opacityChanged(float);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void deferChanged(bool);
};
