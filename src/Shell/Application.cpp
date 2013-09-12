#include "Application.h"

#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QDebug>

Application::Application(int& argc, char** argv) : QApplication(argc, argv)
{
}

bool Application::notify(QObject * receiver, QEvent * event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (boost::property_tree::file_parser_error& e)
    {
        qCritical() << "Exception thrown: " << e.what();
    }

    return false;
}
