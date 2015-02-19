/*
Copyright 2014  Peter Simonsson <peter.simonsson@gmail.com>

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

#ifndef QATEMMIXEFFECT_H
#define QATEMMIXEFFECT_H

#include <qatemconnection.h>
#include <qupstreamkeysettings.h>

#include <QObject>

class QColor;

class QAtemMixEffect : public QObject
{
    Q_OBJECT
public:
    explicit QAtemMixEffect(quint8 id, QAtemConnection *parent = 0);
    ~QAtemMixEffect();

    void createUpstreamKeyers(quint8 count);

    /// @returns the index of the input that is on program
    quint16 programInput() const { return m_programInput; }
    /// @returns the index of the input that is on preview
    quint16 previewInput() const { return m_previewInput; }

    /// @returns true if transition preview is enabled
    bool transitionPreviewEnabled() const { return m_transitionPreviewEnabled; }
    /// @returns number of frames left of transition
    quint8 transitionFrameCount() const { return m_transitionFrameCount; }
    /// @returns percent left of transition
    quint16 transitionPosition() const { return m_transitionPosition; }
    /// @returns keyers used on next transition. Bit 0 = Background, 1-4 = keys, only bit 0 and 1 available on TVS
    quint8 keyersOnNextTransition() const { return m_keyersOnNextTransition; }
    /// @returns index of selected transition style for next transition. Bit 0 = Mix, 1 = Dip, 2 = Wipe, 3 = DVE and 4 = Stinger, only bit 0-2 available on TVS
    quint8 nextTransitionStyle() const { return m_nextTransitionStyle; }
    /// @returns keyers used on current transition. Bit 0 = Background, 1-4 = keys, only bit 0 and 1 available on TVS
    quint8 keyersOnCurrentTransition() const { return m_keyersOnCurrentTransition; }
    /// @returns index of selected transition style for current transition. Bit 0 = Mix, 1 = Dip, 2 = Wipe, 3 = DVE and 4 = Stinger, only bit 0-2 available on TVS
    quint8 currentTransitionStyle() const { return m_currentTransitionStyle; }

    /// @returns true if fade to black is on.
    bool fadeToBlackEnabled() const { return m_fadeToBlackEnabled; }
    /// @returns true if fade to black is fading to/from black
    bool fadeToBlackFading() const { return m_fadeToBlackFading; }
    /// @returns number of frames left of fade to black transition.
    quint8 fadeToBlackFrameCount() const { return m_fadeToBlackFrameCount; }
    /// @returns duration in number of frames for the fade to black transition.
    quint8 fadeToBlackFrames() const { return m_fadeToBlackFrames; }

    /// @returns duration in number of frames for mix transition
    quint8 mixFrames() const { return m_mixFrames; }

    /// @returns duration in number of frames for dip transition
    quint8 dipFrames() const { return m_dipFrames; }
    /// @returns the source used for a dip transition
    quint16 dipSource() const { return m_dipSource; }

    /// @returns duration in number of frames for wipe transition
    quint8 wipeFrames() const { return m_wipeFrames; }
    /// @returns the border source index, used for wipe transition
    quint16 wipeBorderSource() const { return m_wipeBorderSource; }
    /// @returns border width for wipe transition
    quint16 wipeBorderWidth() const { return m_wipeBorderWidth; }
    /// @returns border softness for wipe transition
    quint16 wipeBorderSoftness() const { return m_wipeBorderSoftness; }
    /// @returns type of wipe transition
    quint8 wipeType() const { return m_wipeType; }
    /// @returns symmetry of wipe transition
    quint16 wipeSymmetry() const { return m_wipeSymmetry; }
    /// @returns x position of wipe transition
    quint16 wipeXPosition() const { return m_wipeXPosition; }
    /// @returns y position of wipe transition
    quint16 wipeYPosition() const { return m_wipeYPosition; }
    /// @returns true if wipe transition direction should be reversed
    bool wipeReverseDirection() const { return m_wipeReverseDirection; }
    /// @returns true if wipe transition direction should flip flop
    bool wipeFlipFlop() const { return m_wipeFlipFlop; }

    /// @returns duration in number of frames for DVE transition
    quint8 dveRate() const { return m_dveRate; }
    /**
     * @returns the selected effect for DVE transition
     *
     * Swosh:
     * 0 = Top left
     * 1 = Up
     * 2 = Top right
     * 3 = Left
     * 4 = Right
     * 5 = Bottom left
     * 6 = Down
     * 7 = Bottom right
     *
     * Spin:
     * 8 = Down left, clockwise
     * 9 = Up left, clockwise
     * 10 = Down right, clockwise
     * 11 = Up right, clockwise
     * 12 = Up right, anti clockwise
     * 13 = Down right, anti clockwise
     * 14 = Up left, anti clockwise
     * 15 = Down left, anti clockwise
     *
     * Squeeze:
     * 16 = Top left
     * 17 = Up
     * 18 = Top right
     * 19 = Left
     * 20 = Right
     * 21 = Bottom left
     * 22 = Down
     * 23 = Bottom right
     *
     * Push:
     * 24 = Top left
     * 25 = Up
     * 26 = Top right
     * 27 = Left
     * 28 = Right
     * 29 = Bottom left
     * 30 = Down
     * 31 = Bottom right
     *
     * Graphic:
     * 32 = Spin clockwise
     * 33 = Spin anti clockwise
     * 34 = Logo wipe
     */
    quint8 dveEffect() const { return m_dveEffect; }
    quint16 dveFillSource() const { return m_dveFillSource; }
    quint16 dveKeySource() const { return m_dveKeySource; }
    bool dveKeyEnabled() const { return m_dveKeyEnabled; }
    bool dvePreMultipliedKeyEnabled() const { return m_dvePreMultipliedKeyEnabled; }
    /// @returns the clip of the key in per cent for the DVE transition
    float dveKeyClip() const { return m_dveKeyClip; }
    /// @returns the gain of the key in per cent for the DVE transition
    float dveKeyGain() const { return m_dveKeyGain; }
    bool dveInvertKeyEnabled() const { return m_dveEnableInvertKey; }
    bool dveReverseDirection() const { return m_dveReverseDirection; }
    bool dveFlipFlopDirection() const { return m_dveFlipFlopDirection; }

    /// @returns source used for the Stinger transition. 1 = Media player 1, 2 = Media player 2
    quint8 stingerSource() const { return m_stingerSource; }
    /// @returns true if the Stinger transition has a pre multiplied key
    bool stingerPreMultipliedKeyEnabled() const { return m_stingerPreMultipliedKeyEnabled; }
    float stingerClip() const { return m_stingerClip; }
    float stingerGain() const { return m_stingerGain; }
    bool stingerInvertKeyEnabled() const { return m_stingerInvertKeyEnabled; }
    quint16 stingerPreRoll() const { return m_stingerPreRoll; }
    quint16 stingerClipDuration() const { return m_stingerClipDuration; }
    quint16 stingerTriggerPoint() const { return m_stingerTriggerPoint; }
    quint16 stingerMixRate() const { return m_stingerMixRate; }

    /// @returns number of upstream keys available on this M/E
    quint8 upstreamKeyCount() const { return m_upstreamKeys.count(); }
    /// @returns true if upstream key @p keyer is on air
    bool upstreamKeyOnAir(quint8 keyer) const;
    /// @returns the key type for upstream key @p keyer, 0 = luma, 1 = chroma, 2 = pattern, 3 = DVE
    quint8 upstreamKeyType(quint8 keyer) const { return m_upstreamKeys[keyer]->m_type; }
    /// @returns the source used as fill for upstream key @p keyer
    quint16 upstreamKeyFillSource(quint8 keyer) const { return m_upstreamKeys[keyer]->m_fillSource; }
    /// @returns the source used as key for upstream key @p keyer
    quint16 upstreamKeyKeySource(quint8 keyer) const { return m_upstreamKeys[keyer]->m_keySource; }
    /// @returns true if the mask is enabled for upstream key @p keyer
    bool upstreamKeyEnableMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_enableMask; }
    /// @returns top mask for upstream key @p keyer
    float upstreamKeyTopMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_topMask; }
    /// @returns bottom mask for upstream key @p keyer
    float upstreamKeyBottomMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_bottomMask; }
    /// @returns left mask for upstream key @p keyer
    float upstreamKeyLeftMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_leftMask; }
    /// @returns right mask for upstream key @p keyer
    float upstreamKeyRightMask(quint8 keyer) const { return m_upstreamKeys[keyer]->m_rightMask; }
    /// @returns true if the key is pre multiplied for luma upstream key @p keyer
    bool upstreamKeyLumaPreMultipliedKey(quint8 keyer) const { return m_upstreamKeys[keyer]->m_lumaPreMultipliedKey; }
    /// @returns true if the key source should be inverted for luma upstream key @p keyer
    bool upstreamKeyLumaInvertKey(quint8 keyer) const { return m_upstreamKeys[keyer]->m_lumaInvertKey; }
    /// @returns clip for luma upstream key @p keyer
    float upstreamKeyLumaClip(quint8 keyer) const { return m_upstreamKeys[keyer]->m_lumaClip; }
    /// @returns gain for luma upstream key @p keyer
    float upstreamKeyLumaGain(quint8 keyer) const { return m_upstreamKeys[keyer]->m_lumaGain; }
    /// @returns hue for chroma upstream key @p keyer
    float upstreamKeyChromaHue(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaHue; }
    /// @returns gain for chroma upstream key @p keyer
    float upstreamKeyChromaGain(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaGain; }
    /// @returns y suppress for chroma upstream key @p keyer
    float upstreamKeyChromaYSuppress(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaYSuppress; }
    /// @returns lift for chroma upstream key @p keyer
    float upstreamKeyChromaLift(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaLift; }
    /// @returns true if chroma upstream key @p keyer should have narrow chroma key range
    bool upstreamKeyChromaNarrowRange(quint8 keyer) const { return m_upstreamKeys[keyer]->m_chromaNarrowRange; }
    /// @returns pattern of pattern upstream key @p keyer
    quint8 upstreamKeyPatternPattern(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternPattern; }
    /// @returns true if pattern upstream key @p keyer should invert the pattern
    bool upstreamKeyPatternInvertPattern(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternInvertPattern; }
    /// @returns size for pattern upstream key @p keyer
    float upstreamKeyPatternSize(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternSize; }
    /// @returns symmetry for pattern upstream key @p keyer
    float upstreamKeyPatternSymmetry(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternSymmetry; }
    /// @returns softness for pattern upstream key @p keyer
    float upstreamKeyPatternSoftness(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternSoftness; }
    /// @returns x position for pattern upstream key @p keyer
    float upstreamKeyPatternXPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternXPosition; }
    /// @returns y position for pattern upstream key @p keyer
    float upstreamKeyPatternYPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_patternYPosition; }
    /// @returns x position of DVE for upstream key @p keyer
    float upstreamKeyDVEXPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveXPosition; }
    /// @returns y position of DVE for upstream key @p keyer
    float upstreamKeyDVEYPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveYPosition; }
    /// @returns x size of DVE for upstream key @p keyer
    float upstreamKeyDVEXSize(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveXSize; }
    /// @returns y size of DVE for upstream key @p keyer
    float upstreamKeyDVEYSize(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveYSize; }
    /// @returns rotation of DVE for upstream key @p keyer
    float upstreamKeyDVERotation(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveRotation; }
    /// @returns true if the drop shadow is enabled on the DVE for upstream key @p keyer
    bool upstreamKeyDVEDropShadowEnabled(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveEnableDropShadow; }
    /// @returns direction of the light source for the drop shadow on the DVE for upstream key @p keyer
    float upstreamKeyDVELightSourceDirection(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveLightSourceDirection; }
    /// @returns altitude of the light source for the drop shadow on the DVE for upstream keu @p keyer
    quint8 upstreamKeyDVELightSourceAltitude(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveLightSourceAltitude; }
    /// @returns true if the border is enabled on the DVE for upstream key @p keyer
    bool upstreamKeyDVEBorderEnabled(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveEnableBorder; }
    /// @returns the border style of the DVE for upstream key @p keyer. 0 = No Bevel, 1 = Bevel In Out, 2 = Bevel In, 3 = Bevel Out
    quint8 upstreamKeyDVEBorderStyle(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderStyle; }
    /// @returns the border color of the DVE for upstream key @p keyer
    QColor upstreamKeyDVEBorderColor(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderColor; }
    /// @returns the outside width of the border of the DVE for upstream key @p keyer
    float upstreamKeyDVEBorderOutsideWidth(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderOutsideWidth; }
    /// @returns the inside width of the border of the DVE for upstream key @p keyer
    float upstreamKeyDVEBorderInsideWidth(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderInsideWidth; }
    /// @returns the outside soften (%) of the border of the DVE for upstream key @p keyer
    quint8 upstreamKeyDVEBorderOutsideSoften(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderOutsideSoften; }
    /// @returns the inside soften (%) of the border of the DVE for upstream key @p keyer
    quint8 upstreamKeyDVEBorderInsideSoften(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderInsideSoften; }
    /// @returns the opacity of the border of the DVE for upstream key @p keyer
    quint8 upstreamKeyDVEBorderOpacity(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderOpacity; }
    /// @returns the bevel position of the border of the DVE for upstream key @p keyer
    float upstreamKeyDVEBorderBevelPosition(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderBevelPosition; }
    /// @returns the bevel soften (%) of the border of the DVE for upstream key @p keyer
    quint8 upstreamKeyDVEBorderBevelSoften(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveBorderBevelSoften; }
    /// @returns the rate in frames the DVE for upstream key @p keyer runs at
    quint8 upstreamKeyDVERate(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveRate; }
    /// @returns true if key frame A has been set for the DVE for upstream key @p keyer
    bool upstreamKeyDVEKeyFrameASet(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveKeyFrameASet; }
    /// @returns true if key frame B has been set for the DVE for upstream key @p keyer
    bool upstreamKeyDVEKeyFrameBSet(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveKeyFrameBSet; }
    bool upstreamKeyDVEMaskEnabled(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskEnabled; }
    float upstreamKeyDVEMaskTop(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskTop; }
    float upstreamKeyDVEMaskBottom(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskBottom; }
    float upstreamKeyDVEMaskLeft(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskLeft; }
    float upstreamKeyDVEMaskRight(quint8 keyer) const { return m_upstreamKeys[keyer]->m_dveMaskRight; }
    /// @returns true if fly is enabled for non DVE type of the upstream key @p keyer
    bool upstreamKeyEnableFly(quint8 keyer) const { return m_upstreamKeys[keyer]->m_enableFly; }
    /// @returns the current key frame settings for keyer @p keyer and key frame @frame. 1 = KeyFrame A, 2 = KeyFrame B
    QAtem::DveKeyFrame upstreamKeyKeyFrame(quint8 keyer, quint8 frame) const { return m_upstreamKeys[keyer]->m_keyFrames[frame - 1]; }

