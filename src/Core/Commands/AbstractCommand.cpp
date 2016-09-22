#include "AbstractCommand.h"

#include "Global.h"

AbstractCommand::AbstractCommand(QObject* parent)
    : QObject(parent)
    , channel(Output::DEFAULT_CHANNEL, this)
    , videolayer(Output::DEFAULT_VIDEOLAYER, this)
    , delay(Output::DEFAULT_DELAY, this)
    , duration(Output::DEFAULT_DURATION, this)
    , allowGpi(Output::DEFAULT_ALLOW_GPI, this)
    , allowRemoteTriggering(Output::DEFAULT_ALLOW_REMOTE_TRIGGERING, this)
    , remoteTriggerId(Output::DEFAULT_REMOTE_TRIGGER_ID, this)
    , storyId("", this)
{ }

AbstractCommand::~AbstractCommand() { }
