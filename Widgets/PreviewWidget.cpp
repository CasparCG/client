#include "PreviewWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/LibraryItemSelectedEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"
#include "Models/ThumbnailModel.h"

PreviewWidget::PreviewWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool PreviewWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::LibraryItemSelected) ||
        event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        int thumbnailId = 0;
        if (dynamic_cast<LibraryItemSelectedEvent*>(event))
        {
            LibraryItemSelectedEvent* libraryItemSelectedEvent = dynamic_cast<LibraryItemSelectedEvent*>(event);
            thumbnailId = libraryItemSelectedEvent->getLibraryModel()->getThumbnailId();
        }
        else if (dynamic_cast<RundownItemSelectedEvent*>(event))
        {
            RundownItemSelectedEvent* rundownItemSelected = dynamic_cast<RundownItemSelectedEvent*>(event);
            thumbnailId = rundownItemSelected->getLibraryModel()->getThumbnailId();
        }

        if (thumbnailId > 0)
        {
            QString data = DatabaseManager::getInstance().getThumbnailById(thumbnailId).getData();
            QImage image;
            image.loadFromData(QByteArray::fromBase64(data.toAscii()), "PNG");
            this->labelThumbnail->setPixmap(QPixmap::fromImage(image));
        }
        else
        {
            this->labelThumbnail->setPixmap(NULL);
        }
    }

    return QObject::eventFilter(target, event);
}
