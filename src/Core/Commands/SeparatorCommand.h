#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

class QObject;

class CORE_EXPORT SeparatorCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit SeparatorCommand(QObject* parent = 0);
};
