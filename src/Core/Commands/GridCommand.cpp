#include "GridCommand.h"

#include "Global.h"

GridCommand::GridCommand(QObject* parent)
    : AbstractCommand(parent),
      grid(Mixer::DEFAULT_GRID), duration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int GridCommand::getGrid() const
{
    return this->grid;
}

int GridCommand::getDuration() const
{
    return this->duration;
}

const QString& GridCommand::getTween() const
{
    return this->tween;
}

bool GridCommand::getDefer() const
{
    return this->defer;
}

void GridCommand::setGrid(int grid)
{
    this->grid = grid;
    emit gridChanged(this->grid);
}

void GridCommand::setDuration(int duration)
{
    this->duration = duration;
    emit durationChanged(this->duration);
}

void GridCommand::setTween(const QString& tween)
{
    this->tween = tween;
    emit tweenChanged(this->tween);
}

void GridCommand::setDefer(bool defer)
{
    this->defer = defer;
    emit deferChanged(this->defer);
}

void GridCommand::readProperties(boost::property_tree::wptree& pt)
{
    AbstractCommand::readProperties(pt);

    if (pt.count(L"grid") > 0) setGrid(pt.get<float>(L"grid"));
    if (pt.count(L"duration") > 0) setDuration(pt.get<int>(L"duration"));
    if (pt.count(L"tween") > 0) setTween(QString::fromStdWString(pt.get<std::wstring>(L"tween")));
    if (pt.count(L"defer") > 0) setDefer(pt.get<bool>(L"defer"));
}

void GridCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("grid", QString::number(this->getGrid()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
