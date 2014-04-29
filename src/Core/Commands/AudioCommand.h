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
        QString audioName;
        QString transition;
        int transitionDuration;
        QString tween;
        QString direction;
        bool loop;
        bool triggerOnNext;
        bool useAuto;

        Q_SIGNAL void audioNameChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void loopChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void useAutoChanged(bool);
};
