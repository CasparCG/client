#pragma once

#include "Shared.h"
#include "ui_TemplateDataDialog.h"

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT TemplateDataDialog : public QDialog, Ui::TemplateDataDialog
{
    Q_OBJECT

    public:
        explicit TemplateDataDialog(QWidget* parent = 0);

        const QString getName() const;
        const QString getValue() const;

        void setName(const QString& name);
        void setValue(const QString& value);

    protected:
        void accept();

    private:
};
