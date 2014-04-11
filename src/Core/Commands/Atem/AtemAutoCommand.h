#pragma once

#include "../../Shared.h"
#include "../AbstractCommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT AtemAutoCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AtemAutoCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getStep() const;
        double getSpeed() const;
        const QString& getTransition() const;
        bool getTriggerOnNext() const;

        void setStep(const QString& step);
        void setSpeed(double speed);
        void setTransition(const QString& transition);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString step;
        double speed;
        QString transition;
        bool triggerOnNext;

        Q_SIGNAL void stepChanged(const QString&);
        Q_SIGNAL void speedChanged(double);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
