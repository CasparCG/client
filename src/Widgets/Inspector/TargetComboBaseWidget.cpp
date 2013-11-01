#include "TargetComboBaseWidget.h"

TargetComboBaseWidget::TargetComboBaseWidget(QWidget* parent)
    : QComboBox(parent),
      previousText("")
{
}

void TargetComboBaseWidget::mousePressEvent(QMouseEvent* event)
{
    this->previousText = this->currentText();
    QComboBox::mousePressEvent(event);
}

const QString& TargetComboBaseWidget::getPreviousText() const
{
    return this->previousText;
}
