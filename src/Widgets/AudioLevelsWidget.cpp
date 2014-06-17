#include "AudioLevelsWidget.h"
#include "AudioMeterWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "EventManager.h"

AudioLevelsWidget::AudioLevelsWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi(this);

    for (int i = 1; i < 9; i++)
        this->findChild<AudioMeterWidget*>(QString("widgetAudioMeter%1").arg(i))->configureAudioMeter(i);
}
