#include "LiveDialog.h"

LiveDialog::LiveDialog(QWidget* parent)
    : QDialog(parent, Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint)
{
    setupUi(this);
}

QWidget* LiveDialog::getRenderTarget()
{
    return this->labelLive;
}
