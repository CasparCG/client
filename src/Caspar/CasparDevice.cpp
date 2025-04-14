#include "CasparDevice.h"

#include "Timecode.h"

#include "../Core/DatabaseManager.h"

#include <QtCore/QStringList>

#include <QtNetwork/QHostInfo>

CasparDevice::CasparDevice(const QString& address, int port, QObject* parent)
    : AmcpDevice(address, port, parent)
{
}

const QString CasparDevice::resolveIpAddress() const
{
    if (AmcpDevice::getAddress() == "localhost")
        return "127.0.0.1";

    QHostAddress address(AmcpDevice::getAddress());
    if (!address.isNull())
        return getAddress(); // The ip address is valid.

    // We don't have a valid ip address. Try to resolve using dns lookup.
    QHostInfo hostInfo = QHostInfo::fromName(AmcpDevice::getAddress());
    if (hostInfo.error() != QHostInfo::NoError)
        return "";

    if (hostInfo.addresses().isEmpty())
        return "";

    return hostInfo.addresses().at(0).toString();
}

void CasparDevice::refreshData()
{
    writeMessage("DATA LIST");
}

void CasparDevice::refreshFlashVersion()
{
    writeMessage("VERSION FLASH");
}

void CasparDevice::refreshServerVersion()
{
    writeMessage("VERSION SERVER");
}

void CasparDevice::refreshTemplateHostVersion()
{
    writeMessage("VERSION TEMPLATEHOST");
}

void CasparDevice::refreshMedia()
{
    writeMessage("CLS");
}

void CasparDevice::refreshTemplate()
{
    writeMessage("TLS");
}

void CasparDevice::refreshChannels()
{
    writeMessage("INFO");
}

void CasparDevice::refreshThumbnail()
{
    writeMessage("THUMBNAIL LIST");
}

void CasparDevice::retrieveThumbnail(const QString& name)
{
    writeMessage(QString("THUMBNAIL RETRIEVE \"%1\"").arg(name));
}

void CasparDevice::sendCommand(const QString& command)
{
    writeMessage(QString("%1").arg(command));
}

void CasparDevice::clearChannel(int channel)
{
    writeMessage(QString("CLEAR %1").arg(channel));
}

void CasparDevice::clearMixerChannel(int channel)
{
    writeMessage(QString("MIXER %1 CLEAR").arg(channel));
}

