#pragma once

#include "Shared.h"
#include "AbstractCommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT ImageScrollerCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit ImageScrollerCommand(QObject* parent = 0);

        virtual int getDelay() const;
        virtual int getChannel() const;
        virtual int getVideolayer() const;
        virtual bool getAllowGpi() const;

        virtual void setChannel(int channel);
        virtual void setVideolayer(int videolayer);
        virtual void setDelay(int delay);
        virtual void setAllowGpi(bool allowGpi);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getMediaName() const;
        int getBlur() const;
        int getSpeed() const;
        bool getPremultiply() const;
        bool getProgressive() const;

        void setMediaName(const QString& mediaName);
        void setBlur(int blur);
        void setSpeed(int speed);
        void setPremultiply(bool premultiply);
        void setProgressive(bool progressive);

    private:
        int channel;
        int videolayer;
        int delay;
        bool allowGpi;
        QString mediaName;
        int blur;
        int speed;
        bool premultiply;
        bool progressive;

        Q_SIGNAL void allowGpiChanged(bool);
        Q_SIGNAL void channelChanged(int);
        Q_SIGNAL void videolayerChanged(int);
        Q_SIGNAL void delayChanged(int);
        Q_SIGNAL void blurChanged(int);
        Q_SIGNAL void speedChanged(int);
        Q_SIGNAL void progressiveChanged(bool);
        Q_SIGNAL void premultiplyChanged(bool);
        Q_SIGNAL void mediaNameChanged(const QString&);
};
