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

    setGrid(pt.get(L"grid", Mixer::DEFAULT_GRID));
    setDuration(pt.get(L"duration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void GridCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("grid", QString::number(this->getGrid()));
    writer->writeTextElement("duration", QString::number(this->getDuration()));
    writer->writeTextElement("tween", this->getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
