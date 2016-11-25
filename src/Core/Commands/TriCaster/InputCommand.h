#pragma once

#include "../../Shared.h"
#include "../AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT InputCommand: public AbstractCommand
{
    Q_OBJECT

    public:
        explicit InputCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getSwitcher() const;
        const QString& getInput() const;
        bool getTriggerOnNext() const;

        void setSwitcher(const QString& switcher);
        void setInput(const QString& input);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString switcher = TriCaster::DEFAULT_SWITCHER;
        QString input = TriCaster::DEFAULT_INPUT;
        bool triggerOnNext = TriCaster::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void switcherChanged(const QString&);
        Q_SIGNAL void inputChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
