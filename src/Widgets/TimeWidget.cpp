#include "TimeWidget.h"

#include "Global.h"

#include "DatabaseManager.h"

#include <QtCore/QTime>
#include <QtCore/QTimer>

TimeWidget::TimeWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    QTimer::singleShot(0, this, SLOT(updateTime()));

    qApp->installEventFilter(this);
}

bool TimeWidget::eventFilter(QObject* target, QEvent* event)
{
    return QObject::eventFilter(target, event);
}

void TimeWidget::updateTime()
{
    this->lcdNumber->display(QTime::currentTime().toString("hh:mm:ss"));

    QTimer::singleShot(500, this, SLOT(updateTime()));
}
