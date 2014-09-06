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

class CORE_EXPORT SolidColorCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit SolidColorCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getColor() const;
        const QString getPremultipliedColor() const;
        const QString& getTransition() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        const QString& getDirection() const;
        bool getUseAuto() const;
        bool getTriggerOnNext() const;

        void setColor(const QString& color);
        void setTransition(const QString& transition);
        void setTransitionDuration(int transtitionDuration);
        void setTween(const QString& tween);
        void setDirection(const QString& direction);
        void setUseAuto(bool useAuto);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString color;
        QString transition;
        int transtitionDuration;
        QString tween;
        QString direction;
        bool useAuto;
        bool triggerOnNext;

        Q_SIGNAL void colorChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void transtitionDurationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void useAutoChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
