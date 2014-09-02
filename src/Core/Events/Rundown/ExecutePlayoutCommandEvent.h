#pragma once

#include "../../Shared.h"

#include "Global.h"

#if QT_VERSION >= 0x050000
#include <QKeyEvent>
#else
#include <QtGui/QKeyEvent>
#endif

class CORE_EXPORT ExecutePlayoutCommandEvent
{
    public:
        explicit ExecutePlayoutCommandEvent(QEvent::Type type, int key, Qt::KeyboardModifiers modifiers);

        QEvent::Type getType() const;
        int getKey() const;
        Qt::KeyboardModifiers getModifiers() const;

    private:
        QEvent::Type type;
        int key;
        Qt::KeyboardModifiers modifiers;
};
