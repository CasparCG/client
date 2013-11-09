#include "BlendModeCommand.h"

#include "Global.h"

BlendModeCommand::BlendModeCommand(QObject* parent)
    : AbstractCommand(parent),
    blendMode(Mixer::DEFAULT_BLENDMODE)
{
}

const QString& BlendModeCommand::getBlendMode() const
{
    return this->blendMode;
}

void BlendModeCommand::setBlendMode(const QString& blendMode)
{
    this->blendMode = blendMode;
    emit blendModeChanged(this->blendMode);
}

void BlendModeCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    setBlendMode(QString::fromStdWString(pt.get(L"blendmode", Mixer::DEFAULT_BLENDMODE.toStdWString())));
}

void BlendModeCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("blendmode", this->getBlendMode());
}
