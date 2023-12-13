#pragma once

#include "../Shared.h"
#include "AbstractRundownWidget.h"
#include "RundownTreeBaseWidget.h"
#include "ui_RundownTreeWidget.h"

#include "Global.h"

#include "GpiDevice.h"
#include "RepositoryDevice.h"
#include "Models/RepositoryChangeModel.h"

#include "Events/AddPresetItemEvent.h"
#include "Events/SaveAsPresetEvent.h"
#include "Events/Inspector/AutoPlayChangedEvent.h"
#include "Events/Rundown/AddRudnownItemEvent.h"
#include "Events/Rundown/AutoPlayNextRundownItemEvent.h"
#include "Events/Rundown/AutoPlayRundownItemEvent.h"
#include "Events/Rundown/CompactViewEvent.h"
#include "Events/Rundown/ExecutePlayoutCommandEvent.h"
#include "Events/Rundown/ExecuteRundownItemEvent.h"
#include "Events/Rundown/AllowRemoteTriggeringEvent.h"
#include "Events/Rundown/RemoveItemFromAutoPlayQueueEvent.h"
#include "Events/Rundown/ClearCurrentPlayingItemEvent.h"
#include "Events/Rundown/SaveRundownEvent.h"
#include "Events/Rundown/CopyItemPropertiesEvent.h"
#include "Events/Rundown/PasteItemPropertiesEvent.h"
#include "Events/Rundown/InsertRepositoryChangesEvent.h"
#include "Events/Rundown/CurrentItemChangedEvent.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QEvent>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>
#include <QtCore/QSharedPointer>

#include <QtWidgets/QMenu>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QWidget>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

class WIDGETS_EXPORT RundownTreeWidget : public QWidget, Ui::RundownTreeWidget
{
    Q_OBJECT

    public:
        explicit RundownTreeWidget(QWidget* parent = 0);

        void setActive(bool active);
        void openRundown(const QString& path);
        void openRundownFromUrl(const QString& url);
        void saveRundown(bool saveAs);
        void reloadRundown();
        void setUsed(bool used);
        void setAllUsed(bool used);

        bool checkForSave() const;

        bool getAllowRemoteTriggering() const;
        bool executeCommand(Playout::PlayoutType type, Action::ActionType source, QTreeWidgetItem* item = NULL);

        Q_SLOT void gpiBindingChanged(int, Playout::PlayoutType);

    private:
        bool active;
        bool enterPressed;
        bool allowRemoteRundownTriggering;
        bool repositoryRundown;
        bool previewOnAutoStep;
        bool clearDelayedCommandsOnAutoStep;

        QString page;
        QString activeRundown;
        QString hexHash;

        QMenu* contextMenuTools;
        QMenu* contextMenuColor;
        QMenu* contextMenuMark;
        QMenu* contextMenuMixer;
        QMenu* contextMenuOther;
        QMenu* contextMenuLibrary;
        QMenu* contextMenuRundown;

        QMap<int, Playout::PlayoutType> gpiBindings;

        AbstractRundownWidget* currentAutoPlayWidget;
        QList<QList<AbstractRundownWidget*>* > autoPlayQueues;

        QTreeWidgetItem* copyItem;
        QTreeWidgetItem* currentPlayingItem;
        QTreeWidgetItem* currentPlayingAutoStepItem;

        OscSubscription* upControlSubscription;
        OscSubscription* downControlSubscription;
        OscSubscription* playAndAutoStepControlSubscription;
        OscSubscription* playNowAndAutoStepControlSubscription;
        OscSubscription* playNowIfChannelControlSubscription;
        OscSubscription* stopControlSubscription;
        OscSubscription* playControlSubscription;
        OscSubscription* playNowControlSubscription;
        OscSubscription* loadControlSubscription;
        OscSubscription* pauseControlSubscription;
        OscSubscription* nextControlSubscription;
        OscSubscription* updateControlSubscription;
        OscSubscription* invokeControlSubscription;
        OscSubscription* clearControlSubscription;
        OscSubscription* clearVideolayerControlSubscription;
        OscSubscription* clearChannelControlSubscription;

        QSharedPointer<RepositoryDevice> repositoryDevice;

        QNetworkAccessManager* networkManager;

        bool pasteSelectedItems();
        bool duplicateSelectedItems();
        bool copySelectedItems() const;
        void setupMenus();
        void colorizeItems(const QString& color);
        void resetOscSubscriptions();
        void configureOscSubscriptions();
        QString colorLookup(const QString& color, bool reverse) const;

