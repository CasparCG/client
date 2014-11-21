#pragma once

#include "../Shared.h"

#include <QtCore/QObject>

#include <QtGui/QMouseEvent>

#include <QtWidgets/QComboBox>

class WIDGETS_EXPORT TargetComboBaseWidget : public QComboBox
{
    Q_OBJECT

    public:
        explicit TargetComboBaseWidget(QWidget* parent = 0);

        const QString& getPreviousText() const;

    protected:
        virtual void mousePressEvent(QMouseEvent* event);

    private:
        QString previousText;
};
