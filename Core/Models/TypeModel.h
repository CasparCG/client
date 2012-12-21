#pragma once

#include "../Shared.h"

#include <QtCore/QString>

class CORE_EXPORT TypeModel
{
    public:
        explicit TypeModel(int id, const QString& name);

        int getId() const;
        const QString& getName() const;

        class ByName
        {
            public:
                ByName(const QString& name) : name(name) { }

                bool operator()(const TypeModel& model) const { return model.getName() == name; }

            private:
                QString name;
        };

    private:
        int id;
        QString name;
};


