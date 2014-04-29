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

class CORE_EXPORT GridCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit GridCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        int getGrid() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getDefer() const;

        void setGrid(int grid);
        void setTransitionDuration(int transitionDuration);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        int grid;
        int transitionDuration;
        QString tween;
        bool defer;

        Q_SIGNAL void gridChanged(int);
        Q_SIGNAL void transitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
