#include "EventManager.h"

#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>

Q_GLOBAL_STATIC(EventManager, eventManager)

EventManager::EventManager()
{
}

EventManager& EventManager::getInstance()
{
    return *eventManager();
}

void EventManager::initialize()
{
}

void EventManager::uninitialize()
{
}

void EventManager::fireProcessEvent()
{
    qApp->processEvents();
}

void EventManager::fireImportPresetEvent()
{
    qApp->postEvent(qApp, new ImportPresetEvent());
}

void EventManager::fireExportPresetEvent()
{
    qApp->postEvent(qApp, new ExportPresetEvent());
}

void EventManager::fireNewRundownEvent()
{
    qApp->postEvent(qApp, new NewRundownEvent());
}

void EventManager::fireNewRundownMenuEvent(bool enabled)
{
    qApp->postEvent(qApp, new NewRundownMenuEvent(enabled));
}

void EventManager::fireDeleteRundownEvent(int index)
{
    qApp->postEvent(qApp, new DeleteRundownEvent(index));
}

void EventManager::fireRundownIsChangedEvent(bool changed)
{
    qApp->postEvent(qApp, new RundownIsChangedEvent(changed));
}

void EventManager::fireOpenRundownEvent()
{
    qApp->postEvent(qApp, new OpenRundownEvent());
}

void EventManager::fireSaveRundownEvent(bool saveAs)
{
    qApp->postEvent(qApp, new SaveRundownEvent(saveAs));
}

void EventManager::fireExecuteStopEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireExecutePlayEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireExecuteLoadEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F3, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireExecutePauseEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireExecuteNextEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F5, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireExecuteUpdateEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F6, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireExecuteInvokeEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F7, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireExecuteClearEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F10, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireExecuteClearVideolayerEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F11, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireExecuteClearChannelEvent()
{
    // Use synchronous event through sendEvent().
    QKeyEvent event(QEvent::KeyPress, Qt::Key_F12, Qt::NoModifier);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireToggleCompactViewEvent()
{
    // Use synchronous event through sendEvent().
    qApp->postEvent(qApp, new CompactViewEvent());
}

void EventManager::fireRefreshLibraryEvent(int delay)
{
    // Use synchronous event through sendEvent().
    qApp->postEvent(qApp, new RefreshLibraryEvent(delay));
}

void EventManager::fireEmptyRundownEvent()
{
    // Use synchronous event through sendEvent().
    qApp->sendEvent(qApp, new EmptyRundownEvent());
}

void EventManager::fireAddRudnownItemEvent(const LibraryModel& model)
{
    qApp->postEvent(qApp, new AddRudnownItemEvent(model));
}

void EventManager::fireAddTemplateDataEvent(const QString& value, bool storedData)
{
    qApp->postEvent(qApp, new AddTemplateDataEvent(value, storedData));
}

void EventManager::fireAddRudnownItemEvent(const QString& type)
{
    if (type == Rundown::BLENDMODE)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Blend Mode", "", "", Rundown::BLENDMODE, 0)));
    else if (type == Rundown::BRIGHTNESS)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Brightness", "", "", Rundown::BRIGHTNESS, 0)));
    else if (type == Rundown::CONTRAST)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Contrast", "", "", Rundown::CONTRAST, 0)));
    else if (type == Rundown::CROP)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Crop", "", "", Rundown::CROP, 0)));
    else if (type == Rundown::IMAGESCROLLER)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Image Scroller", "", "", Rundown::IMAGESCROLLER, 0)));
    else if (type == Rundown::DECKLINKINPUT)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "DeckLink Input", "", "", Rundown::DECKLINKINPUT, 0)));
    else if (type == Rundown::PRINT)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Channel Snapshot", "", "", Rundown::PRINT, 0)));
    else if (type == Rundown::CLEAROUTPUT)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Clear Output", "", "", Rundown::CLEAROUTPUT, 0)));
    else if (type == Rundown::GEOMETRY)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Transformation", "", "", Rundown::GEOMETRY, 0)));
    else if (type == Rundown::GPIOUTPUT)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "GPI Output", "", "", Rundown::GPIOUTPUT, 0)));
    else if (type == Rundown::FILERECORDER)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "File Recorder", "", "", Rundown::FILERECORDER, 0)));
    else if (type == Rundown::SEPARATOR)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Separator", "", "", Rundown::SEPARATOR, 0)));
    else if (type == Rundown::GRID)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Grid", "", "", Rundown::GRID, 0)));
    else if (type == Rundown::SOLIDCOLOR)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Solid Color", "", "", Rundown::SOLIDCOLOR, 0)));
    else if (type == Rundown::KEYER)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Mask", "", "", Rundown::KEYER, 0)));
    else if (type == Rundown::LEVELS)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Levels", "", "", Rundown::LEVELS, 0)));
    else if (type == Rundown::OPACITY)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Opacity", "", "", Rundown::OPACITY, 0)));
    else if (type == Rundown::SATURATION)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Saturation", "", "", Rundown::SATURATION, 0)));
    else if (type == Rundown::VOLUME)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Volume", "", "", Rundown::VOLUME, 0)));
    else if (type == Rundown::COMMIT)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Commit", "", "", Rundown::COMMIT, 0)));
    else if (type == Rundown::AUDIO)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Audio", "", "", Rundown::AUDIO, 0)));
    else if (type == Rundown::IMAGE)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Image", "", "", Rundown::IMAGE, 0)));
    else if (type == Rundown::TEMPLATE)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Template", "", "", Rundown::TEMPLATE, 0)));
    else if (type == Rundown::VIDEO)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Video", "", "", Rundown::VIDEO, 0)));
    else if (type == Rundown::CUSTOMCOMMAND)
        qApp->postEvent(qApp, new AddRudnownItemEvent(LibraryModel(0, "Custom Command", "", "", Rundown::CUSTOMCOMMAND, 0)));
}

