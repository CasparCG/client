#pragma once

#include "Shared.h"
#include "ui_PreviewWidget.h"

#include "Animations/BorderAnimation.h"

#include <QtGui/QImage>
#include <QtGui/QWidget>

class WIDGETS_EXPORT PreviewWidget : public QWidget, Ui::PreviewWidget
{
    Q_OBJECT

    public:
        explicit PreviewWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        QImage image;
        bool previewAlpha;

        BorderAnimation* previewAlphaAnimation;

        Q_SLOT void switchPreview();
};
