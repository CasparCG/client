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

class CORE_EXPORT StillCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit StillCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getImageName() const;
        const QString& getTransition() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        const QString& getDirection() const;
        bool getTriggerOnNext() const;
        bool getUseAuto() const;

        void setImageName(const QString& imageName);
        void setTransition(const QString& transition);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setDirection(const QString& direction);
        void setTriggerOnNext(bool triggerOnNext);
        void setUseAuto(bool useAuto);

    private:
        QString imageName;
        QString transition;
        int transitionDuration;
        QString tween;
        QString direction;
        bool triggerOnNext;
        bool useAuto;

        Q_SIGNAL void imageNameChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void useAutoChanged(bool);
};
