#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT ChromaCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit ChromaCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        float getBlur() const;
        const QString& getKey() const;
        bool  getShowMask() const;
        float getSpread() const;
        float getSpill() const;
        float getThreshold() const;

        void setBlur(float blur);
        void setKey(const QString& key);
        void setShowMask(bool show);
        void setSpread(float stop);
        void setSpill(float spill);
        void setThreshold(float start);

    private:
        bool showMask = false;

        QString key = Mixer::DEFAULT_CHROMAKEY;

        float blur = Mixer::DEFAULT_CHROMABLUR;
        float spread = Mixer::DEFAULT_CHROMABLENDSTOP;
        float spill = Mixer::DEFAULT_CHROMASPILL;
        float threshold = Mixer::DEFAULT_CHROMABLENDSTART;


        Q_SIGNAL void blurChanged(float);
        Q_SIGNAL void keyChanged(const QString&);
        Q_SIGNAL void showMaskChanged(bool);
        Q_SIGNAL void spreadChanged(float);
        Q_SIGNAL void spillChanged(float);
        Q_SIGNAL void thresholdChanged(float);
};
