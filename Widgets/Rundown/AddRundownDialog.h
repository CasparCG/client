#pragma once

#include "../Shared.h"
#include "ui_AddRundownDialog.h"

#include "Animations/BorderAnimation.h"

#include <QtGui/QDialog>
#include <QtGui/QWidget>

class WIDGETS_EXPORT AddRundownDialog : public QDialog, Ui::AddRundownDialog
{
    Q_OBJECT

    public:
        explicit AddRundownDialog(QWidget* parent = 0);

        void setName(const QString& name);
        void setEditMode(bool editMode);

        const QString getName() const;

    protected:
        void accept();

    private:
        bool editMode;

        BorderAnimation* nameAnimation;

        Q_SLOT void nameChanged(QString);
};
