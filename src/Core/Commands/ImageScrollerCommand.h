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
        QString imageScrollerName = ImageScroller::DEFAULT_NAME;
        int blur = ImageScroller::DEFAULT_BLUR;
        int speed = ImageScroller::DEFAULT_SPEED;
        bool premultiply = ImageScroller::DEFAULT_PREMULTIPLY;
        bool progressive = ImageScroller::DEFAULT_PROGRESSIVE;

        Q_SIGNAL void blurChanged(int);
        Q_SIGNAL void speedChanged(int);
        Q_SIGNAL void progressiveChanged(bool);
        Q_SIGNAL void premultiplyChanged(bool);
        Q_SIGNAL void imageScrollerNameChanged(const QString&);
};
