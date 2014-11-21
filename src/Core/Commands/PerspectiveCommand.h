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

class CORE_EXPORT PerspectiveCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PerspectiveCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        float getUpperLeftX() const;
        float getUpperLeftY() const;
        float getUpperRightX() const;
        float getUpperRightY() const;
        float getLowerRightX() const;
        float getLowerRightY() const;
        float getLowerLeftX() const;
        float getLowerLeftY() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getTriggerOnNext() const;
        bool getDefer() const;
        bool getUseMipmap() const;

        void setUpperLeftX(float upperLeftX);
        void setUpperLeftY(float upperLeftY);
        void setUpperRightX(float upperRightX);
        void setUpperRightY(float upperRightY);
        void setLowerRightX(float lowerRightX);
        void setLowerRightY(float lowerRightY);
        void setLowerLeftX(float lowerLeftX);
        void setLowerLeftY(float lowerLeftY);
        void setTransitionDuration(int transtitionDuration);
        void setTween(const QString& tween);
        void setTriggerOnNext(bool triggerOnNext);
        void setDefer(bool defer);
        void setUseMipmap(bool useMipmap);

    private:
        float upperLeftX;
        float upperLeftY;
        float upperRightX;
        float upperRightY;
        float lowerRightX;
        float lowerRightY;
        float lowerLeftX;
        float lowerLeftY;
        int transtitionDuration;
        QString tween;
        bool triggerOnNext;
        bool defer;
        bool useMipmap;

        Q_SIGNAL void upperLeftXChanged(float);
        Q_SIGNAL void upperLeftYChanged(float);
        Q_SIGNAL void upperRightXChanged(float);
        Q_SIGNAL void upperRightYChanged(float);
        Q_SIGNAL void lowerRightXChanged(float);
        Q_SIGNAL void lowerRightYChanged(float);
        Q_SIGNAL void lowerLeftXChanged(float);
        Q_SIGNAL void lowerLeftYChanged(float);
        Q_SIGNAL void transtitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void deferChanged(bool);
        Q_SIGNAL void useMipmapChanged(bool);
};
