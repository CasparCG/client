#include "LiveDialog.h"

#include "EventManager.h"

LiveDialog::LiveDialog(QWidget* parent)
    : QDialog(parent, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint),
      applicationFullscreen(false)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(toggleFullscreen(const ToggleFullscreenEvent&)), this, SLOT(toggleFullscreen(const ToggleFullscreenEvent&)));

}

void LiveDialog::toggleFullscreen(const ToggleFullscreenEvent& event)
{
    this->applicationFullscreen = !this->applicationFullscreen;

    (!isHidden() && isFullScreen()) ? setWindowState(Qt::WindowNoState) : setWindowState(Qt::WindowFullScreen);
}

void LiveDialog::visible()
{
    (this->applicationFullscreen == true) ? setWindowState(Qt::WindowFullScreen) : setWindowState(Qt::WindowNoState);

    show();
}

QWidget* LiveDialog::getRenderTarget()
{
    return this->labelLive;
}
