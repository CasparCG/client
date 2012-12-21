#include "InspectorGridWidget.h"

#include "Global.h"

#include "DatabaseManager.h"
#include "Events/RundownItemPreviewEvent.h"
#include "Events/RundownItemSelectedEvent.h"
#include "Models/TweenModel.h"

#include <QtGui/QApplication>

InspectorGridWidget::InspectorGridWidget(QWidget* parent)
    : QWidget(parent), preview(false), model(NULL), command(NULL)
{
    setupUi(this);

    loadTween();

    qApp->installEventFilter(this);
}

bool InspectorGridWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == static_cast<QEvent::Type>(Enum::EventType::RundownItemSelected))
    {
        RundownItemSelectedEvent* rundownItemSelectedEvent = dynamic_cast<RundownItemSelectedEvent*>(event);
        if (dynamic_cast<GridCommand*>(rundownItemSelectedEvent->getCommand()))
        {
            this->preview = false;

            this->model = rundownItemSelectedEvent->getLibraryModel();
            this->command = dynamic_cast<GridCommand*>(rundownItemSelectedEvent->getCommand());

            this->spinBoxGrid->setValue(this->command->getGrid());
            this->spinBoxDuration->setValue(this->command->getDuration());
            this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(this->command->getTween()));
            this->checkBoxDefer->setChecked(this->command->getDefer());

            this->preview = true;
        }
    }

    return QObject::eventFilter(target, event);
}

void InspectorGridWidget::loadTween()
{
    // We do not have a command object, block the signals.
    // Events will not be triggered while we update the values.
    this->comboBoxTween->blockSignals(true);

    QList<TweenModel> models = DatabaseManager::getInstance().getTween();
    foreach (TweenModel model, models)
        this->comboBoxTween->addItem(model.getValue());

    this->comboBoxTween->blockSignals(false);
}

void InspectorGridWidget::durationChanged(int duration)
{
    this->command->setDuration(duration);
}

void InspectorGridWidget::tweenChanged(QString tween)
{
    this->command->setTween(tween);
}

void InspectorGridWidget::gridChanged(int grid)
{
    this->command->setGrid(grid);

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGridWidget::resetGrid(QString grid)
{
    this->spinBoxGrid->setValue(1);
    this->command->setGrid(this->spinBoxGrid->value());

    if (this->preview)
        qApp->postEvent(qApp, new RundownItemPreviewEvent());
}

void InspectorGridWidget::resetDuration(QString duration)
{
    this->spinBoxDuration->setValue(Mixer::DEFAULT_DURATION);
    this->command->setDuration(this->spinBoxDuration->value());
}

void InspectorGridWidget::resetTween(QString tween)
{
    this->comboBoxTween->setCurrentIndex(this->comboBoxTween->findText(Mixer::DEFAULT_TWEEN));
    this->command->setTween(this->comboBoxTween->currentText());
}

void InspectorGridWidget::deferChanged(int state)
{
    this->command->setDefer((state == Qt::Checked) ? true : false);
}

void InspectorGridWidget::resetDefer(QString defer)
{
    this->checkBoxDefer->setChecked(false);
    this->command->setDefer(this->checkBoxDefer->isChecked());
}
