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
        void setTransitionDuration(int transtitionDuration);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        float left;
        float width;
        float top;
        float height;
        int transtitionDuration;
        QString tween;
        bool defer;

        Q_SIGNAL void leftChanged(float);
        Q_SIGNAL void widthChanged(float);
        Q_SIGNAL void topChanged(float);
        Q_SIGNAL void heightChanged(float);
        Q_SIGNAL void transtitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
