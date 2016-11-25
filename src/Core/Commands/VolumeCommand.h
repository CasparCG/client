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

class CORE_EXPORT VolumeCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit VolumeCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        float getVolume() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getDefer() const;

        void setVolume(float volume);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        float volume = Mixer::DEFAULT_VOLUME;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        bool defer = Mixer::DEFAULT_DEFER;

        Q_SIGNAL void volumeChanged(float);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
