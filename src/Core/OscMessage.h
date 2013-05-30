#pragma once

#include "Shared.h"

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QVariant>

class CORE_EXPORT OscMessage : public QObject
{
    Q_OBJECT

    public:
        explicit OscMessage(const QString& predicate, QObject* parent = 0);

        Q_SIGNAL void messageReceived(const QString&, const QList<QVariant>&);

    private:
        QRegExp predicate;

        Q_SLOT void oscMessageReceived(const QString&, const QString&, const QList<QVariant>&);
};