public slots:
    void cut();
    void autoTransition();

    void changeProgramInput(quint16 index);
    void changePreviewInput(quint16 index);

    /// Set the current position of the transition to @p position. Set @p position to 0 to signal transition done.
    void setTransitionPosition(quint16 position);
    void setTransitionPreview(bool state);
    void setTransitionType(quint8 type);
    void setUpstreamKeyOnNextTransition(quint8 keyer, bool state);
    void setBackgroundOnNextTransition(bool state);

    void toggleFadeToBlack();
    void setFadeToBlackFrameRate(quint8 frames);

    void setMixFrames(quint8 frames);

    void setDipFrames(quint8 frames);
    void setDipSource(quint16 source);

    void setWipeBorderSource(quint16 source);
    void setWipeFrames(quint8 frames);
    void setWipeBorderWidth(quint16 width);
    void setWipeBorderSoftness(quint16 softness);
    void setWipeType(quint8 type);
    void setWipeSymmetry(quint16 value);
    void setWipeXPosition(quint16 value);
    void setWipeYPosition(quint16 value);
    void setWipeReverseDirection(bool reverse);
    void setWipeFlipFlop(bool flipFlop);

    void setDVERate(quint8 frames);
    /**
     * Set the effect used for the DVE transition to @p effect
     *
     * Swosh:
     * 0 = Top left
     * 1 = Up
     * 2 = Top right
     * 3 = Left
     * 4 = Right
     * 5 = Bottom left
     * 6 = Down
     * 7 = Bottom right
     *
     * Spin:
     * 8 = Down left, clockwise
     * 9 = Up left, clockwise
     * 10 = Down right, clockwise
     * 11 = Up right, clockwise
     * 12 = Up right, anti clockwise
     * 13 = Down right, anti clockwise
     * 14 = Up left, anti clockwise
     * 15 = Down left, anti clockwise
     *
     * Squeeze:
     * 16 = Top left
     * 17 = Up
     * 18 = Top right
     * 19 = Left
     * 20 = Right
     * 21 = Bottom left
     * 22 = Down
     * 23 = Bottom right
     *
     * Push:
     * 24 = Top left
     * 25 = Up
     * 26 = Top right
     * 27 = Left
     * 28 = Right
     * 29 = Bottom left
     * 30 = Down
     * 31 = Bottom right
     *
     * Graphic:
     * 32 = Spin clockwise
     * 33 = Spin anti clockwise
     * 34 = Logo wipe
     */
    void setDVEEffect(quint8 effect);
    void setDVEFillSource(quint16 source);
    void setDVEKeySource(quint16 source);
    void setDVEKeyEnabled(bool enabled);
    void setDVEPreMultipliedKeyEnabled(bool enabled);
    /// Set clip of key for DVE transition to @p percent
    void setDVEKeyClip(float percent);
    /// Set gain of key for DVE transition to @p percent
    void setDVEKeyGain(float percent);
    void setDVEInvertKeyEnabled(bool enabled);
    /// Set to true to do the DVE transition in the reverse direction
    void setDVEReverseDirection(bool reverse);
    /// Set to true to flip flop the direction of the DVE transition between uses
    void setDVEFlipFlopDirection(bool flipFlop);

    /// Set the source used for Stinger transition to @p source. 1 = Media player 1, 2 = Media player 2
    void setStingerSource(quint8 source);
    /// Enable if the key is pre multiplied in the source for the Stinger transition
    void setStingerPreMultipliedKeyEnabled(bool enabled);
    void setStingerClip(float percent);
    void setStingerGain(float percent);
    void setStingerInvertKeyEnabled(bool enabled);
    void setStingerPreRoll(quint16 frames);
    void setStingerClipDuration(quint16 frames);
    void setStingerTriggerPoint(quint16 frames);
    void setStingerMixRate(quint16 frames);

    void setUpstreamKeyOnAir(quint8 keyer, bool state);
    void setUpstreamKeyType(quint8 keyer, quint8 type);
    void setUpstreamKeyFillSource(quint8 keyer, quint16 source);
    void setUpstreamKeyKeySource(quint8 keyer, quint16 source);
    void setUpstreamKeyEnableMask(quint8 keyer, bool enable);
    void setUpstreamKeyMask(quint8 keyer, float top, float bottom, float left, float right);
    void setUpstreamKeyLumaPreMultipliedKey(quint8 keyer, bool preMultiplied);
    void setUpstreamKeyLumaInvertKey(quint8 keyer, bool invert);
    void setUpstreamKeyLumaClip(quint8 keyer, float clip);
    void setUpstreamKeyLumaGain(quint8 keyer, float gain);
    void setUpstreamKeyChromaHue(quint8 keyer, float hue);
    void setUpstreamKeyChromaGain(quint8 keyer, float gain);
    void setUpstreamKeyChromaYSuppress(quint8 keyer, float ySuppress);
    void setUpstreamKeyChromaLift(quint8 keyer, float lift);
    void setUpstreamKeyChromaNarrowRange(quint8 keyer, bool narrowRange);
    void setUpstreamKeyPatternPattern(quint8 keyer, quint8 pattern);
    void setUpstreamKeyPatternInvertPattern(quint8 keyer, bool invert);
    void setUpstreamKeyPatternSize(quint8 keyer, float size);
    void setUpstreamKeyPatternSymmetry(quint8 keyer, float symmetry);
    void setUpstreamKeyPatternSoftness(quint8 keyer, float softness);
    void setUpstreamKeyPatternXPosition(quint8 keyer, float xPosition);
    void setUpstreamKeyPatternYPosition(quint8 keyer, float yPosition);
    void setUpstreamKeyDVEPosition(quint8 keyer, float xPosition, float yPosition);
    void setUpstreamKeyDVESize(quint8 keyer, float xSize, float ySize);
    void setUpstreamKeyDVERotation(quint8 keyer, float rotation);
    void setUpstreamKeyDVELightSource(quint8 keyer, float direction, quint8 altitude);
    void setUpstreamKeyDVEDropShadowEnabled(quint8 keyer, bool enabled);
    void setUpstreamKeyDVEBorderEnabled(quint8 keyer, bool enabled);
    /// Set the border style of the upstream key DVE. 0 = No Bevel, 1 = Bevel In Out, 2 = Bevel In, 3 = Bevel Out
    void setUpstreamKeyDVEBorderStyle(quint8 keyer, quint8 style);
    void setUpstreamKeyDVEBorderColorH(quint8 keyer, float h);
    void setUpstreamKeyDVEBorderColorS(quint8 keyer, float s);
    void setUpstreamKeyDVEBorderColorL(quint8 keyer, float l);
    void setUpstreamKeyDVEBorderColor(quint8 keyer, const QColor& color);
    void setUpstreamKeyDVEBorderWidth(quint8 keyer, float outside, float inside);
    void setUpstreamKeyDVEBorderSoften(quint8 keyer, quint8 outside, quint8 inside);
    void setUpstreamKeyDVEBorderOpacity(quint8 keyer, quint8 opacity);
    void setUpstreamKeyDVEBorderBevelPosition(quint8 keyer, float position);
    void setUpstreamKeyDVEBorderBevelSoften(quint8 keyer, float soften);
    void setUpstreamKeyDVERate(quint8 keyer, quint8 rate);
    /// Set the @p keyFrame of the DVE for upstream keyer @p keyer. 1 = Keyframe A, 2 = Keyframe B
    void setUpstreamKeyDVEKeyFrame(quint8 keyer, quint8 keyFrame);
    /**
     * Make the upstream key @p keyer run to @p position. 1 = Keyframe A, 2 = Keyframe B, 3 = Fullscreen, 4 = Infinite
     * If the @p position = infinite there is also a @p direction.
     * Available directions:
     * 0 = Center
     * 1 = Top left
     * 2 = Up
     * 3 = Top right
     * 4 = Left
     * 5 = Center
     * 6 = Right
     * 7 = Bottom left
     * 8 = Down
     * 9 = Bottom right
     */
    void runUpstreamKeyTo(quint8 keyer, quint8 position, quint8 direction);
    /// Enable fly on the non DVE key types of the upstream key @p keyer
    void setUpstreamKeyFlyEnabled(quint8 keyer, bool enable);
    void setUpstreamKeyDVEMaskEnabled(quint8 keyer, bool enable);
    void setUpstreamKeyDVEMask(quint8 keyer, float top, float bottom, float left, float right);

