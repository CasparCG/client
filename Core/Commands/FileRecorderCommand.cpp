#include "FileRecorderCommand.h"

#include "Global.h"

FileRecorderCommand::FileRecorderCommand(QObject* parent)
    : QObject(parent),
      channel(Output::DEFAULT_CHANNEL), videolayer(Output::DEFAULT_VIDEOLAYER), delay(Output::DEFAULT_DELAY),
      allowGpi(Output::DEFAULT_ALLOW_GPI), output(FileRecorder::DEFAULT_OUTPUT), codec(FileRecorder::DEFAULT_CODEC),
      preset(FileRecorder::DEFAULT_PRESET), tune(FileRecorder::DEFAULT_TUNE), withAlpha(FileRecorder::DEFAULT_WITH_ALPHA)
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

const QString& FileRecorderCommand::getOutput() const
{
    return this->output;
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

bool FileRecorderCommand::getWithAlpha() const
{
    return this->withAlpha;
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

void FileRecorderCommand::setOutput(const QString& output)
{
    this->output = output;
    emit outputChanged(this->output);
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

void FileRecorderCommand::setWithAlpha(bool withAlpha)
{
    this->withAlpha = withAlpha;
    emit withAlphaChanged(this->withAlpha);
}

void FileRecorderCommand::readProperties(boost::property_tree::wptree& pt)
{
    if (pt.count(L"channel") > 0) setChannel(pt.get<int>(L"channel"));
    if (pt.count(L"delay") > 0) setDelay(pt.get<int>(L"delay"));
    if (pt.count(L"allowgpi") > 0) setAllowGpi(pt.get<bool>(L"allowgpi"));
    if (pt.count(L"output") > 0) setOutput(QString::fromStdWString(pt.get<std::wstring>(L"output")));
    if (pt.count(L"codec") > 0) setCodec(QString::fromStdWString(pt.get<std::wstring>(L"codec")));
    if (pt.count(L"preset") > 0) setPreset(QString::fromStdWString(pt.get<std::wstring>(L"preset")));
    if (pt.count(L"tune") > 0) setTune(QString::fromStdWString(pt.get<std::wstring>(L"tune")));
    if (pt.count(L"withalpha") > 0) setWithAlpha(pt.get<bool>(L"withalpha"));
}

void FileRecorderCommand::writeProperties(QXmlStreamWriter* writer)
{
    writer->writeTextElement("channel", QString::number(this->getChannel()));
    writer->writeTextElement("videolayer", "");
    writer->writeTextElement("delay", QString::number(this->getDelay()));
    writer->writeTextElement("allowgpi", (getAllowGpi() == true) ? "true" : "false");
    writer->writeTextElement("output", this->getOutput());
    writer->writeTextElement("codec", this->getCodec());
    writer->writeTextElement("preset", this->getPreset());
    writer->writeTextElement("tune", this->getTune());
    writer->writeTextElement("withalpha", (getWithAlpha() == true) ? "true" : "false");
}
