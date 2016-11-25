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

class CORE_EXPORT MovieCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit MovieCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getVideoName() const;
        const QString& getTransition() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        const QString& getDirection() const;
        bool getLoop() const;
        bool getFreezeOnLoad() const;
        bool getTriggerOnNext() const;
        int getSeek() const;
        int getLength() const;
        bool getAutoPlay() const;

        void setVideoName(const QString& videoName);
        void setTransition(const QString& transition);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setDirection(const QString& direction);
        void setFreezeOnLoad(bool freezeOnLoad);
        void setTriggerOnNext(bool triggerOnNext);
        void setLoop(bool loop);
        void setSeek(int seek);
        void setLength(int length);
        void setAutoPlay(bool autoPlay);

    private:
        QString videoName = Movie::DEFAULT_NAME;
        QString transition = Mixer::DEFAULT_TRANSITION;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        QString direction = Mixer::DEFAULT_DIRECTION;
        bool loop = Movie::DEFAULT_LOOP;
        bool freezeOnLoad = Movie::DEFAULT_FREEZE_ON_LOAD;
        bool triggerOnNext = Movie::DEFAULT_TRIGGER_ON_NEXT;
        int seek = Movie::DEFAULT_SEEK;
        int length = Movie::DEFAULT_LENGTH;
        bool autoPlay = Movie::DEFAULT_AUTO_PLAY;

        Q_SIGNAL void videoNameChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void loopChanged(bool);
        Q_SIGNAL void freezeOnLoadChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void seekChanged(int);
        Q_SIGNAL void lengthChanged(int);
        Q_SIGNAL void autoPlayChanged(bool);
};
