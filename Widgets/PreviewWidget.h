#pragma once

#include "Shared.h"
#include "ui_PreviewWidget.h"

#include "MediaPlayer.h"

#include <QtGui/QWidget>

class WIDGETS_EXPORT PreviewWidget : public QWidget, Ui::PreviewWidget
{
    Q_OBJECT

    public:
        explicit PreviewWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        QString name;
        QString hostname;
        QString contentType;
        MediaPlayer* player;

        Q_SLOT void play();
};
