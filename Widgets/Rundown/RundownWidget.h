#pragma once

#include "../Shared.h"
#include "ui_RundownWidget.h"
#include "GpiDevice.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QEvent>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

#include <QtGui/QKeyEvent>
#include <QtGui/QMenu>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

class WIDGETS_EXPORT RundownWidget : public QWidget, Ui::RundownWidget
{
    Q_OBJECT

    public:
        explicit RundownWidget(QWidget* parent = 0);

    protected:
        virtual bool eventFilter(QObject* target, QEvent* event);

    private:
        QString page;

        QMenu* newMenu;
        QMenu* colorMenu;
        QMenu* mixerMenu;
        QMenu* libraryMenu;
        QMenu* contextMenu;

        bool compactView;
        bool enterPressed;
        QMap<int, Playout::PlayoutType::Type> gpiBindings;

        QTreeWidgetItem* copyItem;

        enum ActionSource
        {
            KeyPress,
            GpiPulse
        };

        void checkEmptyRundown();
        void colorizeItems(const QString& color);
        bool executeCommand(Playout::PlayoutType::Type type, ActionSource source);
        bool groupItems();
        bool moveItemDown();
        bool moveItemIntoGroup();
        bool moveItemOutOfGroup();
        bool moveItemUp();
        void parsePage(QKeyEvent* event);
        bool removeSelectedItems();
        void setupUiMenu();
        bool ungroupItems();
        bool copySelectedItem();
        bool pasteSelectedItem();
        void readRundownGroup(const QString& type, boost::property_tree::wptree& pt);
        void readRundownItem(const QString& type, boost::property_tree::wptree& pt, QTreeWidgetItem* parent);
        void writeRundownGroup(const QString& type, QXmlStreamWriter* writer, QTreeWidgetItem* child);
        void writeRundownItem(const QString& type, QXmlStreamWriter* writer, QTreeWidgetItem* item);

        Q_SLOT void addSeparatorCommand();
        Q_SLOT void addFileRecorderCommand();
        Q_SLOT void addImageScrollerCommand();
        Q_SLOT void addAudioCommand();
        Q_SLOT void addImageCommand();
        Q_SLOT void addTemplateCommand();
        Q_SLOT void addVideoCommand();
        Q_SLOT void addBlendModeCommand();
        Q_SLOT void addBrightnessCommand();
        Q_SLOT void addCommitCommand();
        Q_SLOT void addContrastCommand();
        Q_SLOT void addCropCommand();
        Q_SLOT void addDeckLinkInputCommand();
        Q_SLOT void addGeometryCommand();
        Q_SLOT void addGpiOutputCommand();
        Q_SLOT void addGridCommand();
        Q_SLOT void addKeyerCommand();
        Q_SLOT void addLevelsCommand();
        Q_SLOT void addOpacityCommand();
        Q_SLOT void addSaturationCommand();
        Q_SLOT void addVolumeCommand();
        Q_SLOT void colorMenuTriggered(QAction*);
        Q_SLOT void contextMenuTriggered(QAction*);
        Q_SLOT void customContextMenuRequested(const QPoint&);
        Q_SLOT void gpiPortTriggered(int, GpiDevice*);
        Q_SLOT void gpiBindingChanged(int, Playout::PlayoutType::Type);
        Q_SLOT void filterRundown();
        Q_SLOT void currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
        Q_SLOT void itemDoubleClicked(QTreeWidgetItem*, int);
        Q_SLOT void newMenuTriggered(QAction*);
};