void EventManager::fireAddPresetItemEvent(const QString& preset)
{
    // Use synchronous event through sendEvent().
    AddPresetItemEvent event(preset);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireStatusbarEvent(const QString& message, int timeout)
{
    // Use synchronous event through sendEvent().
    StatusbarEvent event(message, timeout);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireActiveRundownChangedEvent(const QString& path)
{
    qApp->postEvent(qApp, new ActiveRundownChangedEvent(path));
}

void EventManager::fireRundownItemSelectedEvent(AbstractCommand* command, LibraryModel* model)
{
    // Use synchronous event through sendEvent().
    RundownItemSelectedEvent event(command, model);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireLibraryItemSelectedEvent(AbstractCommand* command, LibraryModel* model)
{
    // Use synchronous event through sendEvent().
    LibraryItemSelectedEvent event(command, model);
    qApp->sendEvent(qApp, &event);
}

void EventManager::fireAutoRefreshLibraryEvent(bool autoRefresh, int interval)
{
    qApp->postEvent(qApp, new AutoRefreshLibraryEvent(autoRefresh, interval));
}

void EventManager::fireMediaChangedEvent()
{
    qApp->postEvent(qApp, new MediaChangedEvent());
}

void EventManager::fireTemplateChangedEvent()
{
    qApp->postEvent(qApp, new TemplateChangedEvent());
}

void EventManager::fireDataChangedEvent()
{
    qApp->postEvent(qApp, new DataChangedEvent());
}

void EventManager::firePresetChangedEvent()
{
    qApp->postEvent(qApp, new PresetChangedEvent());
}

void EventManager::firePreviewEvent()
{
    qApp->postEvent(qApp, new PreviewEvent());
}

void EventManager::fireLabelChangedEvent(const QString& label)
{
    qApp->postEvent(qApp, new LabelChangedEvent(label));
}

void EventManager::fireTargetChangedEvent(const QString& target)
{
    qApp->postEvent(qApp, new TargetChangedEvent(target));
}

void EventManager::fireDeviceChangedEvent(const QString& deviceName)
{
    qApp->postEvent(qApp, new DeviceChangedEvent(deviceName));
}

void EventManager::fireChannelChangedEvent(int channel)
{
    qApp->postEvent(qApp, new ChannelChangedEvent(channel));
}

void EventManager::fireVideolayerChangedEvent(int videolayer)
{
    qApp->postEvent(qApp, new VideolayerChangedEvent(videolayer));
}