protected slots:
    void onPrgI(const QByteArray& payload);
    void onPrvI(const QByteArray& payload);

    void onTrPr(const QByteArray& payload);
    void onTrPs(const QByteArray& payload);
    void onTrSS(const QByteArray& payload);

    void onFtbS(const QByteArray& payload);
    void onFtbP(const QByteArray& payload);

    void onTMxP(const QByteArray& payload);

    void onTDpP(const QByteArray& payload);

    void onTWpP(const QByteArray& payload);

    void onTDvP(const QByteArray& payload);

    void onTStP(const QByteArray& payload);

    void onKeOn(const QByteArray& payload);
    void onKeBP(const QByteArray& payload);
    void onKeLm(const QByteArray& payload);
    void onKeCk(const QByteArray& payload);
    void onKePt(const QByteArray& payload);
    void onKeDV(const QByteArray& payload);
    void onKeFS(const QByteArray& payload);
    void onKKFP(const QByteArray& payload);

protected:
    void setKeyOnNextTransition (int index, bool state);

private:
    quint8 m_id;
    QAtemConnection *m_atemConnection;

    quint16 m_programInput;
    quint16 m_previewInput;

    bool m_transitionPreviewEnabled;
    quint8 m_transitionFrameCount;
    quint16 m_transitionPosition;
    quint8 m_keyersOnCurrentTransition;
    quint8 m_currentTransitionStyle;
    quint8 m_keyersOnNextTransition;
    quint8 m_nextTransitionStyle;

    bool m_fadeToBlackEnabled;
    bool m_fadeToBlackFading;
    quint8 m_fadeToBlackFrameCount;
    quint8 m_fadeToBlackFrames;

    quint8 m_mixFrames;

    quint8 m_dipFrames;
    quint16 m_dipSource;

    quint8 m_wipeFrames;
    quint16 m_wipeBorderSource;
    quint16 m_wipeBorderWidth;
    quint16 m_wipeBorderSoftness;
    quint8 m_wipeType;
    quint16 m_wipeSymmetry;
    quint16 m_wipeXPosition;
    quint16 m_wipeYPosition;
    bool m_wipeReverseDirection;
    bool m_wipeFlipFlop;

    quint8 m_dveRate;
    quint8 m_dveEffect;
    quint16 m_dveFillSource;
    quint16 m_dveKeySource;
    bool m_dveKeyEnabled;
    bool m_dvePreMultipliedKeyEnabled;
    float m_dveKeyClip;
    float m_dveKeyGain;
    bool m_dveEnableInvertKey;
    bool m_dveReverseDirection;
    bool m_dveFlipFlopDirection;

    quint8 m_stingerSource;
    bool m_stingerPreMultipliedKeyEnabled;
    float m_stingerClip;
    float m_stingerGain;
    bool m_stingerInvertKeyEnabled;
    quint16 m_stingerPreRoll;
    quint16 m_stingerClipDuration;
    quint16 m_stingerTriggerPoint;
    quint16 m_stingerMixRate;

    QVector<QUpstreamKeySettings*> m_upstreamKeys;

