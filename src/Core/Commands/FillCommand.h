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

class CORE_EXPORT FillCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit FillCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        float getPositionX() const;
        float getPositionY() const;
        float getScaleX() const;
        float getScaleY() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getTriggerOnNext() const;
        bool getDefer() const;
        bool getUseMipmap() const;

        void setPositionX(float positionX);
        void setPositionY(float positionY);
        void setScaleX(float scaleX);
        void setScaleY(float scaleY);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setTriggerOnNext(bool triggerOnNext);
        void setDefer(bool defer);
        void setUseMipmap(bool useMipmap);

    private:
        float positionX = Mixer::DEFAULT_FILL_XPOS;
        float positionY = Mixer::DEFAULT_FILL_YPOS;
        float scaleX = Mixer::DEFAULT_FILL_XSCALE;
        float scaleY = Mixer::DEFAULT_FILL_YSCALE;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        bool triggerOnNext = Fill::DEFAULT_TRIGGER_ON_NEXT;
        bool defer = Mixer::DEFAULT_DEFER;
        bool useMipmap = Mixer::DEFAULT_MIPMAP;

        Q_SIGNAL void positionXChanged(float);
        Q_SIGNAL void positionYChanged(float);
        Q_SIGNAL void scaleXChanged(float);
        Q_SIGNAL void scaleYChanged(float);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void deferChanged(bool);
        Q_SIGNAL void useMipmapChanged(bool);
};
