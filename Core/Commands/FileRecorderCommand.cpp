#include "FileRecorderCommand.h"

#include "Global.h"

FileRecorderCommand::FileRecorderCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), filename(FileRecorder::DEFAULT_OUTPUT), container(FileRecorder::DEFAULT_CONTAINER),
      codec(FileRecorder::DEFAULT_CODEC), preset(FileRecorder::DEFAULT_PRESET), tune(FileRecorder::DEFAULT_TUNE)
{
}

int FileRecorderCommand::getDelay() const
{
    return this->delay;
}

int FileRecorderCommand::getChannel() const
{
    return this->channel;
}

int FileRecorderCommand::getVideolayer() const
{
    return this->videolayer;
}

bool FileRecorderCommand::getAllowGpi() const
{
    return this->allowGpi;
}

const QString& FileRecorderCommand::getFilename() const
{
    return this->filename;
}

const QString& FileRecorderCommand::getContainer() const
{
    return this->container;
}

const QString& FileRecorderCommand::getCodec() const
{
    return this->codec;
}

const QString& FileRecorderCommand::getPreset() const
{
    return this->preset;
}

const QString& FileRecorderCommand::getTune() const
{
    return this->tune;
}

void FileRecorderCommand::setChannel(int channel)
{
    this->channel = channel;
    emit channelChanged(this->channel);
}

void FileRecorderCommand::setVideolayer(int videolayer)
{
    this->videolayer = videolayer;
    emit videolayerChanged(this->videolayer);
}

void FileRecorderCommand::setDelay(int delay)
{
    this->delay = delay;
    emit delayChanged(this->delay);
}

void FileRecorderCommand::setAllowGpi(bool allowGpi)
{
    this->allowGpi = allowGpi;
    emit allowGpiChanged(this->allowGpi);
}

void FileRecorderCommand::setFilename(const QString& filename)
{
    this->filename = filename;
    emit filenameChanged(this->filename);
}

void FileRecorderCommand::setContainer(const QString& container)
{
    this->container = container;
    emit containerChanged(this->container);
}

void FileRecorderCommand::setCodec(const QString& codec)
{
    this->codec = codec;
    emit codecChanged(this->codec);
}

void FileRecorderCommand::setPreset(const QString& preset)
{
    this->preset = preset;
    emit presetChanged(this->preset);
}

void FileRecorderCommand::setTune(const QString& tune)
{
    this->tune = tune;
    emit tuneChanged(this->tune);
}

void FileRecorderCommand::readProperties(boost::property_tree::wptree& pt)
{
    setChannel(pt.get<int>(L"channel"));
    setDelay(pt.get<int>(L"delay"));
    setAllowGpi(pt.get<bool>(L"allowgpi"));
    setFilename(QString::fromStdWString(pt.get<std::wstring>(L"filename")));
    setContainer(QString::fromStdWString(pt.get<std::wstring>(L"container")));
    setCodec(QString::fromStdWString(pt.get<std::wstring>(L"codec")));
    setPreset(QString::fromStdWString(pt.get<std::wstring>(L"preset")));
    setTune(QString::fromStdWString(pt.get<std::wstring>(L"tune")));
}

void FileRecorderCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", "");
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("filename", this->getFilename());
    writer->writeTextElement("container", this->getContainer());
    writer->writeTextElement("codec", this->getCodec());
    writer->writeTextElement("preset", this->getPreset());
    writer->writeTextElement("tune", this->getTune());
}
