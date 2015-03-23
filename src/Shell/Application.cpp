#include "Application.h"

#include <exception>

#include <QtCore/QDebug>

Application::Application(int& argc, char** argv) : QApplication(argc, argv)
{
}

bool Application::notify(QObject* receiver, QEvent* event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch (std::exception& e)
    {
        qCritical() << "Exception thrown: " << e.what();
    }

    return false;
}
