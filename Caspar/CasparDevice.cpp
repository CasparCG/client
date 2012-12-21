#include "CasparDevice.h"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>

CasparDevice::CasparDevice(QObject* parent)
    : AMCPDevice(parent)
{
}

void CasparDevice::connect(const QString& address, int port)
{
    AMCPDevice::connectDevice(address, port);
}

void CasparDevice::disconnect()
{
    AMCPDevice::disconnectDevice();
}

const int CasparDevice::getPort() const
{
    return AMCPDevice::getPort();
}

const QString CasparDevice::getAddress() const
{
    return AMCPDevice::getAddress();
}

void CasparDevice::refreshData()
{
    AMCPDevice::writeMessage("DATA LIST");
}

void CasparDevice::refreshVersion()
{
    AMCPDevice::writeMessage("VERSION");
}

void CasparDevice::refreshMedia()
{
    AMCPDevice::writeMessage("CLS");
}

void CasparDevice::refreshTemplate()
{
    AMCPDevice::writeMessage("TLS");
}

void CasparDevice::sendCommand(const QString& command)
{
    AMCPDevice::writeMessage(QString("%1").arg(command));
}

void CasparDevice::clearChannel(int channel)
{
    AMCPDevice::writeMessage(QString("CLEAR %1").arg(channel));
}

void CasparDevice::clearMixerChannel(int channel)
{
    AMCPDevice::writeMessage(QString("MIXER %1 CLEAR").arg(channel));
}

void CasparDevice::clearVideolayer(int channel, int videolayer)
{
    AMCPDevice::writeMessage(QString("CLEAR %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::clearMixerVideolayer(int channel, int videolayer)
{
    AMCPDevice::writeMessage(QString("MIXER %1-%2 CLEAR").arg(channel).arg(videolayer));
}

void CasparDevice::addTemplate(int channel, int videolayer, int flashlayer, const QString& name, bool playOnLoad)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 ADD %3 \"%4\" %5").arg(channel).arg(videolayer).arg(flashlayer).arg(name).arg((playOnLoad == true) ? "1" : "0"));
}

void CasparDevice::addTemplate(int channel, int videolayer, int flashlayer, const QString& name, bool playOnLoad, const QString& data)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 ADD %3 %4 \"%5\" \"%6\"").arg(channel).arg(videolayer).arg(flashlayer).arg(name).arg((playOnLoad == true) ? "1" : "0").arg(data));
}

void CasparDevice::invokeTemplate(int channel, int videolayer, int flashlayer, const QString& label)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 INVOKE %3 %4").arg(channel).arg(videolayer).arg(flashlayer).arg(label));
}

void CasparDevice::nextTemplate(int channel, int videolayer, int flashlayer)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 NEXT %3").arg(channel).arg(videolayer).arg(flashlayer));
}

void CasparDevice::playTemplate(int channel, int videolayer, int flashlayer)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 PLAY %3").arg(channel).arg(videolayer).arg(flashlayer));
}

void CasparDevice::playTemplate(int channel, int videolayer, int flashlayer, const QString& name)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 ADD %3 \"%4\" 1").arg(channel).arg(videolayer).arg(flashlayer).arg(name));
}

void CasparDevice::playTemplate(int channel, int videolayer, int flashlayer, const QString& name, const QString& data)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 ADD %3 \"%4\" 1 \"%5\"").arg(channel).arg(videolayer).arg(flashlayer).arg(name).arg(data));
}

void CasparDevice::removeTemplate(int channel, int videolayer, int flashlayer)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 REMOVE %3").arg(channel).arg(videolayer).arg(flashlayer));
}

void CasparDevice::stopTemplate(int channel, int videolayer, int flashlayer)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 STOP %3").arg(channel).arg(videolayer).arg(flashlayer));
}

void CasparDevice::updateTemplate(int channel, int videolayer, int flashlayer, const QString &data)
{
    AMCPDevice::writeMessage(QString("CG %1-%2 UPDATE %3 \"%4\"").arg(channel).arg(videolayer).arg(flashlayer).arg(data));
}

void CasparDevice::pauseMedia(int channel, int videolayer)
{
    AMCPDevice::writeMessage(QString("PAUSE %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::playMedia(int channel, int videolayer)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::playMedia(int channel, int videolayer, const QString& item)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2 \"%3\"").arg(channel).arg(videolayer).arg(item));
}

void CasparDevice::playMedia(int channel, int videolayer, const QString& item, bool loop)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2 \"%3\" %4").arg(channel).arg(videolayer).arg(item).arg((loop == true) ? "LOOP" : ""));
}

void CasparDevice::playMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& direction)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2 \"%3\" %4 %5 %6").arg(channel).arg(videolayer).arg(item).arg(transition).arg(duration).arg(direction));
}

