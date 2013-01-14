#pragma once

#include "Shared.h"
#include "ICommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT MediaCommand : public QObject, public ICommand
{
    Q_OBJECT

    public:
        explicit MediaCommand(QObject* parent = 0);

        virtual int getDelay() const;
        virtual int getChannel() const;
        virtual int getVideolayer() const;
        virtual bool getAllowGpi() const;

        virtual void setChannel(int channel);
        virtual void setVideolayer(int videolayer);
        virtual void setDelay(int delay);
        virtual void setAllowGpi(bool allowGpi);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getMediaName() const;
        const QString& getTransition() const;
        int getDuration() const;
        const QString& getTween() const;
        const QString& getDirection() const;
        bool getLoop() const;
        bool getPauseOnLoad() const;
        int getSeek() const;
        int getLength() const;

        void setMediaName(const QString& mediaName);
        void setTransition(const QString& transition);
        void setDuration(int duration);
        void setTween(const QString& tween);
        void setDirection(const QString& direction);
        void setPauseOnLoad(bool pauseOnLoad);
        void setLoop(bool loop);
        void setSeek(int seek);
        void setLength(int length);

    private:
        int channel;
        int videolayer;
        int delay;
        bool allowGpi;
        QString mediaName;
        QString transition;
        int duration;
        QString tween;
        QString direction;
        bool loop;
        bool pauseOnLoad;
        int seek;
        int length;

        Q_SIGNAL void allowGpiChanged(bool);
        Q_SIGNAL void channelChanged(int);
        Q_SIGNAL void videolayerChanged(int);
        Q_SIGNAL void delayChanged(int);
        Q_SIGNAL void mediaNameChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void durationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void loopChanged(bool);
        Q_SIGNAL void pauseOnLoadChanged(bool);
        Q_SIGNAL void seekChanged(int);
        Q_SIGNAL void lengthChanged(int);
};
