/*
Copyright 2015  Peter Simonsson <peter.simonsson@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DOWNSTREAMKEY_H
#define DOWNSTREAMKEY_H

#include <QObject>
#include "libqatemcontrol_global.h"

class QAtemConnection;

class LIBQATEMCONTROLSHARED_EXPORT QAtemDownstreamKey : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool onAir READ onAir WRITE setOnAir NOTIFY onAirChanged)
    Q_PROPERTY(bool tie READ tie WRITE setTie NOTIFY tieChanged)
    Q_PROPERTY(quint8 frameRate READ frameRate WRITE setFrameRate NOTIFY frameRateChanged)
    Q_PROPERTY(quint8 frameCount READ frameCount NOTIFY frameCountChanged)
    Q_PROPERTY(quint16 fillSource READ fillSource WRITE setFillSource NOTIFY sourcesChanged)
    Q_PROPERTY(quint16 keySource READ keySource WRITE setKeySource NOTIFY sourcesChanged)
    Q_PROPERTY(bool invertKey READ invertKey WRITE setInvertKey NOTIFY invertKeyChanged)
    Q_PROPERTY(bool preMultiplied READ preMultiplied WRITE setPreMultiplied NOTIFY preMultipliedChanged)
    Q_PROPERTY(float clip READ clip WRITE setClip NOTIFY clipChanged)
    Q_PROPERTY(float gain READ gain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(bool enableMask READ enableMask WRITE setEnableMask NOTIFY enableMaskChanged)

public:
    explicit QAtemDownstreamKey(quint8 id, QAtemConnection *parent);
    ~QAtemDownstreamKey();

    bool onAir() const { return m_onAir; }
    bool inTransition() const { return m_inTransition; }
    bool inAutoTransition() const { return m_inAutoTransition; }
    bool tie() const { return m_tie; }
    quint8 frameRate() const { return m_frameRate; }
    quint8 frameCount() const { return m_frameCount; }
    quint16 fillSource() const { return m_fillSource; }
    quint16 keySource() const { return m_keySource; }
    bool invertKey() const { return m_invertKey; }
    bool preMultiplied() const { return m_preMultiplied; }
    float clip() const { return m_clip; }
    float gain() const { return m_gain; }
    bool enableMask() const { return m_enableMask; }
    float topMask() const { return m_topMask; }
    float bottomMask() const { return m_bottomMask; }
    float leftMask() const { return m_leftMask; }
    float rightMask() const { return m_rightMask; }

public slots:
    void setOnAir(bool state);
    void setTie(bool state);
    void setFrameRate(quint8 frames);
    void setFillSource(quint16 source);
    void setKeySource(quint16 source);
    void setInvertKey(bool invert);
    void setPreMultiplied(bool preMultiplied);
    void setClip(float clip);
    void setGain(float gain);
    void setEnableMask(bool enabled);
    void setMask(float top, float bottom, float left, float right);

    void doAuto();

protected slots:
    void onDskS(const QByteArray &payload);
    void onDskP(const QByteArray &payload);
    void onDskB(const QByteArray &payload);

private:
    quint8 m_id;

    bool m_onAir;
    bool m_inTransition;
    bool m_inAutoTransition;
    bool m_tie;
    quint8 m_frameRate;
    quint8 m_frameCount;
    quint16 m_fillSource;
    quint16 m_keySource;
    bool m_invertKey;
    bool m_preMultiplied;
    float m_clip;
    float m_gain;
    bool m_enableMask;
    float m_topMask;
    float m_bottomMask;
    float m_leftMask;
    float m_rightMask;

    QAtemConnection *m_atemConnection;

signals:
    void onAirChanged(quint8 keyer, bool state);
    void inTransitionChanged(quint8 keyer, bool state);
    void inAutoTransitionChanged(quint8 keyer, bool state);
    void tieChanged(quint8 keyer, bool state);
    void frameCountChanged(quint8 keyer, quint8 count);
    void frameRateChanged(quint8 keyer, quint8 frames);
    void sourcesChanged(quint8 keyer, quint16 fill, quint16 key);
    void invertKeyChanged(quint8 keyer, bool invert);
    void preMultipliedChanged(quint8 keyer, bool preMultiplied);
    void clipChanged(quint8 keyer, float clip);
    void gainChanged(quint8 keyer, float gain);
    void enableMaskChanged(quint8 keyer, bool enable);
    void maskChanged(quint8 keyer, float top, float bottom, float left, float right);
};

#endif // DOWNSTREAMKEY_H
