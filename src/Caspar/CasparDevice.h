#pragma once

#include "Shared.h"

#include "AmcpDevice.h"
#include "Models/CasparData.h"
#include "Models/CasparMedia.h"
#include "Models/CasparTemplate.h"
#include "Models/CasparThumbnail.h"

class CASPAR_EXPORT CasparDevice : public AmcpDevice
{
    Q_OBJECT

    public:
        explicit CasparDevice(const QString& address, int port = 5250, QObject* parent = 0);

        const QString resolveIpAddress() const;

        void refreshData();
        void refreshMedia();
        void refreshTemplate();
        void refreshChannels();
        void refreshThumbnail();

        void refreshFlashVersion();
        void refreshServerVersion();
        void refreshTemplateHostVersion();

        void retrieveThumbnail(const QString& name);

        void sendCommand(const QString& command);

        void clearChannel(int channel);
        void clearMixerChannel(int channel);
        void clearVideolayer(int channel, int videolayer);
        void clearMixerVideolayer(int channel, int videolayer);

        void pause(int channel, int videolayer);
        void resume(int channel, int videolayer);
        void stop(int channel, int videolayer);
        void play(int channel, int videolayer);

        void print(int channel, const QString& output);

        void loadRouteChannel(int toChannel, int toVideolayer, int fromChannel);
        void playRouteChannel(int toChannel, int toVideolayer, int fromChannel);
        void loadRouteVideolayer(int toChannel, int toVideolayer, int fromChannel, int fromVideolayer);
        void playRouteVideolayer(int toChannel, int toVideolayer, int fromChannel, int fromVideolayer);

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

        void playHtml(int channel, int videolayer, const QString& url, const QString& transition, int duration, const QString& easing, const QString& direction, bool useAuto);
        void loadHtml(int channel, int videolayer, const QString& url, const QString& transition, int duration, const QString& easing, const QString& direction, bool freezeOnLoad, bool useAuto);

        void playMovie(int channel, int videolayer, const QString& name, const QString& transition, int duration, const QString& easing, const QString& direction, int seek, int length, bool loop, bool useAuto);
        void loadMovie(int channel, int videolayer, const QString& name, const QString& transition, int duration, const QString& easing, const QString& direction, int seek, int length, bool loop, bool freezeOnLoad, bool useAuto);

        void playAudio(int channel, int videolayer, const QString& name, const QString& transition, int duration, const QString& easing, const QString& direction, bool loop, bool useAuto);
        void loadAudio(int channel, int videolayer, const QString& name, const QString& transition, int duration, const QString& easing, const QString& direction, bool loop, bool useAuto);

        void playColor(int channel, int videolayer, const QString& color, const QString& transition, int duration, const QString& easing, const QString& direction, bool useAuto);
        void loadColor(int channel, int videolayer, const QString& color, const QString& transition, int duration, const QString& easing, const QString& direction, bool useAuto);

        void playStill(int channel, int videolayer, const QString& name, const QString& transition, int duration, const QString& easing, const QString& direction, bool useAuto);
        void loadStill(int channel, int videolayer, const QString& name, const QString& transition, int duration, const QString& easing, const QString& direction, bool useAuto);

        void startFileRecorder(int channel, const QString& filename, const QString& codec, const QString& preset, const QString& tune, bool withAlpha);
        void stopFileRecorder(int channel);

        void startStream(int channel, int port, int quality = 23, bool key = false, int width = 0, int height = 0);
        void stopStream(int channel, int port);

        void playDeviceInput(int channel, int videolayer, int device, const QString& format);
        void loadDeviceInput(int channel, int videolayer, int device, const QString& format);

        void playImageScroll(int channel, int videolayer, const QString& name, int blur, int speed, bool premultiply, bool progressive);
        void loadImageScroll(int channel, int videolayer, const QString& name, int blur, int speed, bool premultiply, bool progressive);

        void setCommit(int channel);
        void setReset(int channel, int videolayer);
        void setChroma(int channel, int videolayer, const QString& key, float threshold, float spread, float spill, float blur, bool mask);
        void setBlendMode(int channel, int videolayer, const QString& blendMode);
        void setGrid(int channel, int grid, int duration, const QString& easing, bool defer = false);
        void setKeyer(int channel, int videolayer, int keyer, bool defer = false);
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
        void setFill(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, bool defer = false, bool useMipmap = false);
        void setFill(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, int duration, const QString& easing, bool defer = false, bool useMipmap = false);
        void setClipping(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, bool defer = false);
        void setClipping(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, int duration, const QString& easing, bool defer = false);
        void setPerspective(int channel, int videolayer, float upperLeftX, float upperLeftY, float upperRightX, float upperRightY, float lowerRightX, float lowerRightY, float lowerLeftX, float lowerLeftY, bool defer = false, bool useMipmap = false);
        void setPerspective(int channel, int videolayer, float upperLeftX, float upperLeftY, float upperRightX, float upperRightY, float lowerRightX, float lowerRightY, float lowerLeftX, float lowerLeftY, int duration, const QString& easing, bool defer = false, bool useMipmap = false);
        void setRotation(int channel, int videolayer, float rotation, bool defer = false);
        void setRotation(int channel, int videolayer, float rotation, int duration, const QString& easing, bool defer = false);
        void setAnchor(int channel, int videolayer, float positionX, float positionY, bool defer = false);
        void setAnchor(int channel, int videolayer, float positionX, float positionY, int duration, const QString& easing, bool defer = false);
        void setCrop(int channel, int videolayer, float upperLeftX, float upperLeftY, float lowerRightX, float lowerRightY, bool defer = false);
        void setCrop(int channel, int videolayer, float upperLeftX, float upperLeftY, float lowerRightX, float lowerRightY, int duration, const QString& easing, bool defer = false);
        void setMasterVolume(int channel, float masterVolume);

        Q_SIGNAL void connectionStateChanged(CasparDevice&);
        Q_SIGNAL void infoChanged(const QList<QString>&, CasparDevice&);
        Q_SIGNAL void infoSystemChanged(const QList<QString>&, CasparDevice&);
        Q_SIGNAL void mediaChanged(const QList<CasparMedia>&, CasparDevice&);
        Q_SIGNAL void mediaInfoChanged(const QList<QString>&, CasparDevice&);
        Q_SIGNAL void templateChanged(const QList<CasparTemplate>&, CasparDevice&);
        Q_SIGNAL void dataChanged(const QList<CasparData>&, CasparDevice&);
        Q_SIGNAL void versionChanged(const QString& version, CasparDevice&);
        Q_SIGNAL void responseChanged(const QString&, CasparDevice&);
        Q_SIGNAL void thumbnailChanged(const QList<CasparThumbnail>&, CasparDevice&);
        Q_SIGNAL void thumbnailRetrieveChanged(const QString& data, CasparDevice&);

    protected:
        void sendNotification();
};