void CasparDevice::playMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& direction, bool loop)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2 \"%3\" %4 %5 %6 %7").arg(channel).arg(videolayer).arg(item).arg(transition).arg(duration).arg(direction).arg((loop == true) ? "LOOP" : ""));
}

void CasparDevice::playMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& easing, const QString& direction)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2 \"%3\" %4 %5 %6 %7").arg(channel).arg(videolayer).arg(item).arg(transition).arg(duration).arg(easing).arg(direction));
}

void CasparDevice::playMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& easing, const QString& direction, bool loop)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2 \"%3\" %4 %5 %6 %7 %8").arg(channel).arg(videolayer).arg(item).arg(transition).arg(duration).arg(easing).arg(direction).arg((loop == true) ? "LOOP" : ""));
}

void CasparDevice::playMedia(int channel, int videolayer, const QString &item, const QString &transition, int duration, const QString& easing, const QString& direction, int seek, int length, bool loop)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2 \"%3\" %4 %5 %6 %7 SEEK %8 LENGTH %9 %10").arg(channel).arg(videolayer).arg(item).arg(transition).arg(duration).arg(easing).arg(direction).arg(seek).arg(length).arg((loop == true) ? "LOOP" : ""));
}

void CasparDevice::loadMedia(int channel, int videolayer, const QString& item)
{
    AMCPDevice::writeMessage(QString("LOADBG %1-%2 \"%3\"").arg(channel).arg(videolayer).arg(item));
}

void CasparDevice::loadMedia(int channel, int videolayer, const QString &item, const QString &transition, int duration, const QString& easing, const QString& direction, bool loop)
{
    AMCPDevice::writeMessage(QString("LOADBG %1-%2 \"%3\" %4 %5 %6 %7 %8").arg(channel).arg(videolayer).arg(item).arg(transition).arg(duration).arg(easing).arg(direction).arg((loop == true) ? "LOOP" : ""));
}

void CasparDevice::loadMedia(int channel, int videolayer, const QString& item, const QString& transition, int duration, const QString& easing, const QString& direction, int seek, int length, bool loop)
{
    AMCPDevice::writeMessage(QString("LOADBG %1-%2 \"%3\" %4 %5 %6 %7 SEEK %8 LENGTH %9 %10").arg(channel).arg(videolayer).arg(item).arg(transition).arg(duration).arg(easing).arg(direction).arg(seek).arg(length).arg((loop == true) ? "LOOP" : ""));
}

void CasparDevice::stopMedia(int channel, int videolayer)
{
    AMCPDevice::writeMessage(QString("STOP %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::startRecording(int channel, const QString& filename, const QString& codec)
{
    AMCPDevice::writeMessage(QString("ADD %1 FILE %2 -vcodec %3").arg(channel).arg(filename).arg(codec));
}

void CasparDevice::stopRecording(int channel)
{
    AMCPDevice::writeMessage(QString("REMOVE %1 FILE").arg(channel));
}

void CasparDevice::playDeviceInput(int channel, int videolayer)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::playDeviceInput(int channel, int videolayer, int device, const QString& format)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2 DECKLINK DEVICE %3 FORMAT %4").arg(channel).arg(videolayer).arg(device).arg(format));
}

void CasparDevice::loadDeviceInput(int channel, int videolayer, int device, const QString& format)
{
    AMCPDevice::writeMessage(QString("LOADBG %1-%2 DECKLINK DEVICE %3 FORMAT %4").arg(channel).arg(videolayer).arg(device).arg(format));
}

void CasparDevice::stopDeviceInput(int channel, int videolayer)
{
    AMCPDevice::writeMessage(QString("STOP %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::playImageScroll(int channel, int videolayer)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::playImageScroll(int channel, int videolayer, const QString& media, int blur, int speed, bool premultiply, bool progressive)
{
    AMCPDevice::writeMessage(QString("PLAY %1-%2 %3 BLUR %4 SPEED %5 %6 %7")
                             .arg(channel).arg(videolayer).arg(media).arg(blur).arg(speed)
                             .arg((premultiply == true) ? "PREMULTIPLY" : "")
                             .arg((progressive == true) ? "PROGRESSIVE" : ""));
}

void CasparDevice::loadImageScroll(int channel, int videolayer, const QString& media, int blur, int speed, bool premultiply, bool progressive)
{
    AMCPDevice::writeMessage(QString("LOADBG %1-%2 %3 BLUR %4 SPEED %5 %6 %7")
                             .arg(channel).arg(videolayer).arg(media).arg(blur).arg(speed)
                             .arg((premultiply == true) ? "PREMULTIPLY" : "")
                             .arg((progressive == true) ? "PROGRESSIVE" : ""));
}

void CasparDevice::stopImageScroll(int channel, int videolayer)
{
    AMCPDevice::writeMessage(QString("STOP %1-%2").arg(channel).arg(videolayer));
}

void CasparDevice::setCommit(int channel)
{
    AMCPDevice::writeMessage(QString("MIXER %1 COMMIT").arg(channel));
}

void CasparDevice::setBlend(int channel, int videolayer, const QString& blend)
{
    AMCPDevice::writeMessage(QString("MIXER %1-%2 BLEND %3 DEFER").arg(channel).arg(videolayer).arg(blend));
}

void CasparDevice::setGrid(int channel, int grid, int duration, const QString& easing, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1 GRID %2 %3 %4 DEFER").arg(channel).arg(grid).arg(duration).arg(easing));
    else
        AMCPDevice::writeMessage(QString("MIXER %1 GRID %2 %3 %4").arg(channel).arg(grid).arg(duration).arg(easing));
}

void CasparDevice::setKeyer(int channel, int videolayer, int keyer)
{
    AMCPDevice::writeMessage(QString("MIXER %1-%2 KEYER %3").arg(channel).arg(videolayer).arg(keyer));
}

void CasparDevice::setVolume(int channel, int videolayer, float volume, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 VOLUME %3 DEFER").arg(channel).arg(videolayer).arg(volume));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 VOLUME %3").arg(channel).arg(videolayer).arg(volume));
}

void CasparDevice::setVolume(int channel, int videolayer, float volume, int duration, const QString& easing, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 VOLUME %3 %4 %5 DEFER").arg(channel).arg(videolayer).arg(volume).arg(duration).arg(easing));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 VOLUME %3 %4 %5").arg(channel).arg(videolayer).arg(volume).arg(duration).arg(easing));
}

void CasparDevice::setOpacity(int channel, int videolayer, float opacity, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 OPACITY %3 DEFER").arg(channel).arg(videolayer).arg(opacity));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 OPACITY %3").arg(channel).arg(videolayer).arg(opacity));
}

