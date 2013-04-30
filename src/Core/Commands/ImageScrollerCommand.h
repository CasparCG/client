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

class CORE_EXPORT ImageScrollerCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit ImageScrollerCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getImageScrollerName() const;
        int getBlur() const;
        int getSpeed() const;
        bool getPremultiply() const;
        bool getProgressive() const;

        void setImageScrollerName(const QString& imageScrollerName);
        void setBlur(int blur);
        void setSpeed(int speed);
        void setPremultiply(bool premultiply);
        void setProgressive(bool progressive);

    private:
        QString imageScrollerName;
        int blur;
        int speed;
        bool premultiply;
        bool progressive;

        Q_SIGNAL void blurChanged(int);
        Q_SIGNAL void speedChanged(int);
        Q_SIGNAL void progressiveChanged(bool);
        Q_SIGNAL void premultiplyChanged(bool);
        Q_SIGNAL void imageScrollerNameChanged(const QString&);
};
