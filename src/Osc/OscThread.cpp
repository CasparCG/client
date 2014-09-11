#include "OscThread.h"

OscThread::OscThread(SocketReceiveMultiplexer* multiplexer, QObject* parent)
    : QThread(parent),
      multiplexer(multiplexer)
{
}

void OscThread::run()
{
    this->multiplexer->Run();
}

void OscThread::stop()
{
    this->multiplexer->AsynchronousBreak();
}
