#pragma once

#include <QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

class Application : public QApplication
{
    public:
        explicit Application(int& argc, char** argv);
        virtual ~Application() {}

    protected:
        virtual bool notify(QObject* receiver, QEvent* event);
};
