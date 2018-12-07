#pragma once

#include "Shared.h"
#include "ui_AudioMeterWidget.h"

#include "OscSubscription.h"
#include "Events/Inspector/ChannelChangedEvent.h"
#include "Events/Inspector/DeviceChangedEvent.h"
#include "Events/Rundown/EmptyRundownEvent.h"
#include "Events/Rundown/RundownItemSelectedEvent.h"
#include "Models/LibraryModel.h"

#include <QtGui/QImage>

#include <QtWidgets/QWidget>

class WIDGETS_EXPORT AudioMeterWidget : public QWidget, Ui::AudioMeterWidget
{
    Q_OBJECT

    public:
        explicit AudioMeterWidget(QWidget* parent = 0);

        void configureAudioMeter(int channel);

    private:
        int channel;
        LibraryModel* model;
        AbstractCommand* command;

        OscSubscription* audioSubscription;

        double convertToLevel(int value);
        void configureOscSubscriptions();

        Q_SLOT void deviceChanged(const DeviceChangedEvent&);
        Q_SLOT void channelChanged(const ChannelChangedEvent&);
        Q_SLOT void emptyRundown(const EmptyRundownEvent&);
        Q_SLOT void rundownItemSelected(const RundownItemSelectedEvent&);
        Q_SLOT void audioSubscriptionReceived(const QString&, const QList<QVariant>&);
};
