#include "PreviewWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/Library/LibraryItemSelectedEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"
#include "Models/ThumbnailModel.h"

PreviewWidget::PreviewWidget(QWidget* parent)
    : QWidget(parent),
      previewAlpha(false)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool PreviewWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Event::EventType::LibraryItemSelected) ||
        event->type() == static_cast<QEvent::Type>(Event::EventType::RundownItemSelected))
    {
        QString name;
        QString deviceName;
        int thumbnailId = 0;
        if (dynamic_cast<LibraryItemSelectedEvent*>(event))
        {
            LibraryItemSelectedEvent* libraryItemSelectedEvent = dynamic_cast<LibraryItemSelectedEvent*>(event);
            thumbnailId = libraryItemSelectedEvent->getLibraryModel()->getThumbnailId();
            name = libraryItemSelectedEvent->getLibraryModel()->getName();
            deviceName = libraryItemSelectedEvent->getLibraryModel()->getDeviceName();
        }
        else if (dynamic_cast<RundownItemSelectedEvent*>(event))
        {
            RundownItemSelectedEvent* rundownItemSelected = dynamic_cast<RundownItemSelectedEvent*>(event);
            thumbnailId = rundownItemSelected->getLibraryModel()->getThumbnailId();
            name = rundownItemSelected->getLibraryModel()->getName();
            deviceName = rundownItemSelected->getLibraryModel()->getDeviceName();
        }

        QString data = DatabaseManager::getInstance().getThumbnailById(thumbnailId).getData();
        if (data.isEmpty())
            data = DatabaseManager::getInstance().getThumbnailByNameAndDeviceName(name, deviceName).getData();

        if (!data.isEmpty())
        {
            this->image.loadFromData(QByteArray::fromBase64(data.toAscii()), "PNG");

            if (this->previewAlpha)
                this->labelPreview->setPixmap(QPixmap::fromImage(this->image.alphaChannel()));
            else
                this->labelPreview->setPixmap(QPixmap::fromImage(this->image));
        }
        else
        {
            this->labelPreview->setPixmap(NULL);
        }
    }

    return QObject::eventFilter(target, event);
}

void PreviewWidget::switchPreview()
{
    if (this->previewAlpha)
    {
        this->toolButtonPreviewAlpha->setStyleSheet("");
        this->labelPreview->setPixmap(QPixmap::fromImage(this->image));
    }
    else
    {
        this->toolButtonPreviewAlpha->setStyleSheet("border-color: firebrick;");
        this->labelPreview->setPixmap(QPixmap::fromImage(this->image.alphaChannel()));
    }

    this->previewAlpha = !this->previewAlpha;
}
