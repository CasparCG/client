#include "PreviewWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/LibraryItemSelectedEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"
#include "Models/ThumbnailModel.h"

PreviewWidget::PreviewWidget(QWidget* parent)
    : QWidget(parent),
      alphaPreview(false)
{
    setupUi(this);

    this->alphaPreviewAnimation = new BorderAnimation(this->toolButtonAlphaPreview);

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
            this->image.loadFromData(QByteArray::fromBase64(data.toAscii()), "PNG");

            if (this->alphaPreview)
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
    if (this->alphaPreview)
    {
        this->alphaPreviewAnimation->stop();
        this->labelPreview->setPixmap(QPixmap::fromImage(this->image));
    }
    else
    {
        this->alphaPreviewAnimation->start();
        this->labelPreview->setPixmap(QPixmap::fromImage(this->image.alphaChannel()));
    }

    this->alphaPreview = !this->alphaPreview;
}
