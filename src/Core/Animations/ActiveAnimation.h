#pragma once

#include "../Shared.h"

#include <QtCore/QObject>
#include <QtCore/QPropertyAnimation>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif

class CORE_EXPORT ActiveAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int color READ color WRITE setColor)

    public:
        explicit ActiveAnimation(QWidget* target, QObject* parent = 0);

        void start(int loopCount = -1);
        void stop();

    private:
        int value;
        QWidget* target;
        QPropertyAnimation* animation;

        int color() const;
        void setColor(const int value);
};
