#include "Global.h"

namespace Playout {

PlayoutType::Type fromString(const QString& value)
{
    if (value == "Stop")
        return PlayoutType::Stop;
    else if (value == "Play")
        return PlayoutType::Play;
    else if (value == "Pause")
        return PlayoutType::Pause;
    else if (value == "Load")
        return PlayoutType::Load;
    else if (value == "Next")
        return PlayoutType::Next;
    else if (value == "Update")
        return PlayoutType::Update;
    else if (value == "Invoke")
        return PlayoutType::Invoke;
    else if (value == "Clear")
        return PlayoutType::Clear;
    else if (value == "ClearVideolayer")
        return PlayoutType::ClearVideolayer;
    else if (value == "ClearChannel")
        return PlayoutType::ClearChannel;
    else
        throw std::invalid_argument(value.toStdString());
}

QString toString(PlayoutType::Type value)
{
    switch (value)
    {
    case PlayoutType::Stop:
        return "Stop";
    case PlayoutType::Play:
        return "Play";
    case PlayoutType::Pause:
        return "Pause";
    case PlayoutType::Load:
        return "Load";
    case PlayoutType::Next:
        return "Next";
    case PlayoutType::Update:
        return "Update";
    case PlayoutType::Invoke:
        return "Invoke";
    case PlayoutType::Clear:
        return "Clear";
    case PlayoutType::ClearVideolayer:
        return "ClearVideolayer";
    case PlayoutType::ClearChannel:
        return "ClearChannel";
    default:
        throw std::invalid_argument("Invalid enum constant");
    }
}

QList<PlayoutType::Type> createConstantList()
{
    QList<PlayoutType::Type> result;

    result.push_back(PlayoutType::Stop);
    result.push_back(PlayoutType::Play);
    result.push_back(PlayoutType::Pause);
    result.push_back(PlayoutType::Load);
    result.push_back(PlayoutType::Next);
    result.push_back(PlayoutType::Update);
    result.push_back(PlayoutType::Invoke);
    result.push_back(PlayoutType::Clear);
    result.push_back(PlayoutType::ClearVideolayer);
    result.push_back(PlayoutType::ClearChannel);

    return result;
}

const QList<PlayoutType::Type>& enumConstants()
{
    static QList<PlayoutType::Type> result(createConstantList());

    return result;
}

}
