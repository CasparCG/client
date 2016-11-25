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

class CORE_EXPORT PerspectiveCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit PerspectiveCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

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
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setTriggerOnNext(bool triggerOnNext);
        void setDefer(bool defer);
        void setUseMipmap(bool useMipmap);

    private:
        float upperLeftX = Mixer::DEFAULT_PERSPECTIVE_UPPERLEFT_X;
        float upperLeftY = Mixer::DEFAULT_PERSPECTIVE_UPPERLEFT_Y;
        float upperRightX = Mixer::DEFAULT_PERSPECTIVE_UPPERRIGHT_X;
        float upperRightY = Mixer::DEFAULT_PERSPECTIVE_UPPERRIGHT_Y;
        float lowerRightX = Mixer::DEFAULT_PERSPECTIVE_LOWERRIGHT_X;
        float lowerRightY = Mixer::DEFAULT_PERSPECTIVE_LOWERRIGHT_Y;
        float lowerLeftX = Mixer::DEFAULT_PERSPECTIVE_LOWERLEFT_X;
        float lowerLeftY = Mixer::DEFAULT_PERSPECTIVE_LOWERLEFT_Y;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        bool triggerOnNext = Fill::DEFAULT_TRIGGER_ON_NEXT;
        bool defer = Mixer::DEFAULT_DEFER;
        bool useMipmap = Mixer::DEFAULT_MIPMAP;

        Q_SIGNAL void upperLeftXChanged(float);
        Q_SIGNAL void upperLeftYChanged(float);
        Q_SIGNAL void upperRightXChanged(float);
        Q_SIGNAL void upperRightYChanged(float);
        Q_SIGNAL void lowerRightXChanged(float);
        Q_SIGNAL void lowerRightYChanged(float);
        Q_SIGNAL void lowerLeftXChanged(float);
        Q_SIGNAL void lowerLeftYChanged(float);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void deferChanged(bool);
        Q_SIGNAL void useMipmapChanged(bool);
};
