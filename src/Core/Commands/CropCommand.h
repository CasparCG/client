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

class CORE_EXPORT CropCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit CropCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        float getLeft() const;
        float getTop() const;
        float getRight() const;
        float getBottom() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getDefer() const;

        void setLeft(float left);
        void setTop(float top);
        void setRight(float right);
        void setBottom(float bottom);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        float left = Mixer::DEFAULT_CROP_LEFT;
        float top = Mixer::DEFAULT_CROP_TOP;
        float right = Mixer::DEFAULT_CROP_RIGHT;
        float bottom = Mixer::DEFAULT_CROP_BOTTOM;
        int transitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        bool defer = Mixer::DEFAULT_DEFER;

        Q_SIGNAL void leftChanged(float);
        Q_SIGNAL void topChanged(float);
        Q_SIGNAL void rightChanged(float);
        Q_SIGNAL void bottomChanged(float);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
