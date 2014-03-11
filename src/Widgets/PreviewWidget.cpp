#include "PreviewWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/LibraryModel.h"
#include "Models/ThumbnailModel.h"

PreviewWidget::PreviewWidget(QWidget* parent)
    : QWidget(parent),
      previewAlpha(false)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(libraryItemSelected(const LibraryItemSelectedEvent&)), this, SLOT(libraryItemSelected(const LibraryItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}







void PreviewWidget::libraryItemSelected(const LibraryItemSelectedEvent& event)
{
    int thumbnailId = event.getLibraryModel()->getThumbnailId();
    QString name = event.getLibraryModel()->getName();
    QString deviceName = event.getLibraryModel()->getDeviceName();

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

void PreviewWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    int thumbnailId = event.getLibraryModel()->getThumbnailId();
    QString name = event.getLibraryModel()->getName();
    QString deviceName = event.getLibraryModel()->getDeviceName();

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
