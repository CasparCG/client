#pragma once

#include "../Shared.h"
#include "AbstractCommand.h"

#include "Global.h"

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <QtCore/QString>

class QObject;
class QXmlStreamWriter;

class CORE_EXPORT LevelsCommand : public AbstractCommand
{
    Q_OBJECT

    public:
        explicit LevelsCommand(QObject* parent = 0);

        virtual void readProperties(boost::property_tree::wptree& pt);
        virtual void writeProperties(QXmlStreamWriter* writer);

        float getMinIn() const;
        float getMaxIn() const;
        float getMinOut() const;
        float getMaxOut() const;
        float getGamma() const;
        int getTransitionDuration() const;
        const QString& getTween() const;
        bool getDefer() const;

        void setMinIn(float minIn);
        void setMaxIn(float maxIn);
        void setMinOut(float minOut);
        void setMaxOut(float maxOut);
        void setGamma(float gamma);
        void setTransitionDuration(int transtitionDuration);
        void setTween(const QString& tween);
        void setDefer(bool defer);

    private:
        float minIn = Mixer::DEFAULT_LEVELS_MIN_IN;
        float maxIn = Mixer::DEFAULT_LEVELS_MAX_IN;
        float minOut = Mixer::DEFAULT_LEVELS_MIN_OUT;
        float maxOut = Mixer::DEFAULT_LEVELS_MAX_OUT;
        float gamma = Mixer::DEFAULT_LEVELS_GAMMA;
        int transtitionDuration = Mixer::DEFAULT_DURATION;
        QString tween = Mixer::DEFAULT_TWEEN;
        bool defer = Mixer::DEFAULT_DEFER;

        Q_SIGNAL void minInChanged(float);
        Q_SIGNAL void maxInChanged(float);
        Q_SIGNAL void minOutChanged(float);
        Q_SIGNAL void maxOutChanged(float);
        Q_SIGNAL void gammaChanged(float);
        Q_SIGNAL void transtitionDurationChanged(int);
        Q_SIGNAL void tweenChanged(const QString&);
        Q_SIGNAL void deferChanged(bool);
};
