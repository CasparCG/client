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

class CORE_EXPORT AnchorCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AnchorCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        float getPositionX() const;
        float getPositionY() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getTriggerOnNext() const;
        bool getDefer() const;

        void setPositionX(float positionX);
        void setPositionY(float positionY);
        void setTransitionDuration(int transtitionDuration);
        void setTween(const QString& tween);
        void setTriggerOnNext(bool triggerOnNext);
        void setDefer(bool defer);

    private:
        float positionX;
        float positionY;
        int transtitionDuration;
        QString tween;
        bool triggerOnNext;
        bool defer;

        Q_SIGNAL void positionXChanged(float);
        Q_SIGNAL void positionYChanged(float);
        Q_SIGNAL void transtitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void deferChanged(bool);
};
