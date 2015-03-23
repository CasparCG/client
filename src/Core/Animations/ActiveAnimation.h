#pragma once

#include "../Shared.h"

#include <QtCore/QObject>

class QWidget;
class QPropertyAnimation;

class CORE_EXPORT ActiveAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int color READ color WRITE setColor)

    public:
        explicit ActiveAnimation(QWidget* target, QObject* parent = 0);

        void start(int loopCount = -1);
        void stop();

    private:
        int value = 255;
        QWidget* target = nullptr;
        QPropertyAnimation* animation = nullptr;

        int color() const;
        void setColor(const int value);
};
