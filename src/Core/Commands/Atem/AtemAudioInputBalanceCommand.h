#pragma once

#include "../../Shared.h"
#include "../AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QObject>
#include <QtCore/QString>

class QXmlStreamWriter;

class CORE_EXPORT AtemAudioInputBalanceCommand: public AbstractCommand
{
    Q_OBJECT

    public:
        explicit AtemAudioInputBalanceCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getInput() const;
        float getBalance() const;
        bool getTriggerOnNext() const;

        void setInput(const QString& input);
        void setBalance(float balance);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString input = Atem::DEFAULT_AUDIO_INPUT;
        float balance = Atem::DEFAULT_AUDIO_INPUT_BALANCE;
        bool triggerOnNext = Atem::DEFAULT_TRIGGER_ON_NEXT;

        Q_SIGNAL void inputChanged(const QString&);
        Q_SIGNAL void balanceChanged(float);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
