#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT AnchorCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AnchorCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        float getPositionX() const;
        float getPositionY() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getTriggerOnNext() const;
        bool getDefer() const;

        void setPositionX(float positionX);
        void setPositionY(float positionY);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setTriggerOnNext(bool triggerOnNext);
        void setDefer(bool defer);

    private:
        float positionX = Mixer::DEFAULT_ANCHOR_XPOS;
        float positionY = Mixer::DEFAULT_ANCHOR_YPOS;

        QString tween = Mixer::DEFAULT_TWEEN;

        bool defer = Mixer::DEFAULT_DEFER;
        bool triggerOnNext = Anchor::DEFAULT_TRIGGER_ON_NEXT;

        int transitionDuration = Mixer::DEFAULT_DURATION;

        Q_SIGNAL void positionXChanged(float);
        Q_SIGNAL void positionYChanged(float);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void deferChanged(bool);
};
