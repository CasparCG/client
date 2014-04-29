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

class CORE_EXPORT DeckLinkInputCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit DeckLinkInputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        int getDevice() const;
        const QString& getFormat() const;
        const QString& getTransition() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        const QString& getDirection() const;

        void setDevice(int device);
        void setFormat(const QString& format);
        void setTransition(const QString& transition);
        void setTransitionDuration(int transtitionDuration);
        void setTween(const QString& tween);
        void setDirection(const QString& direction);

    private:
        int device;
        QString format;
        QString transition;
        int transtitionDuration;
        QString tween;
        QString direction;

        Q_SIGNAL void deviceChanged(int);
        Q_SIGNAL void formatChanged(QString);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void transtitionDurationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void resetDevice(QString);
        Q_SIGNAL void resetFormat(QString);
};
