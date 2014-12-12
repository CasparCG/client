#include "Playout.h"

#include <QtCore/QList>
#include <QtCore/QString>

#include <stdexcept>

Playout::PlayoutType::Type Playout::fromString(const QString& value)
{
    if (value == "Stop")
        return PlayoutType::Stop;
    else if (value == "Play")
        return PlayoutType::Play;
    else if (value == "Play Now")
        return PlayoutType::PlayNow;
    else if (value == "Pause / Resume")
        return PlayoutType::PauseResume;
    else if (value == "Load")
        return PlayoutType::Load;
    else if (value == "Next")
        return PlayoutType::Next;
    else if (value == "Update")
        return PlayoutType::Update;
    else if (value == "Invoke")
        return PlayoutType::Invoke;
    else if (value == "Preview")
        return PlayoutType::Preview;
    else if (value == "Clear")
        return PlayoutType::Clear;
    else if (value == "Clear Video Layer")
        return PlayoutType::ClearVideoLayer;
    else if (value == "Clear Channel")
        return PlayoutType::ClearChannel;
    else
        throw std::invalid_argument(value.toStdString());
}

QString Playout::toString(PlayoutType::Type value)
{
    switch (value)
    {
        case PlayoutType::Stop:
            return "Stop";
        case PlayoutType::Play:
            return "Play";
        case PlayoutType::PlayNow:
            return "Play Now";
        case PlayoutType::PauseResume:
            return "Pause / Resume";
        case PlayoutType::Load:
            return "Load";
        case PlayoutType::Next:
            return "Next";
        case PlayoutType::Update:
            return "Update";
        case PlayoutType::Invoke:
            return "Invoke";
        case PlayoutType::Preview:
            return "Preview";
        case PlayoutType::Clear:
            return "Clear";
        case PlayoutType::ClearVideoLayer:
            return "Clear Video Layer";
        case PlayoutType::ClearChannel:
            return "Clear Channel";
        default:
            throw std::invalid_argument("Invalid enum constant");
    }
}

QList<Playout::PlayoutType::Type> Playout::createConstantList()
{
    QList<Playout::PlayoutType::Type> result;

    result.push_back(Playout::PlayoutType::Stop);
    result.push_back(Playout::PlayoutType::Play);
    result.push_back(Playout::PlayoutType::PlayNow);
    result.push_back(Playout::PlayoutType::PauseResume);
    result.push_back(Playout::PlayoutType::Load);
    result.push_back(Playout::PlayoutType::Next);
    result.push_back(Playout::PlayoutType::Update);
    result.push_back(Playout::PlayoutType::Invoke);
    result.push_back(Playout::PlayoutType::Preview);
    result.push_back(Playout::PlayoutType::Clear);
    result.push_back(Playout::PlayoutType::ClearVideoLayer);
    result.push_back(Playout::PlayoutType::ClearChannel);

    return result;
}

const QList<Playout::PlayoutType::Type>& Playout::enumConstants()
{
    static QList<Playout::PlayoutType::Type> result(createConstantList());

    return result;
}
