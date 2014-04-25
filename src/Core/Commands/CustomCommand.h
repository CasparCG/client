#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QXmlStreamWriter>

class CORE_EXPORT CustomCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit CustomCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        const QString& getStopCommand() const;
        const QString& getPlayCommand() const;
        const QString& getLoadCommand() const;
        const QString& getPauseCommand() const;
        const QString& getNextCommand() const;
        const QString& getUpdateCommand() const;
        const QString& getInvokeCommand() const;
        const QString& getPreviewCommand() const;
        const QString& getClearCommand() const;
        const QString& getClearVideolayerCommand() const;
        const QString& getClearChannelCommand() const;
        bool getTriggerOnNext() const;

        void setStopCommand(const QString& command);
        void setPlayCommand(const QString& command);
        void setLoadCommand(const QString& command);
        void setPauseCommand(const QString& command);
        void setNextCommand(const QString& command);
        void setUpdateCommand(const QString& command);
        void setInvokeCommand(const QString& command);
        void setPreviewCommand(const QString& command);
        void setClearCommand(const QString& command);
        void setClearVideolayerCommand(const QString& command);
        void setClearChannelCommand(const QString& command);
        void setTriggerOnNext(bool triggerOnNext);

    private:
        QString stopCommand;
        QString playCommand;
        QString loadCommand;
        QString pauseCommand;
        QString nextCommand;
        QString updateCommand;
        QString invokeCommand;
        QString previewCommand;
        QString clearCommand;
        QString clearVideolayerCommand;
        QString clearChannelCommand;
        bool triggerOnNext;

        Q_SIGNAL void stopCommandChanged(const QString&);
        Q_SIGNAL void playCommandChanged(const QString&);
        Q_SIGNAL void loadCommandChanged(const QString&);
        Q_SIGNAL void pauseCommandChanged(const QString&);
        Q_SIGNAL void nextCommandChanged(const QString&);
        Q_SIGNAL void updateCommandChanged(const QString&);
        Q_SIGNAL void invokeCommandChanged(const QString&);
        Q_SIGNAL void previewCommandChanged(const QString&);
        Q_SIGNAL void clearCommandChanged(const QString&);
        Q_SIGNAL void clearVideolayerCommandChanged(const QString&);
        Q_SIGNAL void clearChannelCommandChanged(const QString&);
        Q_SIGNAL void triggerOnNextChanged(bool);
};
