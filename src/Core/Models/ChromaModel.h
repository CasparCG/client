#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT ChromaModel
{
    public:
        explicit ChromaModel(int id, const QString& key);

        int getId() const;
        const QString& getKey() const;

    private:
        int id;
        QString key;
};
