#include "InspectorImageScrollerWidget.h"

#include "Global.h"

#include "Events/RundownItemSelectedEvent.h"

InspectorImageScrollerWidget::InspectorImageScrollerWidget(QWidget* parent)
    : QWidget(parent),
      model(NULL), command(NULL)
{
    setupUi(this);

    qApp->installEventFilter(this);
}

bool InspectorImageScrollerWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        this->model = rundownItemSelectedEvent->getLibraryModel();

        blockAllSignals(true);

        if (dynamic_cast<ImageScrollerCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->command = dynamic_cast<ImageScrollerCommand*>(rundownItemSelectedEvent->getCommand());

            this->spinBoxBlur->setValue(this->command->getBlur());
            this->spinBoxSpeed->setValue(this->command->getSpeed());
            this->checkBoxPremultiply->setChecked(this->command->getPremultiply());
            this->checkBoxProgressive->setChecked(this->command->getProgressive());
        }

        blockAllSignals(false);
    }

    return QObject::eventFilter(target, event);
}

void InspectorImageScrollerWidget::blockAllSignals(bool block)
{
    this->spinBoxBlur->blockSignals(block);
    this->spinBoxSpeed->blockSignals(block);
    this->checkBoxPremultiply->blockSignals(block);
    this->checkBoxProgressive->blockSignals(block);
}

void InspectorImageScrollerWidget::blurChanged(int blur)
{
    this->command->setBlur(blur);
}

void InspectorImageScrollerWidget::speedChanged(int speed)
{
    this->command->setSpeed(speed);
}

void InspectorImageScrollerWidget::premultiplyChanged(int state)
{
    this->command->setPremultiply((state == Qt::Checked) ? true : false);
}

void InspectorImageScrollerWidget::progressiveChanged(int state)
{
    this->command->setProgressive((state == Qt::Checked) ? true : false);
}

void InspectorImageScrollerWidget::resetSpeed(QString speed)
{
    this->spinBoxSpeed->setValue(ImageScroller::DEFAULT_SPEED);
    this->command->setSpeed(this->spinBoxSpeed->value());
}

void InspectorImageScrollerWidget::resetBlur(QString blur)
{
    this->spinBoxBlur->setValue(ImageScroller::DEFAULT_BLUR);
    this->command->setBlur(this->spinBoxBlur->value());
}

void InspectorImageScrollerWidget::resetPremultiply(QString premultiply)
{
    this->checkBoxPremultiply->setChecked(false);
    this->command->setPremultiply(this->checkBoxPremultiply->isChecked());
}

void InspectorImageScrollerWidget::resetProgressive(QString progressive)
{
    this->checkBoxProgressive->setChecked(false);
    this->command->setProgressive(this->checkBoxProgressive->isChecked());
}