        Q_SLOT void addPlayoutCommandItem();
        Q_SLOT void addCustomCommandItem();
        Q_SLOT void addChromaKeyItem();
        Q_SLOT void addPrintItem();
        Q_SLOT void addSeparatorItem();
        Q_SLOT void addFileRecorderItem();
        Q_SLOT void addImageScrollerItem();
        Q_SLOT void addAudioItem();
        Q_SLOT void addImageItem();
        Q_SLOT void addTemplateItem();
        Q_SLOT void addVideoItem();
        Q_SLOT void addBlendModeItem();
        Q_SLOT void addSolidColorItem();
        Q_SLOT void addFadeToBlackItem();
        Q_SLOT void addBrightnessItem();
        Q_SLOT void addCommitItem();
        Q_SLOT void addContrastItem();
        Q_SLOT void addClipItem();
        Q_SLOT void addCropItem();
        Q_SLOT void addDeckLinkInputItem();
        Q_SLOT void addFillItem();
        Q_SLOT void addGpiOutputItem();
        Q_SLOT void addGridItem();
        Q_SLOT void addClearOutputItem();
        Q_SLOT void addKeyerItem();
        Q_SLOT void addLevelsItem();
        Q_SLOT void addOpacityItem();
        Q_SLOT void addSaturationItem();
        Q_SLOT void addVolumeItem();
        Q_SLOT void addPerspectiveItem();
        Q_SLOT void addRotationItem();
        Q_SLOT void addAnchorItem();
        Q_SLOT void addHttpGetItem();
        Q_SLOT void addHttpPostItem();
        Q_SLOT void addResetItem();
        Q_SLOT void addHtmlItem();
        Q_SLOT void addRouteChannelItem();
        Q_SLOT void addRouteVideolayerItem();
        Q_SLOT void contextMenuColorTriggered(QAction*);
        Q_SLOT void contextMenuRundownTriggered(QAction*);
        Q_SLOT void customContextMenuRequested(const QPoint&);
        Q_SLOT void gpiPortTriggered(int, GpiDevice*);
        Q_SLOT void currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
        Q_SLOT void itemDoubleClicked(QTreeWidgetItem*, int);
        Q_SLOT void itemClicked(QTreeWidgetItem*, int);
        Q_SLOT void itemSelectionChanged();
        Q_SLOT void contextMenuNewTriggered(QAction*);
        Q_SLOT void selectItemBelow();
        Q_SLOT void executePreview();
        Q_SLOT bool removeSelectedItems();
        Q_SLOT void saveAsPreset();
        Q_SLOT void addOscOutputItem();
        Q_SLOT void removeItemFromAutoPlayQueue(const RemoveItemFromAutoPlayQueueEvent&);
        Q_SLOT void clearCurrentPlayingItem(const ClearCurrentPlayingItemEvent&);
        Q_SLOT void executePlayoutCommand(const ExecutePlayoutCommandEvent&);
        Q_SLOT void saveAsPreset(const SaveAsPresetEvent&);
        Q_SLOT void addPresetItem(const AddPresetItemEvent&);
        Q_SLOT void addRudnownItem(const AddRudnownItemEvent&);
        Q_SLOT void compactView(const CompactViewEvent&);
        Q_SLOT void executeRundownItem(const ExecuteRundownItemEvent&);
        Q_SLOT void allowRemoteTriggering(const AllowRemoteTriggeringEvent&);
        Q_SLOT void autoPlayRundownItem(const AutoPlayRundownItemEvent&);
        Q_SLOT void autoPlayChanged(const AutoPlayChangedEvent&);
        Q_SLOT void autoPlayNextRundownItem(const AutoPlayNextRundownItemEvent&);
        Q_SLOT void playAndAutoStepControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void playNowAndAutoStepControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void playNowIfChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void upControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void downControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void stopControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void playControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void playNowControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void loadControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void pauseControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void nextControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void updateControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void invokeControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearVideolayerControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void clearChannelControlSubscriptionReceived(const QString&, const QList<QVariant>&);
        Q_SLOT void doOpenRundownFromUrl(QNetworkReply*);
        Q_SLOT void markItemAsUsed();
        Q_SLOT void markItemAsUnused();
        Q_SLOT void markAllItemsAsUsed();
        Q_SLOT void markAllItemsAsUnused();
        Q_SLOT void copyItemProperties(const CopyItemPropertiesEvent&);
        Q_SLOT void pasteItemProperties(const PasteItemPropertiesEvent&);
        Q_SLOT void copyItemProperties();
        Q_SLOT void pasteItemProperties();
        Q_SLOT void repositoryConnectionStateChanged(RepositoryDevice&);
        Q_SLOT void repositoryChanged(const RepositoryChangeModel&, RepositoryDevice&);
        Q_SLOT void insertRepositoryChanges(const InsertRepositoryChangesEvent&);
        Q_SLOT void currentItemChanged(const CurrentItemChangedEvent&);
        Q_SLOT void clearDelayedCommands();
};
