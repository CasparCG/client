/*
Copyright 2012  Peter Simonsson <peter.simonsson@gmail.com>

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

#ifndef QDOWNSTREAMKEYSETTINGS_H
#define QDOWNSTREAMKEYSETTINGS_H

#include <QtGlobal>

struct QDownstreamKeySettings
{
    QDownstreamKeySettings ()
    {
        m_onAir = false;
        m_tie = false;
        m_frames = 0;
        m_frameCount = 0;
        m_fillSource = 0;
        m_keySource = 0;
        m_invertKey = false;
        m_preMultiplied = false;
        m_clip = 0;
        m_gain = 0;
        m_enableMask = false;
        m_topMask = 0;
        m_bottomMask = 0;
        m_leftMask = 0;
        m_rightMask = 0;
    }

    bool m_onAir;
    bool m_tie;
    quint8 m_frames;
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
};

#endif //QDOWNSTREAMKEYSETTINGS_H
