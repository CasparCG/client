#pragma once

#include "Shared.h"

#include "AMCPDevice.h"
#include "CasparData.h"
#include "CasparMedia.h"
#include "CasparTemplate.h"
#include "CasparVersion.h"

class CASPAR_EXPORT CasparDevice : public AMCPDevice
{
    Q_OBJECT

    public:
        explicit CasparDevice(QObject* parent = 0);

        void connect(const QString& address, int port = 5250);
        void disconnect();

        const int getPort() const;
        const QString getAddress() const;

        void refreshData();
        void refreshMedia();
        void refreshTemplate();
        void refreshVersion();

        void sendCommand(const QString& command);

        void clearChannel(int channel);
        void clearMixerChannel(int channel);
        void clearVideolayer(int channel, int videolayer);
        void clearMixerVideolayer(int channel, int videolayer);

        void addTemplate(int channel, int videolayer, int flashlayer, const QString& name, bool playOnLoad);
        void addTemplate(int channel, int videolayer, int flashlayer, const QString& name, bool playOnLoad, const QString& data);
        void invokeTemplate(int channel, int videolayer, int flashlayer, const QString& label);
        void nextTemplate(int channel, int videolayer, int flashlayer);
        void playTemplate(int channel, int videolayer, int flashlayer);
        void playTemplate(int channel, int videolayer, int flashlayer, const QString& name);
        void playTemplate(int channel, int videolayer, int flashlayer, const QString& name, const QString& data);
        void removeTemplate(int channel, int videolayer, int flashlayer);
        void stopTemplate(int channel, int videolayer, int flashlayer);
        void updateTemplate(int channel, int videolayer, int flashlayer, const QString& data);
        void pauseMedia(int channel, int videolayer);
        void playMedia(int channel, int videolayer);
        void playMedia(int channel, int videolayer, const QString& item);
        void playMedia(int channel, int videolayer, const QString& item, bool loop);
        void playMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& direction);
        void playMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& direction, bool loop);
        void playMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& easing, const QString& direction);
        void playMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& easing, const QString& direction, bool loop);
        void playMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& easing, const QString& direction, int seek, int length, bool loop);
        void loadMedia(int channel, int videolayer, const QString& item);
        void loadMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& easing, const QString& direction, bool loop);
        void loadMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& easing, const QString& direction, int seek, int length, bool loop);
        void stopMedia(int channel, int videolayer);

        void startRecording(int channel, const QString& filename, const QString& codec);
        void stopRecording(int channel);

        void playDeviceInput(int channel, int videolayer);
        void playDeviceInput(int channel, int videolayer, int device, const QString& format);
        void loadDeviceInput(int channel, int videolayer, int device, const QString& format);
        void stopDeviceInput(int channel, int videolayer);

        void playImageScroll(int channel, int videolayer);
        void playImageScroll(int channel, int videolayer, const QString& media, int blur, int speed, bool premultiply, bool progressive);
        void loadImageScroll(int channel, int videolayer, const QString& media, int blur, int speed, bool premultiply, bool progressive);
        void stopImageScroll(int channel, int videolayer);

        void setCommit(int channel);
        void setBlend(int channel, int videolayer, const QString& blend);
        void setGrid(int channel, int grid, int duration, const QString& easing, bool defer = false);
        void setKeyer(int channel, int videolayer, int keyer);
        void setVolume(int channel, int videolayer, float volume, bool defer = false);
        void setVolume(int channel, int videolayer, float volume, int duration, const QString& easing, bool defer = false);
        void setOpacity(int channel, int videolayer, float opacity, bool defer = false);
        void setOpacity(int channel, int videolayer, float opacity, int duration, const QString& easing, bool defer = false);
        void setBrightness(int channel, int videolayer, float brightness, bool defer = false);
        void setBrightness(int channel, int videolayer, float brightness, int duration, const QString& easing, bool defer = false);
        void setContrast(int channel, int videolayer, float contrast, bool defer = false);
        void setContrast(int channel, int videolayer, float contrast, int duration, const QString& easing, bool defer = false);
        void setSaturation(int channel, int videolayer, float saturation, bool defer = false);
        void setSaturation(int channel, int videolayer, float saturation, int duration, const QString& easing, bool defer = false);
        void setLevels(int channel, int videolayer, float minIn, float maxIn, float gamma, float minOut, float maxOut, bool defer = false);
        void setLevels(int channel, int videolayer, float minIn, float maxIn, float gamma, float minOut, float maxOut, int duration, const QString& easing, bool defer = false);
        void setGeometry(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, bool defer = false);
        void setGeometry(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, int duration, const QString& easing, bool defer = false);
        void setClipping(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, bool defer = false);
        void setClipping(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, int duration, const QString& easing, bool defer = false);

        Q_SIGNAL void connectionStateChanged(CasparDevice&);
        Q_SIGNAL void infoChanged(const QList<QString>&, CasparDevice&);
        Q_SIGNAL void infoSystemChanged(const QList<QString>&, CasparDevice&);
        Q_SIGNAL void mediaChanged(const QList<CasparMedia>&, CasparDevice&);
        Q_SIGNAL void mediaInfoChanged(const QList<QString>&, CasparDevice&);
        Q_SIGNAL void templateChanged(const QList<CasparTemplate>&, CasparDevice&);
        Q_SIGNAL void dataChanged(const QList<CasparData>&, CasparDevice&);
        Q_SIGNAL void versionChanged(const CasparVersion&, CasparDevice&);
        Q_SIGNAL void responseChanged(const QList<QString>&, CasparDevice&);

    protected:
        void sendNotification();
};
