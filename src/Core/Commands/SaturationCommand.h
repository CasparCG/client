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

class CORE_EXPORT SaturationCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit SaturationCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        float getSaturation() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getDefer() const;

        void setSaturation(float saturation);
        void setTransitionDuration(int transtitionDuration);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        float saturation;
        int transtitionDuration;
        QString tween;
        bool defer;

        Q_SIGNAL void saturationChanged(float);
        Q_SIGNAL void transtitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
