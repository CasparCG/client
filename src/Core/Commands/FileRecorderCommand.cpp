#include "FileRecorderCommand.h"

#include <QtCore/QXmlStreamWriter>

FileRecorderCommand::FileRecorderCommand(QObject* parent)
    : AbstractCommand(parent)
{
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

bool FileRecorderCommand::getWithAlpha() const
{
    return this->withAlpha;
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

void FileRecorderCommand::setWithAlpha(bool withAlpha)
{
    this->withAlpha = withAlpha;
    emit withAlphaChanged(this->withAlpha);
}

void FileRecorderCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setOutput(QString::fromStdWString(pt.get(L"output", FileRecorder::DEFAULT_OUTPUT.toStdWString())));
    setCodec(QString::fromStdWString(pt.get<std::wstring>(L"codec", FileRecorder::DEFAULT_CODEC.toStdWString())));
    setPreset(QString::fromStdWString(pt.get<std::wstring>(L"preset", FileRecorder::DEFAULT_PRESET.toStdWString())));
    setWithAlpha(pt.get(L"withalpha", FileRecorder::DEFAULT_WITH_ALPHA));
}

void FileRecorderCommand::writeProperties(QXmlStreamWriter& writer)
{
    AbstractCommand::writeProperties(writer);

    writer.writeTextElement("output", this->getOutput());
    writer.writeTextElement("codec", this->getCodec());
    writer.writeTextElement("preset", this->getPreset());
    writer.writeTextElement("withalpha", (getWithAlpha() == true) ? "true" : "false");
}
