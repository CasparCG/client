#include "PreviewWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"
#include "Models/LibraryModel.h"
#include "Models/ThumbnailModel.h"

#include <QtGui/QToolButton>

PreviewWidget::PreviewWidget(QWidget* parent)
    : QWidget(parent),
      viewAlpha(false), collapsed(false)
{
    setupUi(this);
    setupMenus();

    QObject::connect(&EventManager::getInstance(), SIGNAL(libraryItemSelected(const LibraryItemSelectedEvent&)), this, SLOT(libraryItemSelected(const LibraryItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(targetChanged(const TargetChangedEvent&)), this, SLOT(targetChanged(const TargetChangedEvent&)));
}

void PreviewWidget::setupMenus()
{
    this->contextMenuPreviewDropdown = new QMenu(this);
    this->contextMenuPreviewDropdown->setTitle("Dropdown");
    this->viewAlphaAction = this->contextMenuPreviewDropdown->addAction(/*QIcon(":/Graphics/Images/ViewAlpha    .png"),*/ "View Alpha");
    this->viewAlphaAction->setCheckable(true);
    this->contextMenuPreviewDropdown->addSeparator();
    this->expandCollapseAction = this->contextMenuPreviewDropdown->addAction(/*QIcon(":/Graphics/Images/Collapse.png"),*/ "Collapse", this, SLOT(toggleExpandCollapse()));

    QToolButton* toolButtonPreviewDropdown = new QToolButton(this);
    toolButtonPreviewDropdown->setObjectName("toolButtonPreviewDropdown");
    toolButtonPreviewDropdown->setMenu(this->contextMenuPreviewDropdown);
    toolButtonPreviewDropdown->setPopupMode(QToolButton::InstantPopup);
    this->tabWidgetPreview->setCornerWidget(toolButtonPreviewDropdown);
    //this->tabWidgetPreview->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));

    QObject::connect(this->viewAlphaAction, SIGNAL(toggled(bool)), this, SLOT(viewAlphaChanged(bool)));
}

void PreviewWidget::targetChanged(const TargetChangedEvent& event)
{
    this->model->setName(event.getTarget());

    setThumbnail();
}

void PreviewWidget::libraryItemSelected(const LibraryItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    setThumbnail();
}

void PreviewWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();

    setThumbnail();
}

void PreviewWidget::setThumbnail()
{
    if (this->model->getType() != "STILL" && this->model->getType() != "MOVIE")
    {
        this->labelPreview->setPixmap(NULL);
        return;
    }

    int thumbnailId = this->model->getThumbnailId();
    QString name = this->model->getName();
    QString deviceName = this->model->getDeviceName();

    QString data = DatabaseManager::getInstance().getThumbnailByNameAndDeviceName(name, deviceName).getData();

    /*
    QString data = DatabaseManager::getInstance().getThumbnailById(thumbnailId).getData();
    if (data.isEmpty())
        data = DatabaseManager::getInstance().getThumbnailByNameAndDeviceName(name, deviceName).getData();
    */

    if (!data.isEmpty())
    {
        this->image.loadFromData(QByteArray::fromBase64(data.toAscii()), "PNG");

        if (this->viewAlpha)
            this->labelPreview->setPixmap(QPixmap::fromImage(this->image.alphaChannel()));
        else
            this->labelPreview->setPixmap(QPixmap::fromImage(this->image));
    }
    else
    {
        this->labelPreview->setPixmap(NULL);
    }
}

void PreviewWidget::viewAlphaChanged(bool enabled)
{
    if (enabled)
        this->labelPreview->setPixmap(QPixmap::fromImage(this->image.alphaChannel()));
    else
        this->labelPreview->setPixmap(QPixmap::fromImage(this->image));

    this->viewAlpha = enabled;
}

void PreviewWidget::toggleExpandCollapse()
{
    this->collapsed = !this->collapsed;

    this->expandCollapseAction->setText((this->collapsed == true) ? "Expand" : "Collapse");
    this->setFixedHeight((this->collapsed == true) ? Panel::COMPACT_PREVIEW_HEIGHT : Panel::DEFAULT_PREVIEW_HEIGHT);
}
