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

    QObject::connect(&EventManager::getInstance(), SIGNAL(rundownItemSelected(const RundownItemSelectedEvent&)), this, SLOT(rundownItemSelected(const RundownItemSelectedEvent&)));
}

void AudioMeterWidget::configureAudioMeter(int channel)
{
    this->channel = channel;
    this->labelAudioMeterChannel->setText(QString("Ch %1").arg(channel));
}

void AudioMeterWidget::rundownItemSelected(const RundownItemSelectedEvent& event)
{
    this->model = event.getLibraryModel();
    this->command = dynamic_cast<AbstractCommand*>(event.getCommand());

    configureOscSubscriptions();
}

void AudioMeterWidget::configureOscSubscriptions()
{
    if (DeviceManager::getInstance().getDeviceByName(this->model->getDeviceName()) == NULL)
        return;

    if (this->audioSubscription != NULL)
        this->audioSubscription->disconnect(); // Disconnect all events.

    QString audioFilter = Osc::DEFAULT_AUDIO_FILTER;
    audioFilter.replace("#IPADDRESS#", QString("%1").arg(DeviceManager::getInstance().getDeviceByName(this->model->getDeviceName())->resolveIpAddress()))
               .replace("#CHANNEL#", QString("%1").arg(this->command->getChannel()))
               .replace("#AUDIOCHANNEL#", QString("%1").arg(this->channel));
    this->audioSubscription = new OscSubscription(audioFilter, this);
    QObject::connect(this->audioSubscription, SIGNAL(subscriptionReceived(const QString&, const QList<QVariant>&)),
                     this, SLOT(audioSubscriptionReceived(const QString&, const QList<QVariant>&)));
}

void AudioMeterWidget::audioSubscriptionReceived(const QString& predicate, const QList<QVariant>& arguments)
{
    //qDebug() << predicate << " " << arguments;

    int value = arguments.at(0).toInt();
    if (value < -60)
        this->progressBarAudioMeter->setValue(-61);
    else
        this->progressBarAudioMeter->setValue(arguments.at(0).toInt());
}
