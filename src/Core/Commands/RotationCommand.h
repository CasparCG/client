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

class CORE_EXPORT RotationCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit RotationCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        bool getTriggerOnNext() const;
        float getRotation() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getDefer() const;

        void setRotation(float rotation);
        void setTransitionDuration(int transtitionDuration);
        void setTriggerOnNext(bool triggerOnNext);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        float rotation;
        int transtitionDuration;
        QString tween;
        bool triggerOnNext;
        bool defer;

        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void rotationChanged(float);
        Q_SIGNAL void transtitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
