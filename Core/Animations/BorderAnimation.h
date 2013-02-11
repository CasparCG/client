#pragma once

#include "../Shared.h"

#include <QtCore/QObject>
#include <QtCore/QPropertyAnimation>

#include <QtGui/QWidget>

class CORE_EXPORT BorderAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int alpha READ alpha WRITE setAlpha)

    public:
        explicit BorderAnimation(QWidget* target, QObject* parent = 0);

        void start(int loopCount = -1);
        void stop();

    private:
        int value;
        QWidget* target;
        QPropertyAnimation* animation;

        int alpha() const;
        void setAlpha(const int value);
};