signals:
    void programInputChanged(quint8 me, quint16 oldIndex, quint16 newIndex);
    void previewInputChanged(quint8 me, quint16 oldIndex, quint16 newIndex);

    void transitionPreviewChanged(quint8 me, bool state);
    void transitionFrameCountChanged(quint8 me, quint8 count);
    void transitionPositionChanged(quint8 me, quint16 count);
    void nextTransitionStyleChanged(quint8 me, quint8 style);
    void keyersOnNextTransitionChanged(quint8 me, quint8 keyers);
    void currentTransitionStyleChanged(quint8 me, quint8 style);
    void keyersOnCurrentTransitionChanged(quint8 me, quint8 keyers);

    ///@p fading is true while fading to/from black, @p enabled is true when program is faded to black
    void fadeToBlackChanged(quint8 me, bool fading, bool enabled);
    void fadeToBlackFrameCountChanged(quint8 me, quint8 count);
    void fadeToBlackFramesChanged(quint8 me, quint8 frames);

    void mixFramesChanged(quint8 me, quint8 frames);

    void dipFramesChanged(quint8 me, quint8 frames);
    void dipSourceChanged(quint8 me, quint16 source);

    void wipeFramesChanged(quint8 me, quint8 frames);
    void wipeBorderWidthChanged(quint8 me, quint16 width);
    void wipeBorderSoftnessChanged(quint8 me, quint16 softness);
    void wipeTypeChanged(quint8 me, quint8 type);
    void wipeSymmetryChanged(quint8 me, quint16 value);
    void wipeXPositionChanged(quint8 me, quint16 value);
    void wipeYPositionChanged(quint8 me, quint16 value);
    void wipeReverseDirectionChanged(quint8 me, bool reverse);
    void wipeFlipFlopChanged(quint8 me, bool flipFlop);
    void wipeBorderSourceChanged(quint8 me, quint16 index);

    void dveRateChanged(quint8 me, quint16 frames);
    void dveEffectChanged(quint8 me, quint8 effect);
    void dveFillSourceChanged(quint8 me, quint16 source);
    void dveKeySourceChanged(quint8 me, quint16 source);
    void dveEnableKeyChanged(quint8 me, bool enabled);
    void dveEnablePreMultipliedKeyChanged(quint8 me, bool enabled);
    void dveKeyClipChanged(quint8 me, float clip);
    void dveKeyGainChanged(quint8 me, float gain);
    void dveEnableInvertKeyChanged(quint8 me, bool enabled);
    void dveReverseDirectionChanged(quint8 me, bool reverse);
    void dveFlipFlopDirectionChanged(quint8 me, bool flipFlop);

    void stingerSourceChanged(quint8 me, quint8 source);
    void stingerEnablePreMultipliedKeyChanged(quint8 me, bool enabled);
    void stingerClipChanged(quint8 me, float percent);
    void stingerGainChanged(quint8 me, float percent);
    void stingerEnableInvertKeyChanged(quint8 me, bool enabled);
    void stingerPreRollChanged(quint8 me, quint16 frames);
    void stingerClipDurationChanged(quint8 me, quint16 frames);
    void stingerTriggerPointChanged(quint8 me, quint16 frames);
    void stingerMixRateChanged(quint8 me, quint16 frames);

    void upstreamKeyOnAirChanged(quint8 me, quint8 keyer, bool state);
    void upstreamKeyTypeChanged(quint8 me, quint8 keyer, quint8 type);
    void upstreamKeyFillSourceChanged(quint8 me, quint8 keyer, quint16 source);
    void upstreamKeyKeySourceChanged(quint8 me, quint8 keyer, quint16 source);
    void upstreamKeyEnableMaskChanged(quint8 me, quint8 keyer, bool enable);
    void upstreamKeyTopMaskChanged(quint8 me, quint8 keyer, float value);
    void upstreamKeyBottomMaskChanged(quint8 me, quint8 keyer, float value);
    void upstreamKeyLeftMaskChanged(quint8 me, quint8 keyer, float value);
    void upstreamKeyRightMaskChanged(quint8 me, quint8 keyer, float value);
    void upstreamKeyLumaPreMultipliedKeyChanged(quint8 me, quint8 keyer, bool preMultiplied);
    void upstreamKeyLumaInvertKeyChanged(quint8 me, quint8 keyer, bool invert);
    void upstreamKeyLumaClipChanged(quint8 me, quint8 keyer, float clip);
    void upstreamKeyLumaGainChanged(quint8 me, quint8 keyer, float gain);
    void upstreamKeyChromaHueChanged(quint8 me, quint8 keyer, float hue);
    void upstreamKeyChromaGainChanged(quint8 me, quint8 keyer, float gain);
    void upstreamKeyChromaYSuppressChanged(quint8 me, quint8 keyer, float ySuppress);
    void upstreamKeyChromaLiftChanged(quint8 me, quint8 keyer, float lift);
    void upstreamKeyChromaNarrowRangeChanged(quint8 me, quint8 keyer, bool narrowRange);
    void upstreamKeyPatternPatternChanged(quint8 me, quint8 keyer, quint8 pattern);
    void upstreamKeyPatternInvertPatternChanged(quint8 me, quint8 keyer, bool invert);
    void upstreamKeyPatternSizeChanged(quint8 me, quint8 keyer, float size);
    void upstreamKeyPatternSymmetryChanged(quint8 me, quint8 keyer, float symmetry);
    void upstreamKeyPatternSoftnessChanged(quint8 me, quint8 keyer, float softness);
    void upstreamKeyPatternXPositionChanged(quint8 me, quint8 keyer, float xPosition);
    void upstreamKeyPatternYPositionChanged(quint8 me, quint8 keyer, float yPosition);
    void upstreamKeyDVEXPositionChanged(quint8 me, quint8 keyer, float xPosition);
    void upstreamKeyDVEYPositionChanged(quint8 me, quint8 keyer, float yPosition);
    void upstreamKeyDVEXSizeChanged(quint8 me, quint8 keyer, float xSize);
    void upstreamKeyDVEYSizeChanged(quint8 me, quint8 keyer, float ySize);
    void upstreamKeyDVERotationChanged(quint8 me, quint8 keyer, float rotation);
    void upstreamKeyDVEEnableDropShadowChanged(quint8 me, quint8 keyer, bool enable);
    void upstreamKeyDVELighSourceDirectionChanged(quint8 me, quint8 keyer, float direction);
    void upstreamKeyDVELightSourceAltitudeChanged(quint8 me, quint8 keyer, quint8 altitude);
    void upstreamKeyDVEEnableBorderChanged(quint8 me, quint8 keyer, bool enable);
    void upstreamKeyDVEBorderStyleChanged(quint8 me, quint8 keyer, quint8 style);
    void upstreamKeyDVEBorderColorChanged(quint8 me, quint8 keyer, QColor color);
    void upstreamKeyDVEBorderOutsideWidthChanged(quint8 me, quint8 keyer, float width);
    void upstreamKeyDVEBorderInsideWidthChanged(quint8 me, quint8 keyer, float width);
    void upstreamKeyDVEBorderOutsideSoftenChanged(quint8 me, quint8 keyer, quint8 soften);
    void upstreamKeyDVEBorderInsideSoftenChanged(quint8 me, quint8 keyer, quint8 soften);
    void upstreamKeyDVEBorderOpacityChanged(quint8 me, quint8 keyer, quint8 opacity);
    void upstreamKeyDVEBorderBevelPositionChanged(quint8 me, quint8 keyer, float position);
    void upstreamKeyDVEBorderBevelSoftenChanged(quint8 me, quint8 keyer, float soften);
    void upstreamKeyDVERateChanged(quint8 me, quint8 keyer, quint8 rate);
    void upstreamKeyDVEKeyFrameASetChanged(quint8 me, quint8 keyer, bool set);
    void upstreamKeyDVEKeyFrameBSetChanged(quint8 me, quint8 keyer, bool set);
    void upstreamKeyEnableFlyChanged(quint8 me, quint8 keyer, bool enabled);
    void upstreamKeyDVEKeyFrameChanged(quint8 me, quint8 keyer, quint8 frame);
    void upstreamKeyDVEMaskEnabledChanged(quint8 me, quint8 keyer, bool enabled);
    void upstreamKeyDVEMaskTopChanged(quint8 me, quint8 keyer, float top);
    void upstreamKeyDVEMaskBottomChanged(quint8 me, quint8 keyer, float bottom);
    void upstreamKeyDVEMaskLeftChanged(quint8 me, quint8 keyer, float left);
    void upstreamKeyDVEMaskRightChanged(quint8 me, quint8 keyer, float right);
};

#endif // QATEMMIXEFFECT_H
