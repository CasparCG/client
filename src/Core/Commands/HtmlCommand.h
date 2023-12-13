#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT HtmlCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit HtmlCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter& writer);

        const QString& getUrl() const;
        const QString& getTransition() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        const QString& getDirection() const;
        bool getFreezeOnLoad() const;
        bool getTriggerOnNext() const;
        bool getUseAuto() const;

        void setUrl(const QString& url);
        void setTransition(const QString& transition);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setDirection(const QString& direction);
        void setFreezeOnLoad(bool freezeOnLoad);
        void setTriggerOnNext(bool triggerOnNext);
        void setUseAuto(bool useAuto);

    private:
        QString url = Html::DEFAULT_URL;
        QString transition = Mixer::DEFAULT_TRANSITION;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        QString direction = Mixer::DEFAULT_DIRECTION;
        bool freezeOnLoad = Html::DEFAULT_FREEZE_ON_LOAD;
        bool triggerOnNext = Html::DEFAULT_TRIGGER_ON_NEXT;
        bool useAuto = Html::DEFAULT_USE_AUTO;

        Q_SIGNAL void urlChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void freezeOnLoadChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void useAutoChanged(bool);
};
