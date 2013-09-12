#pragma once

#include <QtGui/QApplication>

class Application : public QApplication
{
    public:
        explicit Application(int& argc, char** argv);
        virtual ~Application() {}

    protected:
        virtual bool notify(QObject* receiver, QEvent* event);
};