void CasparDevice::setOpacity(int channel, int videolayer, float opacity, int duration, const QString& easing, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 OPACITY %3 %4 %5 DEFER").arg(channel).arg(videolayer).arg(opacity).arg(duration).arg(easing));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 OPACITY %3 %4 %5").arg(channel).arg(videolayer).arg(opacity).arg(duration).arg(easing));
}

void CasparDevice::setBrightness(int channel, int videolayer, float brightness, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 BRIGHTNESS %3 DEFER").arg(channel).arg(videolayer).arg(brightness));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 BRIGHTNESS %3").arg(channel).arg(videolayer).arg(brightness));
}

void CasparDevice::setBrightness(int channel, int videolayer, float brightness, int duration, const QString& easing, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 BRIGHTNESS %3 %4 %5 DEFER").arg(channel).arg(videolayer).arg(brightness).arg(duration).arg(easing));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 BRIGHTNESS %3 %4 %5").arg(channel).arg(videolayer).arg(brightness).arg(duration).arg(easing));
}

void CasparDevice::setContrast(int channel, int videolayer, float contrast, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 CONTRAST %3 DEFER").arg(channel).arg(videolayer).arg(contrast));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 CONTRAST %3").arg(channel).arg(videolayer).arg(contrast));
}

void CasparDevice::setContrast(int channel, int videolayer, float contrast, int duration, const QString& easing, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 CONTRAST %3 %4 %5 DEFER").arg(channel).arg(videolayer).arg(contrast).arg(duration).arg(easing));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 CONTRAST %3 %4 %5").arg(channel).arg(videolayer).arg(contrast).arg(duration).arg(easing));
}

void CasparDevice::setSaturation(int channel, int videolayer, float saturation, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 SATURATION %3 DEFER").arg(channel).arg(videolayer).arg(saturation));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 SATURATION %3").arg(channel).arg(videolayer).arg(saturation));
}

void CasparDevice::setSaturation(int channel, int videolayer, float saturation, int duration, const QString& easing, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 SATURATION %3 %4 %5 DEFER").arg(channel).arg(videolayer).arg(saturation).arg(duration).arg(easing));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 SATURATION %3 %4 %5").arg(channel).arg(videolayer).arg(saturation).arg(duration).arg(easing));
}

void CasparDevice::setLevels(int channel, int videolayer, float minIn, float maxIn, float gamma, float minOut, float maxOut, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 LEVELS %3 %4 %5 %6 %7 DEFER").arg(channel).arg(videolayer).arg(minIn).arg(maxIn).arg(gamma).arg(minOut).arg(maxOut));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 LEVELS %3 %4 %5 %6 %7").arg(channel).arg(videolayer).arg(minIn).arg(maxIn).arg(gamma).arg(minOut).arg(maxOut));
}

