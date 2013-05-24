#include "OscWidget.h"

#include "Global.h"

OscWidget::OscWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool OscWidget::eventFilter(QObject* target, QEvent* event)
{
    return QObject::eventFilter(target, event);
}
