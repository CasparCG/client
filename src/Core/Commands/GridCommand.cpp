#include "GridCommand.h"

#include "Global.h"

GridCommand::GridCommand(QObject* parent)
    : AbstractCommand(parent),
      grid(Mixer::DEFAULT_GRID), transitionDuration(Mixer::DEFAULT_DURATION), tween(Mixer::DEFAULT_TWEEN), defer(Mixer::DEFAULT_DEFER)
{
}

int GridCommand::getGrid() const
{
    return this->grid;
}

int GridCommand::getTransitionDuration() const
{
    return this->transitionDuration;
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

void GridCommand::setTransitionDuration(int transitionDuration)
{
    this->transitionDuration = transitionDuration;
    emit transitionDurationChanged(this->transitionDuration);
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
    setTransitionDuration(pt.get(L"transitionDuration", Mixer::DEFAULT_DURATION));
    setTween(QString::fromStdWString(pt.get(L"tween", Mixer::DEFAULT_TWEEN.toStdWString())));
    setDefer(pt.get(L"defer", Mixer::DEFAULT_DEFER));
}

void GridCommand::writeProperties(QXmlStreamWriter* writer)
{
    AbstractCommand::writeProperties(writer);

    writer->writeTextElement("grid", QString::number(getGrid()));
    writer->writeTextElement("transitionDuration", QString::number(getTransitionDuration()));
    writer->writeTextElement("tween", getTween());
    writer->writeTextElement("defer", (getDefer() == true) ? "true" : "false");
}
