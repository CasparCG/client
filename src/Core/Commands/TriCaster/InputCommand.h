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

class CORE_EXPORT InputCommand: public AbstractCommand
{
    Q_OBJECT

    public:
        explicit InputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getSwitcher() const;
        const QString& getInput() const;
        bool getTriggerOnNext() const;

        void setSwitcher(const QString& switcher);
        void setInput(const QString& input);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString switcher;
        QString input;
        bool triggerOnNext;

        Q_SIGNAL void switcherChanged(const QString&);
        Q_SIGNAL void inputChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
