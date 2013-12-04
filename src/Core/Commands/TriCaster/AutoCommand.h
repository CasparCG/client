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

class CORE_EXPORT AutoCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AutoCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getStep() const;
        const QString& getSpeed() const;
        const QString& getTransition() const;

        void setStep(const QString& step);
        void setSpeed(const QString& speed);
        void setTransition(const QString& transition);

    private:
        QString step;
        QString speed;
        QString transition;

        Q_SIGNAL void stepChanged(const QString&);
        Q_SIGNAL void speedChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
};
