#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT MovieCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit MovieCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

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
        QString videoName;
        QString transition;
        int transitionDuration;
        QString tween;
        QString direction;
        bool loop;
        bool freezeOnLoad;
        bool triggerOnNext;
        int seek;
        int length;
        bool autoPlay;

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
