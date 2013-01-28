#pragma once

#include "../Shared.h"

#include <QtCore/QObject>
#include <QtCore/QPropertyAnimation>

#include <QtGui/QWidget>

class CORE_EXPORT AlphaAnimation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int alpha READ alpha WRITE setAlpha)

    public:
        explicit AlphaAnimation(QWidget* target, QObject* parent = 0);

        void start(int loopCount = -1);

    private:
        int value;
        QWidget* target;
        QPropertyAnimation* animation;

        int alpha() const;
        void setAlpha(const int value);
};
