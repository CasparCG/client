#include "AudioMeterWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "DeviceManager.h"
#include "EventManager.h"

AudioMeterWidget::AudioMeterWidget(QWidget* parent)
    : QWidget(parent),
      channel(-1), model(NULL), command(NULL), audioSubscription(NULL)
{
    setupUi(this);

    QObject::connect(&EventManager::getInstance(), SIGNAL(deviceChanged(const DeviceChangedEvent&)), this, SLOT(deviceChanged(const DeviceChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(channelChanged(const ChannelChangedEvent&)), this, SLOT(channelChanged(const ChannelChangedEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(emptyRundown(const EmptyRundownEvent&)), this, SLOT(emptyRundown(const EmptyRundownEvent&)));
    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void AudioMeterWidget::configureAudioMeter(int channel)
{
    this->channel = channel;
    this->labelAudioMeterChannel->setText(QString("Ch %1").arg(channel));
}

void AudioMeterWidget::deviceChanged(const DeviceChangedEvent& event)
{
    if (this->model == NULL)
        return;

    if (!event.getDeviceName().isEmpty() && event.getDeviceName() != this->model->getDeviceName())
    {
        if (this->audioSubscription != NULL)
            this->audioSubscription->disconnect(); // Disconnect all events.

        if (DeviceManager::getInstance().getDeviceByName(event.getDeviceName()) == NULL)
            return;

        this->progressBarAudioMeter->setValue(-61);
        this->progressBarAudioMeter->update();

        QString audioFilter = Osc::AUDIOCHANNEL_FILTER;
        audioFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(event.getDeviceName())->resolveIpAddress()))
                   .replace("#CHANNEL#", QString("%1").arg(this->command->getChannel()));
        this->audioSubscription = new OscSubscription(audioFilter, this);
        QObject::connect(this->audioSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                         this, SLOT(audioSubscriptionReceived(const QString&, const QList<QVariant>&)));
    }
}

void AudioMeterWidget::channelChanged(const ChannelChangedEvent& event)
{
    Q_UNUSED(event);

    configureOscSubscriptions();
}

void AudioMeterWidget::emptyRundown(const EmptyRundownEvent& event)
{
    Q_UNUSED(event);

    if (this->audioSubscription != NULL)
        this->audioSubscription->disconnect(); // Disconnect all events.

    this->model = NULL;

    this->progressBarAudioMeter->setValue(-61);
    this->progressBarAudioMeter->update();
}

void AudioMeterWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();
    this->command = dynamic_cast<AbstractCommand*>(event.getCommand());

    configureOscSubscriptions();
}

void AudioMeterWidget::configureOscSubscriptions()
{
    if (this->audioSubscription != NULL)
        this->audioSubscription->disconnect(); // Disconnect all events.

    if (this->model == NULL)
        return;

    if (DeviceManager::getInstance().getDeviceByName(this->model->getDeviceName()) == NULL)
        return;

    QString audioFilter = Osc::AUDIOCHANNEL_FILTER;
    audioFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model->getDeviceName())->resolveIpAddress()))
               .replace("#CHANNEL#", QString("%1").arg(this->command->getChannel()));
    this->audioSubscription = new OscSubscription(audioFilter, this);
    QObject::connect(this->audioSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(audioSubscriptionReceived(const QString&, const QList<QVariant>&)));
}

void AudioMeterWidget::audioSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    Q_UNUSED(predicate);

    int value = convertToLevel(arguments.at(this->channel - 1).toInt());
    if (value < -60)
        this->progressBarAudioMeter->setValue(-61);
    else
    {
        this->progressBarAudioMeter->setValue(value);
        this->progressBarAudioMeter->update();
    }
}

double AudioMeterWidget::convertToLevel(int value)
{
    auto MIN_PFS = 0.5 / static_cast<double>(std::numeric_limits<int32_t>::max());
    auto pFS = value / static_cast<double>(std::numeric_limits<int32_t>::max());

    return 20.0 * std::log10((std::max(MIN_PFS, pFS)));
}
