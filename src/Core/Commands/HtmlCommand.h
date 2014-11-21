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

class CORE_EXPORT HtmlCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit HtmlCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

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
        QString url;
        QString transition;
        int transitionDuration;
        QString tween;
        QString direction;
        bool freezeOnLoad;
        bool triggerOnNext;
        bool useAuto;

        Q_SIGNAL void urlChanged(const QString&);
        Q_SIGNAL void transitionChanged(const QString&);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void directionChanged(const QString&);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void freezeOnLoadChanged(bool);
        Q_SIGNAL void triggerOnNextChanged(bool);
        Q_SIGNAL void useAutoChanged(bool);
};
