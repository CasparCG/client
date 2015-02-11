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

#ifndef QATEMCAMERACONTROL_H
#define QATEMCAMERACONTROL_H

#include "qatemtypes.h"

#include <QObject>

class QAtemConnection;

class QAtemCameraControl : public QObject
{
    Q_OBJECT
public:
    explicit QAtemCameraControl(QAtemConnection *parent = 0);
    ~QAtemCameraControl();

    QAtem::Camera *camera(quint8 input) const { return m_cameras[input - 1]; }

public slots:
    void setFocus(quint8 input, quint16 focus);
    void activeAutoFocus(quint8 input);
    void setIris(quint8 input, quint16 iris);
    void setZoomSpeed(quint8 input, qint16 zoom);

    void setGain(quint8 input, quint16 gain);
    void setWhiteBalance(quint8 input, quint16 wb);
    void setShutter(quint8 input, quint16 shutter);

    void setLift(quint8 input, float r, float g, float b, float y);
    void setGamma(quint8 input, float r, float g, float b, float y);
    void setGain(quint8 input, float r, float g, float b, float y);
    void setContrast(quint8 input, quint8 contrast);
    void setLumMix(quint8 input, quint8 mix);
    void setHueSaturation(quint8 input, quint16 hue, quint8 saturation);

protected slots:
    void onCCdP(const QByteArray& payload);

private:
    QAtemConnection *m_atemConnection;

    QList<QAtem::Camera*> m_cameras;

signals:
    void cameraChanged(quint8 input);
};

#endif // QATEMCAMERACONTROL_H
