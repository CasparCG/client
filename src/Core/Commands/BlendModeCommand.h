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

class CORE_EXPORT BlendModeCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit BlendModeCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getBlendMode() const;

        void setBlendMode(const QString& blendMode);

    private:
        QString blendMode;

        Q_SIGNAL void blendModeChanged(const QString&);
};
