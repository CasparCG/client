#include "AudioLevelsWidget.h"
#include "AudioMeterWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"

#include <QtGui/QToolButton>

AudioLevelsWidget::AudioLevelsWidget(QWidget* parent)
    : QWidget(parent),
      collapsed(false)
{
    setupUi(this);
    setupMenus();

    for (int i = 1; i < 9; i++)
        this->findChild<AudioMeterWidget*>(QString("widgetAudioMeter%1").arg(i))->configureAudioMeter(i);
}

void AudioLevelsWidget::setupMenus()
{
    this->contextMenuAudioLevelsDropdown = new QMenu(this);
    this->contextMenuAudioLevelsDropdown->setTitle("Dropdown");
    this->expandCollapseAction = this->contextMenuAudioLevelsDropdown->addAction(/*QIcon(":/Graphics/Images/Collapse.png"),*/ "Collapse", this, SLOT(toggleExpandCollapse()));

    QToolButton* toolButtonAudioLevelsDropdown = new QToolButton(this);
    toolButtonAudioLevelsDropdown->setObjectName("toolButtonAudioLevelsDropdown");
    toolButtonAudioLevelsDropdown->setMenu(this->contextMenuAudioLevelsDropdown);
    toolButtonAudioLevelsDropdown->setPopupMode(QToolButton::InstantPopup);
    this->tabWidgetAudioLevels->setCornerWidget(toolButtonAudioLevelsDropdown);
    //this->tabWidgetPreview->setTabIcon(0, QIcon(":/Graphics/Images/TabSplitter.png"));
}

void AudioLevelsWidget::toggleExpandCollapse()
{
    this->collapsed = !this->collapsed;

    this->expandCollapseAction->setText((this->collapsed == true) ? "Expand" : "Collapse");
    this->setFixedHeight((this->collapsed == true) ? Panel::COMPACT_AUDIOLEVELS_HEIGHT : Panel::DEFAULT_AUDIOLEVELS_HEIGHT);
}
