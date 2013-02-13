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

class CORE_EXPORT ColorCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit ColorCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getColor() const;
        const QString& getTransition() const;
        int getDuration() const;
        const QString& getTween() const;
        const QString& getDirection() const;
        bool getUseAuto() const;

        void setColor(const QString& color);
        void setTransition(const QString& transition);
        void setDuration(int duration);
        void setTween(const QString& tween);
        void setDirection(const QString& direction);
        void setUseAuto(bool useAuto);

    private:
        QString color;
        QString transition;
        int duration;
        QString tween;
        QString direction;
        bool useAuto;

        Q_SIGNAL void colorChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void durationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void useAutoChanged(bool);
};
