#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT GroupCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit GroupCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        bool getAutoStep() const;
        const QString& getNotes() const;
        bool getAutoPlay() const;

        void setAutoStep(bool autoStep);
        void setNotes(const QString& notes);
        void setAutoPlay(bool autoPlay);

    private:
        QString notes = Group::DEFAULT_NOTE;
        bool autoStep = Group::DEFAULT_AUTO_STEP;
        bool autoPlay = Group::DEFAULT_AUTO_PLAY;

        Q_SIGNAL void notesChanged(const QString&);
        Q_SIGNAL void autoStepChanged(bool);
        Q_SIGNAL void autoPlayChanged(bool);
};
