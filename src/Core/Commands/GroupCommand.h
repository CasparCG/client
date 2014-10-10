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

class CORE_EXPORT GroupCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit GroupCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        bool getAutoStep() const;
        const QString& getNotes() const;
        bool getAutoPlay() const;
        const QString& getCountdown() const;

        void setAutoStep(bool autoStep);
        void setNotes(const QString& notes);
        void setAutoPlay(bool autoPlay);
        void setCountdown(const QString& countdown);

    private:
        QString notes;
        bool autoStep;
        bool autoPlay;
        QString countdown;

        Q_SIGNAL void notesChanged(const QString&);
        Q_SIGNAL void autoStepChanged(bool);
        Q_SIGNAL void autoPlayChanged(bool);
        Q_SIGNAL void countdownChanged(const QString&);
};
