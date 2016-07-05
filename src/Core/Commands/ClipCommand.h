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

class CORE_EXPORT ClipCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit ClipCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        float getLeft() const;
        float getTop() const;
        float getWidth() const;
        float getHeight() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getDefer() const;

        void setLeft(float left);
        void setTop(float top);
        void setWidth(float width);
        void setHeight(float height);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        float left = Mixer::DEFAULT_CLIP_LEFT;
        float width = Mixer::DEFAULT_CLIP_WIDTH;
        float top = Mixer::DEFAULT_CLIP_TOP;
        float height = Mixer::DEFAULT_CLIP_HEIGHT;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        bool defer = Mixer::DEFAULT_DEFER;

        Q_SIGNAL void leftChanged(float);
        Q_SIGNAL void widthChanged(float);
        Q_SIGNAL void topChanged(float);
        Q_SIGNAL void heightChanged(float);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
