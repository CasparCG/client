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

        void setAutoStep(bool autoStep);
        void setNotes(const QString& notes);

    private:
        QString notes;
        bool autoStep;

        Q_SIGNAL void notesChanged(const QString&);
        Q_SIGNAL void autoStepChanged(bool);
};
