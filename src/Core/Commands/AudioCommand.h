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

class CORE_EXPORT AudioCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AudioCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getAudioName() const;
        const QString& getTransition() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        const QString& getDirection() const;
        bool getLoop() const;
        bool getTriggerOnNext() const;
        bool getUseAuto() const;

        void setAudioName(const QString& audioName);
        void setTransition(const QString& transition);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setDirection(const QString& direction);
        void setTriggerOnNext(bool triggerOnNext);
        void setLoop(bool loop);
        void setUseAuto(bool useAuto);

    private:
        QString audioName = Audio::DEFAULT_NAME;
        QString transition = Mixer::DEFAULT_TRANSITION;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        QString direction = Mixer::DEFAULT_DIRECTION;
        bool loop = Audio::DEFAULT_LOOP;
        bool triggerOnNext = Audio::DEFAULT_TRIGGER_ON_NEXT;
        bool useAuto = Audio::DEFAULT_USE_AUTO;

        Q_SIGNAL void audioNameChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void loopChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void useAutoChanged(bool);
};
