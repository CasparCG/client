#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT FileRecorderCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit FileRecorderCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getOutput() const;
        const QString& getCodec() const;
        const QString& getPreset() const;
        bool getWithAlpha() const;

        void setOutput(const QString& output);
        void setCodec(const QString& codec);
        void setPreset(const QString& preset);
        void setWithAlpha(bool withAlpha);

    private:
        QString output = FileRecorder::DEFAULT_OUTPUT;
        QString codec = FileRecorder::DEFAULT_CODEC;
        QString preset = FileRecorder::DEFAULT_PRESET;
        bool withAlpha = FileRecorder::DEFAULT_WITH_ALPHA;

        Q_SIGNAL void outputChanged(const QString&);
        Q_SIGNAL void codecChanged(const QString&);
        Q_SIGNAL void presetChanged(const QString&);
        Q_SIGNAL void withAlphaChanged(bool);
};