void CasparDevice::clearVideolayer(int channel, int videolayer)
{
    writeMessage(QString("CLEAR %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::clearMixerVideolayer(int channel, int videolayer)
{
    writeMessage(QString("MIXER %1-%2 CLEAR").arg(channel).arg(videolayer));
}

void CasparDevice::pause(int channel, int videolayer)
{
    writeMessage(QString("PAUSE %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::resume(int channel, int videolayer)
{
    writeMessage(QString("RESUME %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::stop(int channel, int videolayer)
{
    writeMessage(QString("STOP %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::play(int channel, int videolayer)
{
    writeMessage(QString("PLAY %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::print(int channel, const QString& output)
{
    writeMessage(QString("ADD %1 IMAGE \"%2\"").arg(channel).arg(output));
}

void CasparDevice::loadRouteChannel(int toChannel, int toVideolayer, int fromChannel)
{
    writeMessage(QString("LOADBG %1-%2 route://%3").arg(toChannel).arg(toVideolayer).arg(fromChannel));
}

void CasparDevice::loadRouteChannel(int toChannel, int toVideolayer, int fromChannel, int outputDelay)
{
    writeMessage(QString("LOADBG %1-%2 route://%3 FRAMES_DELAY %4").arg(toChannel).arg(toVideolayer).arg(fromChannel).arg(outputDelay));
}

void CasparDevice::playRouteChannel(int toChannel, int toVideolayer, int fromChannel)
{
    writeMessage(QString("PLAY %1-%2 route://%3").arg(toChannel).arg(toVideolayer).arg(fromChannel));
}

void CasparDevice::playRouteChannel(int toChannel, int toVideolayer, int fromChannel, int outputDelay)
{
    writeMessage(QString("PLAY %1-%2 route://%3 FRAMES_DELAY %4").arg(toChannel).arg(toVideolayer).arg(fromChannel).arg(outputDelay));
}

void CasparDevice::loadRouteVideolayer(int toChannel, int toVideolayer, int fromChannel, int fromVideolayer)
{
    writeMessage(QString("LOADBG %1-%2 route://%3-%4").arg(toChannel).arg(toVideolayer).arg(fromChannel).arg(fromVideolayer));
}

void CasparDevice::loadRouteVideolayer(int toChannel, int toVideolayer, int fromChannel, int fromVideolayer, int outputDelay)
{
    writeMessage(QString("LOADBG %1-%2 route://%3-%4 FRAMES_DELAY %5").arg(toChannel).arg(toVideolayer).arg(fromChannel).arg(fromVideolayer).arg(outputDelay));
}

void CasparDevice::playRouteVideolayer(int toChannel, int toVideolayer, int fromChannel, int fromVideolayer)
{
    writeMessage(QString("PLAY %1-%2 route://%3-%4").arg(toChannel).arg(toVideolayer).arg(fromChannel).arg(fromVideolayer));
}

void CasparDevice::playRouteVideolayer(int toChannel, int toVideolayer, int fromChannel, int fromVideolayer, int outputDelay)
{
    writeMessage(QString("PLAY %1-%2 route://%3-%4 FRAMES_DELAY %5").arg(toChannel).arg(toVideolayer).arg(fromChannel).arg(fromVideolayer).arg(outputDelay));
}

void CasparDevice::addTemplate(int channel, int videolayer, int flashlayer, const QString& name, bool playOnLoad)
{
    writeMessage(QString("CG %1-%2 ADD %3 \"%4\" %5")
                 .arg(channel).arg(videolayer).arg(flashlayer).arg(name)
                 .arg((playOnLoad == true) ? "1" : "0"));
}

void CasparDevice::addTemplate(int channel, int videolayer, int flashlayer, const QString& name, bool playOnLoad,
                               const QString& data)
{
    writeMessage(QString("CG %1-%2 ADD %3 %4 \"%5\" \"%6\"")
                 .arg(channel).arg(videolayer).arg(flashlayer).arg(name)
                 .arg((playOnLoad == true) ? "1" : "0").arg(data));
}

void CasparDevice::invokeTemplate(int channel, int videolayer, int flashlayer, const QString& label)
{
    QString value = label;
    value = value.replace("\\", "\\\\").replace("\"", "\\\""); // Escape possibly HTML params.

    writeMessage(QString("CG %1-%2 INVOKE %3 \"%4\"")
                 .arg(channel).arg(videolayer).arg(flashlayer).arg(value));
}

void CasparDevice::nextTemplate(int channel, int videolayer, int flashlayer)
{
    writeMessage(QString("CG %1-%2 NEXT %3")
                 .arg(channel).arg(videolayer).arg(flashlayer));
}

void CasparDevice::playTemplate(int channel, int videolayer, int flashlayer)
{
    writeMessage(QString("CG %1-%2 PLAY %3")
                 .arg(channel).arg(videolayer).arg(flashlayer));
}

void CasparDevice::playTemplate(int channel, int videolayer, int flashlayer, const QString& name)
{
    writeMessage(QString("CG %1-%2 ADD %3 \"%4\" 1")
                 .arg(channel).arg(videolayer).arg(flashlayer).arg(name));
}

void CasparDevice::playTemplate(int channel, int videolayer, int flashlayer, const QString& name, const QString& data)
{
    writeMessage(QString("CG %1-%2 ADD %3 \"%4\" 1 \"%5\"")
                 .arg(channel).arg(videolayer).arg(flashlayer).arg(name).arg(data));
}

void CasparDevice::removeTemplate(int channel, int videolayer, int flashlayer)
{
    writeMessage(QString("CG %1-%2 REMOVE %3")
                 .arg(channel).arg(videolayer).arg(flashlayer));
}

void CasparDevice::stopTemplate(int channel, int videolayer, int flashlayer)
{
    writeMessage(QString("CG %1-%2 STOP %3")
                 .arg(channel).arg(videolayer).arg(flashlayer));
}

void CasparDevice::updateTemplate(int channel, int videolayer, int flashlayer, const QString& data)
{
    writeMessage(QString("CG %1-%2 UPDATE %3 \"%4\"")
                 .arg(channel).arg(videolayer).arg(flashlayer).arg(data));
}

void CasparDevice::playHtml(int channel, int videolayer, const QString& url, const QString& transition, int duration,
                            const QString& easing, const QString& direction, bool useAuto)
{
    if (useAuto)
        loadHtml(channel, videolayer, url, transition, duration, easing, direction, false, useAuto);
    else
        writeMessage(QString("PLAY %1-%2 [HTML] \"%3\" %4 %5 %6 %7")
                     .arg(channel).arg(videolayer).arg(url).arg(transition).arg(duration).arg(easing)
                     .arg(direction));
}

void CasparDevice::loadHtml(int channel, int videolayer, const QString& url, const QString& transition, int duration,
                            const QString& easing, const QString& direction, bool freezeOnLoad, bool useAuto)
{
    writeMessage(QString("%1 %2-%3 [HTML] \"%4\" %5 %6 %7 %8 %9")
                 .arg((freezeOnLoad == true) ? "LOAD" : "LOADBG")
                 .arg(channel).arg(videolayer).arg(url).arg(transition).arg(duration).arg(easing)
                 .arg(direction)
                 .arg((useAuto == true) ? "AUTO" : ""));
}

void CasparDevice::playMovie(int channel, int videolayer, const QString& name, const QString& transition, int duration,
                             const QString& easing, const QString& direction, int seek, int length, bool loop, bool useAuto)
{
    if (useAuto)
        loadMovie(channel, videolayer, name, transition, duration, easing, direction, seek, length, loop, false, useAuto);
    else
        writeMessage(QString("PLAY %1-%2 \"%3\" %4 %5 %6 %7 %8 %9 %10")
                     .arg(channel).arg(videolayer).arg(name).arg(transition).arg(duration).arg(easing)
                     .arg(direction)
                     .arg((seek > 0) ? QString("SEEK %1").arg(seek) : "")
                     .arg((length > 0) ? QString("LENGTH %1").arg(length) : "")
                     .arg((loop == true) ? "LOOP" : ""));
}

void CasparDevice::loadMovie(int channel, int videolayer, const QString& name, const QString& transition, int duration,
                             const QString& easing, const QString& direction, int seek, int length, bool loop,
                             bool freezeOnLoad, bool useAuto)
{
    writeMessage(QString("%1 %2-%3 \"%4\" %5 %6 %7 %8 %9 %10 %11 %12")
                 .arg((freezeOnLoad == true) ? "LOAD" : "LOADBG")
                 .arg(channel).arg(videolayer).arg(name).arg(transition).arg(duration).arg(easing)
                 .arg(direction)
                 .arg((seek > 0) ? QString("SEEK %1").arg(seek) : "")
                 .arg((length > 0) ? QString("LENGTH %1").arg(length) : "")
                 .arg((loop == true) ? "LOOP" : "")
                 .arg((useAuto == true) ? "AUTO" : ""));
}

void CasparDevice::playAudio(int channel, int videolayer, const QString& name, const QString& transition, int duration,
                             const QString& easing, const QString& direction, bool loop, bool useAuto)
{
    if (useAuto)
        loadAudio(channel, videolayer, name, transition, duration, easing, direction, loop, useAuto);
    else
        writeMessage(QString("PLAY %1-%2 \"%3\" %4 %5 %6 %7 %8")
                     .arg(channel).arg(videolayer).arg(name).arg(transition).arg(duration).arg(easing)
                     .arg(direction)
                     .arg((loop == true) ? "LOOP" : ""));
}

void CasparDevice::loadAudio(int channel, int videolayer, const QString& name, const QString& transition, int duration,
                             const QString& easing, const QString& direction, bool loop, bool useAuto)
{
    writeMessage(QString("LOADBG %1-%2 \"%3\" %4 %5 %6 %7 %8 %9")
                 .arg(channel).arg(videolayer).arg(name).arg(transition).arg(duration).arg(easing)
                 .arg(direction)
                 .arg((loop == true) ? "LOOP" : "")
                 .arg((useAuto == true) ? "AUTO" : ""));
}

void CasparDevice::playColor(int channel, int videolayer, const QString& color, const QString &transition, int duration,
                             const QString& easing, const QString& direction, bool useAuto)
{
    if (useAuto)
        loadColor(channel, videolayer, color, transition, duration, easing, direction, useAuto);
    else
        writeMessage(QString("PLAY %1-%2 \"%3\" %4 %5 %6 %7")
                     .arg(channel).arg(videolayer).arg(color).arg(transition).arg(duration).arg(easing)
                     .arg(direction));
}

void CasparDevice::loadColor(int channel, int videolayer, const QString& color, const QString& transition, int duration,
                             const QString& easing, const QString& direction, bool useAuto)
{
    writeMessage(QString("LOADBG %1-%2 \"%3\" %4 %5 %6 %7 %8")
                 .arg(channel).arg(videolayer).arg(color).arg(transition).arg(duration).arg(easing)
                 .arg(direction)
                 .arg((useAuto == true) ? "AUTO" : ""));
}

void CasparDevice::playStill(int channel, int videolayer, const QString& name, const QString& transition, int duration,
                             const QString& easing, const QString& direction, bool useAuto)
{
    if (useAuto)
        loadStill(channel, videolayer, name, transition, duration, easing, direction, useAuto);
    else
        writeMessage(QString("PLAY %1-%2 \"%3\" %4 %5 %6 %7")
                     .arg(channel).arg(videolayer).arg(name).arg(transition).arg(duration).arg(easing)
                     .arg(direction));
}

void CasparDevice::loadStill(int channel, int videolayer, const QString& name, const QString& transition, int duration,
                             const QString& easing, const QString& direction, bool useAuto)
{
    writeMessage(QString("LOADBG %1-%2 \"%3\" %4 %5 %6 %7")
                 .arg(channel).arg(videolayer).arg(name).arg(transition).arg(duration).arg(easing)
                 .arg(direction)
                 .arg((useAuto == true) ? "AUTO" : ""));
}

void CasparDevice::startFileRecorder(int channel, const QString& filename, const QString& preset, bool withAlpha)
{
    writeMessage(QString("ADD %1-%2 FILE \"%3\" %4 %5")
                 .arg(channel)
                 .arg(QString("%1").arg(qChecksum(filename.toUtf8())))
                 .arg(filename)
                 .arg((preset != "") ? preset : "")
                 .arg((withAlpha == true) ? "SEPARATE_KEY" : ""));
}

void CasparDevice::stopFileRecorder(int channel, const QString& filename)
{
    writeMessage(QString("REMOVE %1-%2 FILE")
                 .arg(channel)
                 .arg(QString("%1").arg(qChecksum(filename.toUtf8()))));
}

void CasparDevice::startStream(int channel, int port, int quality, bool key, int width, int height)
{
    if (width > 0 && height > 0)
    {
        //writeMessage(QString("ADD %1 STREAM udp://<client_ip_address>:%2 -format mpegts -codec:v libx264 -crf:v %3 -tune:v zerolatency -preset:v ultrafast -filter:v scale=%4:%5%6")
          //           .arg(channel).arg(port).arg(quality).arg(width).arg(height).arg((key == true) ? ",alphaextract,format=pix_fmts=yuv422p" : ""));
        writeMessage(QString("ADD %1 STREAM udp://<client_ip_address>:%2 -format mpegts -codec:v libx264 -crf:v %3 -tune:v zerolatency -preset:v ultrafast -filter:v %4scale=%5:%6  -filter:a \"pan=stereo|c0=FL|c1=FR\"")
                             .arg(channel).arg(port).arg(quality)
                             .arg((key == true) ? "alphaextract,format=pix_fmts=yuv422p," : "")
                             .arg(width).arg(height));
    }
    else
    {
        writeMessage(QString("ADD %1 STREAM udp://<client_ip_address>:%2 -format mpegts -codec:v libx264 -crf:v %3 -tune:v zerolatency -preset:v ultrafast %4  -filter:a \"pan=stereo|c0=FL|c1=FR\"")
                     .arg(channel).arg(port).arg(quality).arg((key == true) ? "-filter:v alphaextract" : ""));
    }
}

void CasparDevice::stopStream(int channel, int port)
{
    writeMessage(QString("REMOVE %1 STREAM udp://<client_ip_address>:%2").arg(channel).arg(port));
}

void CasparDevice::playDeviceInput(int channel, int videolayer, int device, const QString& format)
{
    writeMessage(QString("PLAY %1-%2 DECKLINK DEVICE %3 FORMAT %4")
                 .arg(channel).arg(videolayer).arg(device).arg(format));
}

void CasparDevice::loadDeviceInput(int channel, int videolayer, int device, const QString& format)
{
    writeMessage(QString("LOADBG %1-%2 DECKLINK DEVICE %3 FORMAT %4")
                 .arg(channel).arg(videolayer).arg(device).arg(format));
}

void CasparDevice::playImageScroll(int channel, int videolayer, const QString& name, int blur, int speed,
                                   bool premultiply, bool progressive)
{
    writeMessage(QString("PLAY %1-%2 \"%3\" BLUR %4 SPEED %5 %6 %7")
                 .arg(channel).arg(videolayer).arg(name).arg(blur).arg(speed)
                 .arg((premultiply == true) ? "PREMULTIPLY" : "")
                 .arg((progressive == true) ? "PROGRESSIVE" : ""));
}

void CasparDevice::loadImageScroll(int channel, int videolayer, const QString& name, int blur, int speed,
                                   bool premultiply, bool progressive)
{
    writeMessage(QString("LOADBG %1-%2 \"%3\" BLUR %4 SPEED %5 %6 %7")
                 .arg(channel).arg(videolayer).arg(name).arg(blur).arg(speed)
                 .arg((premultiply == true) ? "PREMULTIPLY" : "")
                 .arg((progressive == true) ? "PROGRESSIVE" : ""));
}

void CasparDevice::setReset(int channel, int videolayer)
{
    writeMessage(QString("MIXER %1-%2 CLEAR").arg(channel).arg(videolayer));
}

void CasparDevice::setCommit(int channel)
{
    writeMessage(QString("MIXER %1 COMMIT").arg(channel));
}

void CasparDevice::setMasterVolume(int channel, float masterVolume)
{
    writeMessage(QString("MIXER %1 MASTERVOLUME %2").arg(channel).arg(masterVolume));
}

void CasparDevice::setChroma(int channel, int videolayer, const QString& key, float threshold, float spread, float spill)
{
    writeMessage(QString("MIXER %1-%2 CHROMA %3 %4 %5 %6")
                 .arg(channel).arg(videolayer).arg(key).arg(threshold).arg(spread).arg(spill));
}

void CasparDevice::setBlendMode(int channel, int videolayer, const QString& blendMode)
{
    writeMessage(QString("MIXER %1-%2 BLEND %3")
                 .arg(channel).arg(videolayer).arg(blendMode));
}

void CasparDevice::setGrid(int channel, int grid, int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1 GRID %2 %3 %4 %5")
                 .arg(channel).arg(grid).arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setKeyer(int channel, int videolayer, int keyer, bool defer)
{
    writeMessage(QString("MIXER %1-%2 KEYER %3 %4")
                 .arg(channel).arg(videolayer).arg(keyer)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setVolume(int channel, int videolayer, float volume, bool defer)
{
    writeMessage(QString("MIXER %1-%2 VOLUME %3 %4")
                 .arg(channel).arg(videolayer).arg(volume)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setVolume(int channel, int videolayer, float volume, int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 VOLUME %3 %4 %5 %6")
                 .arg(channel).arg(videolayer).arg(volume).arg(duration)
                 .arg(easing).arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setOpacity(int channel, int videolayer, float opacity, bool defer)
{
    writeMessage(QString("MIXER %1-%2 OPACITY %3 %4")
                 .arg(channel).arg(videolayer).arg(opacity)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setOpacity(int channel, int videolayer, float opacity, int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 OPACITY %3 %4 %5 %6")
                 .arg(channel).arg(videolayer).arg(opacity).arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setBrightness(int channel, int videolayer, float brightness, bool defer)
{
    writeMessage(QString("MIXER %1-%2 BRIGHTNESS %3 %4")
                 .arg(channel).arg(videolayer).arg(brightness)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setBrightness(int channel, int videolayer, float brightness, int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 BRIGHTNESS %3 %4 %5 %6")
                .arg(channel).arg(videolayer).arg(brightness).arg(duration).arg(easing)
                .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setContrast(int channel, int videolayer, float contrast, bool defer)
{
    writeMessage(QString("MIXER %1-%2 CONTRAST %3 %4")
                 .arg(channel).arg(videolayer).arg(contrast)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setContrast(int channel, int videolayer, float contrast, int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 CONTRAST %3 %4 %5 %6")
                 .arg(channel).arg(videolayer).arg(contrast).arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setSaturation(int channel, int videolayer, float saturation, bool defer)
{
    writeMessage(QString("MIXER %1-%2 SATURATION %3 %4")
                 .arg(channel).arg(videolayer).arg(saturation)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setSaturation(int channel, int videolayer, float saturation, int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 SATURATION %3 %4 %5 %6")
                 .arg(channel).arg(videolayer).arg(saturation).arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setLevels(int channel, int videolayer, float minIn, float maxIn, float gamma, float minOut, float maxOut,
                             bool defer)
{
    writeMessage(QString("MIXER %1-%2 LEVELS %3 %4 %5 %6 %7 %8")
                 .arg(channel).arg(videolayer).arg(minIn).arg(maxIn).arg(gamma).arg(minOut).arg(maxOut)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setLevels(int channel, int videolayer, float minIn, float maxIn, float gamma, float minOut, float maxOut,
                             int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 LEVELS %3 %4 %5 %6 %7 %8 %9 %10")
                 .arg(channel).arg(videolayer).arg(minIn).arg(maxIn).arg(gamma).arg(minOut).arg(maxOut)
                 .arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setFill(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY,
                           bool defer, bool useMipmap)
{
    writeMessage(QString("MIXER %1-%2 MIPMAP %3").arg(channel).arg(videolayer).arg((useMipmap == true) ? "1" : "0"));
    writeMessage(QString("MIXER %1-%2 FILL %3 %4 %5 %6 %7")
                 .arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setFill(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY,
                           int duration, const QString& easing, bool defer, bool useMipmap)
{
    writeMessage(QString("MIXER %1-%2 MIPMAP %3").arg(channel).arg(videolayer).arg((useMipmap == true) ? "1" : "0"));
    writeMessage(QString("MIXER %1-%2 FILL %3 %4 %5 %6 %7 %8 %9")
                 .arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY)
                 .arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setClipping(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY,
                               bool defer)
{
    writeMessage(QString("MIXER %1-%2 CLIP %3 %4 %5 %6 %7")
                 .arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX)
                 .arg(scaleY).arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setClipping(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY,
                               int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 CLIP %3 %4 %5 %6 %7 %8 %9")
                 .arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY)
                 .arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setCrop(int channel, int videolayer, float upperLeftX, float upperLeftY, float lowerRightX, float lowerRightY, bool defer)
{
    writeMessage(QString("MIXER %1-%2 CROP %3 %4 %5 %6 %7")
                 .arg(channel).arg(videolayer).arg(upperLeftX).arg(upperLeftY).arg(lowerRightX).arg(lowerRightY)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setCrop(int channel, int videolayer, float upperLeftX, float upperLeftY, float lowerRightX, float lowerRightY, int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 CROP %3 %4 %5 %6 %7 %8 %9")
                 .arg(channel).arg(videolayer).arg(upperLeftX).arg(upperLeftY).arg(lowerRightX).arg(lowerRightY)
                 .arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setPerspective(int channel, int videolayer, float upperLeftX, float upperLeftY, float upperRightX, float upperRightY,
                                  float lowerRightX, float lowerRightY, float lowerLeftX, float lowerLeftY, bool defer, bool useMipmap)
{
    writeMessage(QString("MIXER %1-%2 MIPMAP %3").arg(channel).arg(videolayer).arg((useMipmap == true) ? "1" : "0"));
    writeMessage(QString("MIXER %1-%2 PERSPECTIVE %3 %4 %5 %6 %7 %8 %9 %10 %11")
                 .arg(channel).arg(videolayer)
                 .arg(upperLeftX).arg(upperLeftY).arg(upperRightX).arg(upperRightY)
                 .arg(lowerRightX).arg(lowerRightY).arg(lowerLeftX).arg(lowerLeftY)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setPerspective(int channel, int videolayer, float upperLeftX, float upperLeftY, float upperRightX, float upperRightY,
                                  float lowerRightX, float lowerRightY, float lowerLeftX, float lowerLeftY, int duration, const QString& easing, bool defer, bool useMipmap)
{
    writeMessage(QString("MIXER %1-%2 MIPMAP %3").arg(channel).arg(videolayer).arg((useMipmap == true) ? "1" : "0"));
    writeMessage(QString("MIXER %1-%2 PERSPECTIVE %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13")
                 .arg(channel).arg(videolayer)
                 .arg(upperLeftX).arg(upperLeftY).arg(upperRightX).arg(upperRightY)
                 .arg(lowerRightX).arg(lowerRightY).arg(lowerLeftX).arg(lowerLeftY)
                 .arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setRotation(int channel, int videolayer, float rotation, bool defer)
{
    writeMessage(QString("MIXER %1-%2 ROTATION %3 %4")
                 .arg(channel).arg(videolayer).arg(rotation)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setRotation(int channel, int videolayer, float rotation, int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 ROTATION %3 %4 %5 %6")
                .arg(channel).arg(videolayer).arg(rotation).arg(duration).arg(easing)
                .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setAnchor(int channel, int videolayer, float positionX, float positionY, bool defer)
{
    writeMessage(QString("MIXER %1-%2 ANCHOR %3 %4 %5")
                 .arg(channel).arg(videolayer).arg(positionX).arg(positionY)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::setAnchor(int channel, int videolayer, float positionX, float positionY, int duration, const QString& easing, bool defer)
{
    writeMessage(QString("MIXER %1-%2 ANCHOR %3 %4 %5 %6 %7")
                 .arg(channel).arg(videolayer).arg(positionX).arg(positionY)
                 .arg(duration).arg(easing)
                 .arg((defer == true) ? "DEFER" : ""));
}

void CasparDevice::sendNotification()
{
    if (AmcpDevice::response.count() > 0)
        qDebug("Received message from %s:%d: %s\\r\\n", qPrintable(AmcpDevice::getAddress()), AmcpDevice::getPort(), qPrintable(AmcpDevice::response.at(0).trimmed()));

    switch (AmcpDevice::command)
    {
        case AmcpDevice::AmcpDeviceCommand::CLS:
        {
            emit responseChanged(AmcpDevice::response.at(0), *this);

            AmcpDevice::response.removeFirst(); // First post is the header, 200 CLS OK.

            QList<CasparMedia> items;
            foreach (QString response, AmcpDevice::response)
            {
                QString name = response.split("\" ").at(0);
                name.replace("\\", "/");
                if (name.startsWith("\""))
                    name.remove(0, 1);

                if (name.endsWith("\""))
                    name.remove(name.length() - 1, 1);

                QString type = response.split("\" ").at(1).trimmed().split(" ").at(0);

                QString timecode;
                if (response.split("\" ").at(1).trimmed().split(" ").count() > 5)
                {
                    // Format:
                    // "AMB"  MOVIE  6445960 20121101160514 643 1/60
                    // "CG1080I50"  MOVIE  6159792 20121101150514 264 1/25
                    // "GO1080P25"  MOVIE  16694084 20121101150514 445 1/25
                    // "WIPE"  MOVIE  1268784 20121101150514 31 1/25
                    // "HOOLOOVOO"  MOVIE  1111111 22222222222222 333 100/2997
                    QString totalFrames = response.split("\" ").at(1).trimmed().split(" ").at(4);
                    QStringList timebase = response.split("\" ").at(1).trimmed().split(" ").at(5).split("/");

                    int frames = totalFrames.toInt();
                    double fps = timebase.at(1).toDouble() / timebase.at(0).toDouble();

                    double time = frames * (1.0 / fps);
                    timecode = Timecode::fromTime(time, fps, false);
                }

                items.push_back(CasparMedia(name, type, timecode));
            }

            emit mediaChanged(items, *this);

            break;
        }
        case AmcpDevice::AmcpDeviceCommand::TLS:
        {
            emit responseChanged(AmcpDevice::response.at(0), *this);

            AmcpDevice::response.removeFirst(); // First post is the header, 200 TLS OK.

            QList<CasparTemplate> items;
            foreach (QString response, AmcpDevice::response)
            {
                QString name = response.split("\" ").at(0);
                name.replace("\\", "/");
                if (name.startsWith("\""))
                    name.remove(0, 1);

                if (name.endsWith("\""))
                    name.remove(name.length() - 1, 1);

                items.push_back(CasparTemplate(name));
            }

            emit templateChanged(items, *this);

            break;
        }
        case AmcpDevice::AmcpDeviceCommand::INFO:
        {
            AmcpDevice::response.removeFirst(); // First post is the header, 200 INFO OK.
            emit infoChanged(AmcpDevice::response, *this);

            break;
        }
        case AmcpDevice::AmcpDeviceCommand::INFOSYSTEM:
        {
            AmcpDevice::response.removeFirst(); // First post is the header, 201 INFO SYSTEM OK.

            emit infoSystemChanged(AmcpDevice::response, *this);

            break;
        }
        case AmcpDevice::AmcpDeviceCommand::DATALIST:
        {
            emit responseChanged(AmcpDevice::response.at(0), *this);

            AmcpDevice::response.removeFirst(); // First post is the header, 200 DATA LIST OK.

            QList<CasparData> items;
            foreach (QString response, AmcpDevice::response)
            {
                QString name = response.split("\" ").at(0);
                name.replace("\\", "/");
                if (name.startsWith("\""))
                    name.remove(0, 1);

                if (name.endsWith("\""))
                    name.remove(name.length() - 1, 1);

                items.push_back(CasparData(name));
            }

            emit dataChanged(items, *this);

            break;
        }
        case AmcpDevice::AmcpDeviceCommand::THUMBNAILLIST:
        {
            emit responseChanged(AmcpDevice::response.at(0), *this);

            AmcpDevice::response.removeFirst(); // First post is the header, 200 THUMBNAIL LIST OK.

            QList<CasparThumbnail> items;
            foreach (QString response, AmcpDevice::response)
            {
                QString name = response.split("\" ").at(0);
                name.replace("\\", "/");
                if (name.startsWith("\""))
                    name.remove(0, 1);

                if (name.endsWith("\""))
                    name.remove(name.length() - 1, 1);

                QString timestamp = response.split("\" ").at(1).trimmed().split(" ").at(0);
                QString size = response.split("\" ").at(1).trimmed().split(" ").at(1);

                items.push_back(CasparThumbnail(name, timestamp, size));
            }

            emit thumbnailChanged(items, *this);

            break;
        }
        case AmcpDevice::AmcpDeviceCommand::THUMBNAILRETRIEVE:
        {
            AmcpDevice::response.removeFirst(); // First post is the header, 200 THUMBNAIL RETRIEVE OK.

            emit thumbnailRetrieveChanged(AmcpDevice::response.at(0), *this);

            break;
        }
        case AmcpDevice::AmcpDeviceCommand::VERSION:
        {
            AmcpDevice::response.removeFirst(); // First post is the header, 200 VERSION OK.

            emit versionChanged(AmcpDevice::response.at(0), *this);

            break;
        }
        case AmcpDevice::AmcpDeviceCommand::CONNECTIONSTATE:
        {
            emit connectionStateChanged(*this);

            break;
        }
        default:
        {
            emit responseChanged(AmcpDevice::response.at(0), *this);

            break;
        }
    }

    resetDevice();
}
