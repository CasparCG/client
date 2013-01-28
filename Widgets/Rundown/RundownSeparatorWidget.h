#pragma once

#include "../Shared.h"
#include "IPlayoutCommand.h"
#include "IRundownWidget.h"
#include "ui_RundownSeparatorWidget.h"

#include "Global.h"

#include "Commands/ICommand.h"
#include "Commands/SeparatorCommand.h"
#include "Models/LibraryModel.h"

#include <stdexcept>

#include <QtCore/QString>

#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownSeparatorWidget : public QWidget, Ui::RundownSeparatorWidget, public IRundownWidget, public IPlayoutCommand
{
    Q_OBJECT

    public:
        explicit RundownSeparatorWidget(const LibraryModel& model, QWidget* parent = 0,
                                        const QString& color = Color::DEFAULT_SEPARATOR_COLOR,
                                        bool active = false, bool inGroup = false, bool compactView = false);

        virtual IRundownWidget* clone();

        virtual bool isGroup() const;

        virtual ICommand* getCommand();
        virtual LibraryModel* getLibraryModel();

        virtual void setActive(bool active);
        virtual void setInGroup(bool inGroup);
        virtual void setColor(const QString& color);

        virtual void setExpanded(bool expanded) {}

        virtual bool executeCommand(enum Playout::PlayoutType::Type type);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        virtual void setCompactView(bool compactView);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        bool active;
        bool inGroup;
        bool compactView;
        QString color;
        LibraryModel model;
        SeparatorCommand command;
};
