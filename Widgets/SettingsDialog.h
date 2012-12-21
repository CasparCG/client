#pragma once

#include "Shared.h"
#include "ui_SettingsDialog.h"
#include "Global.h"

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT SettingsDialog : public QDialog, Ui::SettingsDialog
{
    Q_OBJECT

    public:
        explicit SettingsDialog(QWidget* parent = 0);

    private:
        QString stylesheet;

        void loadGpi();
        void loadDevices();
        void checkEmptyDeviceList();
        void updateGpi(
                int gpi, const QComboBox* voltage, const QComboBox* action);
        void updateGpo(
                int gpo, const QComboBox* voltage, const QSpinBox* pulseLength);
        void updateGpiDevice();

        Q_SLOT void removeDevice();
        Q_SLOT void showAddDeviceDialog();
        Q_SLOT void startFullscreenChanged(int);
        Q_SLOT void fontSizeChanged(int);
        Q_SLOT void autoSynchronizeChanged(int);
        Q_SLOT void synchronizeIntervalChanged(int);
        Q_SLOT void gpi1Changed();
        Q_SLOT void gpi2Changed();
        Q_SLOT void gpi3Changed();
        Q_SLOT void gpi4Changed();
        Q_SLOT void gpi5Changed();
        Q_SLOT void gpi6Changed();
        Q_SLOT void gpi7Changed();
        Q_SLOT void gpi8Changed();
        Q_SLOT void gpo1Changed();
        Q_SLOT void gpo2Changed();
        Q_SLOT void gpo3Changed();
        Q_SLOT void gpo4Changed();
        Q_SLOT void gpo5Changed();
        Q_SLOT void gpo6Changed();
        Q_SLOT void gpo7Changed();
        Q_SLOT void gpo8Changed();
        Q_SLOT void serialPortChanged();
        Q_SLOT void baudRateChanged(QString);

        Q_SIGNAL void gpiBindingChanged(int, Playout::PlayoutType::Type);
};
