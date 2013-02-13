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

class CORE_EXPORT ColorProducerCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit ColorProducerCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getMediaName() const;
        const QString& getTransition() const;
        int getDuration() const;
        const QString& getTween() const;
        const QString& getDirection() const;
        bool getLoop() const;
        bool getFreezeOnLoad() const;
        bool getTriggerOnNext() const;
        int getSeek() const;
        int getLength() const;
        bool getUseAuto() const;

        void setMediaName(const QString& mediaName);
        void setTransition(const QString& transition);
        void setDuration(int duration);
        void setTween(const QString& tween);
        void setDirection(const QString& direction);
        void setFreezeOnLoad(bool freezeOnLoad);
        void setTriggerOnNext(bool triggerOnNext);
        void setLoop(bool loop);
        void setSeek(int seek);
        void setLength(int length);
        void setUseAuto(bool useAuto);

    private:
        QString mediaName;
        QString transition;
        int duration;
        QString tween;
        QString direction;
        bool loop;
        bool freezeOnLoad;
        bool triggerOnNext;
        int seek;
        int length;
        bool useAuto;

        Q_SIGNAL void mediaNameChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void durationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void loopChanged(bool);
        Q_SIGNAL void freezeOnLoadChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void seekChanged(int);
        Q_SIGNAL void lengthChanged(int);
        Q_SIGNAL void useAutoChanged(bool);
};
