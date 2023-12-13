#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT BrightnessCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit BrightnessCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        float getBrightness() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getDefer() const;

        void setBrightness(float brightness);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        float brightness = Mixer::DEFAULT_BRIGHTNESS;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        bool defer = Mixer::DEFAULT_DEFER;

        Q_SIGNAL void brightnessChanged(float);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
