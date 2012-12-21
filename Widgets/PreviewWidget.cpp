#include "PreviewWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/RundownModel.h"

#include <QtCore/QUrl>

#include <QtNetwork/QFtp>

PreviewWidget::PreviewWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    this->player = new MediaPlayer(this->widgetVideo);

    qApp->installEventFilter(this);
}

bool PreviewWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
    }

    return QObject::eventFilter(target, event);
}

void PreviewWidget::play()
{
    if (this->player->isPlaying())
    {
        this->player->stop();
        if(!this->player->isPlaying())
            this->pushButtonPlay->setIcon(QIcon(":/Graphics/Images/Play.png"));
    }
    else
    {
        if (this->contentType.isEmpty() || this->contentType != "MOVIE")
            return;

        const QString& path = QString("\\\\%1\\caspar\\_media\\%2").arg(this->hostname, this->name);
        //this->player->play(QUrl(path));
        //this->player->play(QUrl::fromLocalFile("/Users/Peter/Desktop/IMG_0086.mov"));
        if (this->player->isPlaying())
            this->pushButtonPlay->setIcon(QIcon(":/Graphics/Images/Stop.png"));
     }
}