void CasparDevice::setLevels(int channel, int videolayer, float minIn, float maxIn, float gamma, float minOut, float maxOut, int duration, const QString& easing, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 LEVELS %3 %4 %5 %6 %7 %8 %9 DEFER").arg(channel).arg(videolayer).arg(minIn).arg(maxIn).arg(gamma).arg(minOut).arg(maxOut).arg(duration).arg(easing));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 LEVELS %3 %4 %5 %6 %7 %8 %9").arg(channel).arg(videolayer).arg(minIn).arg(maxIn).arg(gamma).arg(minOut).arg(maxOut).arg(duration).arg(easing));
}

void CasparDevice::setGeometry(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 FILL %3 %4 %5 %6 DEFER").arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 FILL %3 %4 %5 %6").arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY));
}

void CasparDevice::setGeometry(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, int duration, const QString& easing, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 FILL %3 %4 %5 %6 %7 %8 DEFER").arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY).arg(duration).arg(easing));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 FILL %3 %4 %5 %6 %7 %8").arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY).arg(duration).arg(easing));
}

void CasparDevice::setClipping(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 CLIP %3 %4 %5 %6 DEFER").arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 CLIP %3 %4 %5 %6").arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY));
}

void CasparDevice::setClipping(int channel, int videolayer, float positionX, float positionY, float scaleX, float scaleY, int duration, const QString& easing, bool defer)
{
    if (defer)
        AMCPDevice::writeMessage(QString("MIXER %1-%2 CLIP %3 %4 %5 %6 %7 %8 DEFER").arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY).arg(duration).arg(easing));
    else
        AMCPDevice::writeMessage(QString("MIXER %1-%2 CLIP %3 %4 %5 %6 %7 %8").arg(channel).arg(videolayer).arg(positionX).arg(positionY).arg(scaleX).arg(scaleY).arg(duration).arg(easing));
}

void CasparDevice::sendNotification()
{
    switch (AMCPDevice::command)
    {
        case AMCPDevice::CLS:
        {
            QList<QString> response;
            response.push_back(AMCPDevice::response.at(0));

            emit responseChanged(response, *this);

            AMCPDevice::response.removeFirst(); // First post is the header, 200 CLS OK.

            QList<CasparMedia> items;
            foreach (QString response, AMCPDevice::response)
            {
                QString name = response.split("\" ").at(0);
                name.replace("\\", "/").remove(QRegExp("^\"")).remove(QRegExp("\"$"));

                QString type = response.split("\" ").at(1).trimmed().split(" ").at(0);

                items.push_back(CasparMedia(name, type));
            }

            emit mediaChanged(items, *this);
        }
        break;
        case AMCPDevice::TLS:
        {
            QList<QString> response;
            response.push_back(AMCPDevice::response.at(0));

            emit responseChanged(response, *this);

            AMCPDevice::response.removeFirst(); // First post is the header, 200 TLS OK.

            QList<CasparTemplate> items;
            foreach (QString response, AMCPDevice::response)
            {
                QString name = response.split("\" ").at(0);
                name.replace("\\", "/").remove(QRegExp("^\"")).remove(QRegExp("\"$"));

                items.push_back(CasparTemplate(name));
            }

            emit templateChanged(items, *this);
        }
        break;
        case AMCPDevice::CINF:
            AMCPDevice::response.removeFirst(); // First post is the header, 200 CINF OK.     
            emit mediaInfoChanged(AMCPDevice::response, *this);

            break;
        case AMCPDevice::INFO:
            AMCPDevice::response.removeFirst(); // First post is the header, 201 INFO OK.     
            emit infoChanged(AMCPDevice::response, *this);

            break;
        case AMCPDevice::INFOSYSTEM:
            AMCPDevice::response.removeFirst(); // First post is the header, 201 INFO SYSTEM OK.     
            emit infoSystemChanged(AMCPDevice::response, *this);
            break;
        case AMCPDevice::DATALIST:
        {
            QList<QString> response;
            response.push_back(AMCPDevice::response.at(0));

            emit responseChanged(response, *this);

            AMCPDevice::response.removeFirst(); // First post is the header, 200 DATA LIST OK.

            QList<CasparData> items;
            foreach (QString response, AMCPDevice::response)
            {
                QString name = response.split("\" ").at(0);
                name.replace("\\", "/").remove(QRegExp("^\"")).remove(QRegExp("\"$"));

                items.push_back(CasparData(name));
            }

            emit dataChanged(items, *this);
        }
        break;
        case AMCPDevice::VERSION:
            AMCPDevice::response.removeFirst(); // First post is the header, 200 VERSION OK.          
            emit versionChanged(CasparVersion(AMCPDevice::response.at(0)), *this);      
            break;
        case AMCPDevice::CONNECTIONSTATE:
            emit connectionStateChanged(*this);
            break;
        default:
            emit responseChanged(AMCPDevice::response, *this);
            break;
    }

    AMCPDevice::resetDevice();
}
