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

#ifndef QATEMCONNECTION_H
#define QATEMCONNECTION_H

#include "qdownstreamkeysettings.h"
#include "qatemtypes.h"

#include <QObject>
#include <QUdpSocket>
#include <QColor>

class QTimer;
class QHostAddress;
class QAtemMixEffect;
class QAtemCameraControl;

class QAtemConnection : public QObject
{
    Q_OBJECT
friend class QAtemMixEffect;
friend class QAtemCameraControl;
public:
    enum Command
    {
        Cmd_NoCommand = 0x0,
        Cmd_AckRequest = 0x1,
        Cmd_HelloPacket = 0x2,
        Cmd_Resend = 0x4,
        Cmd_Undefined = 0x8,
        Cmd_Ack = 0x10
    };

    Q_DECLARE_FLAGS(Commands, Command)

    struct CommandHeader
    {
        quint8 bitmask;
        quint16 size;
        quint16 uid;
        quint16 ackId;
        quint16 packageId;

        CommandHeader()
        {
            bitmask = size = uid = ackId = packageId = 0;
        }
    };

    explicit QAtemConnection(QObject* parent = NULL);
    virtual ~QAtemConnection();

    QHostAddress address() const { return m_address; }

    bool isConnected() const;

    /// Connect to ATEM switcher at @p address
    void connectToSwitcher(const QHostAddress& address, int connectionTimeout = 1000);
    void disconnectFromSwitcher();

    void setDebugEnabled(bool enabled) { m_debugEnabled = enabled; }
    bool debugEnabled() const { return m_debugEnabled; }

    /// @returns the tally state of the input @p index. 1 = program, 2 = preview and 3 = both
    quint8 tallyByIndex(quint8 index) const;
    /// @returns number of tally indexes available
    int tallyIndexCount() const { return m_tallyByIndex.count(); }

    /// @returns number of tally channels available
    quint16 tallyChannelCount() const { return m_tallyChannelCount; }

    /// @returns true if downstream key @p keyer is on air
    bool downstreamKeyOn(quint8 keyer) const;
    /// @returns true if downstream key @p keyer is tied to next transition
    bool downstreamKeyTie(quint8 keyer) const;
    /// @returns number of frames left of key transition for @p keyer
    quint8 downstreamKeyFrameCount(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_frameCount; }
    /// @returns duration in number of frames for key transition of dsk @p keyer
    quint8 downstreamKeyFrames(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_frames; }
    /// @returns the input selected as fill source for downstream key @p keyer
    quint16 downstreamKeyFillSource(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_fillSource; }
    /// @returns the input selected as key source for downstream key @p keyer
    quint16 downstreamKeyKeySource(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_keySource; }
    /// @returns true if the should be inverted for downstream key @p keyer
    bool downstreamKeyInvertKey(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_invertKey; }
    /// @returns true if the key is pre multiplied for downstream key @p keyer
    bool downstreamKeyPreMultiplied(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_preMultiplied; }
    /// @returns the clip set for downstream key @p keyer
    quint16 downstreamKeyClip(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_clip; }
    /// @returns the gain set for downstream key @p keyer
    quint16 downstreamKeyGain(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_gain; }
    /// @returns true if the mask for downstream key @p keyer is enabled
    bool downstreamKeyEnableMask(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_enableMask; }
    /// @returns the top position of the mask for downstream key @p keyer
    qint16 downstreamKeyTopMask(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_topMask; }
    /// @returns the bottom position of the mask for downstream key @p keyer
    qint16 downstreamKeyBottomMask(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_bottomMask; }
    /// @returns the left position of the mask for downstream key @p keyer
    qint16 downstreamKeyLeftMask(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_leftMask; }
    /// @returns the right position of the mask for downstream key @p keyer
    qint16 downstreamKeyRightMask(quint8 keyer) const { return m_downstreamKeys.value(keyer).m_rightMask; }

    QColor colorGeneratorColor(quint8 generator) const;

    quint8 mediaPlayerType(quint8 player) const;
    quint8 mediaPlayerSelectedStill(quint8 player) const;
    quint8 mediaPlayerSelectedClip(quint8 player) const;
    /// @returns the current state of the media player @p player
    QAtem::MediaPlayerState mediaPlayerState(quint8 player) const { return m_mediaPlayerStates.value(player); }

    quint16 auxSource(quint8 aux) const;

    QString productInformation() const { return m_productInformation; }
    quint16 majorVersion() const { return m_majorversion; }
    quint16 minorVersion() const { return m_minorversion; }

    /// @returns Info about the input @p index
    QAtem::InputInfo inputInfo(quint16 index) const { return m_inputInfos.value(index); }
    QMap<quint16, QAtem::InputInfo> inputInfos () const { return m_inputInfos; }

    QAtem::MediaInfo stillMediaInfo(quint8 index) const { return m_stillMediaInfos.value(index); }
    QAtem::MediaInfo clipMediaInfo(quint8 index) const { return m_clipMediaInfos.value(index); }
    QAtem::MediaInfo soundMediaInfo(quint8 index) const { return m_soundMediaInfos.value(index); }

    quint8 multiViewCount() const { return m_multiViews.count(); }
    QAtem::MultiView *multiView(quint8 index) const;

    QMap<quint8, QAtem::VideoMode> availableVideoModes() const { return m_availableVideoModes; }
    /// @returns index of the video format in use. 0 = 525i5994, 1 = 625i50, 2 = 525i5994 16:9, 3 = 625i50 16:9, 4 = 720p50, 5 = 720p5994, 6 = 1080i50, 7 = 1080i5994
    quint8 videoFormat() const { return m_videoFormat; }
    /// @returns type of video down coversion, 0 = Center cut, 1 = Letterbox, 2 = Anamorphic
    quint8 videoDownConvertType() const { return m_videoDownConvertType; }

    /// @returns size of clip 1 in the media pool
    quint8 mediaPoolClip1Size() const { return m_mediaPoolClip1Size; }
    /// @returns size of clip 2 in the media pool
    quint8 mediaPoolClip2Size() const { return m_mediaPoolClip2Size; }

    /// @returns number of still banks in the media pool
    quint8 mediaPoolStillBankCount() const { return m_mediaPoolStillBankCount; }
    /// @returns total number of clip banks in the media pool
    quint8 mediaPoolClipBankCount() const { return m_mediaPoolClipBankCount; }

    /// @returns audio input info for input @p index
    QAtem::AudioInput audioInput(quint16 index) { return m_audioInputs.value(index); }
    QHash<quint16, QAtem::AudioInput> audioInputs() { return m_audioInputs; }
    /// @return audio tally state for audio input @p index
    bool audioTallyState(quint16 index) { return m_audioTally.value(index); }

    /// @returns true if the monitor function is enabled on the audio breakout cable.
    bool audioMonitorEnabled() const { return m_audioMonitorEnabled; }
    float audioMonitorGain() const { return m_audioMonitorGain; }
    bool audioMonitorMuted() const { return m_audioMonitorMuted; }
    bool audioMonitorDimmed() const { return m_audioMonitorDimmed; }
    /// @returns the audio channel that is solo on monitor out. -1 = None.
    qint8 audioMonitorSolo() const { return m_audioMonitorSolo; }
    float audioMonitorLevel() const { return m_audioMonitorLevel; }
    float audioMasterOutputGain() const { return m_audioMasterOutputGain; }

    QAtem::AudioLevel audioLevel(quint16 index) const { return m_audioLevels.value(index); }
    float audioMasterOutputLevelLeft() const { return m_audioMasterOutputLevelLeft;}
    float audioMasterOutputLevelRight() const { return m_audioMasterOutputLevelRight;}
    float audioMasterOutputPeakLeft() const { return m_audioMasterOutputPeakLeft; }
    float audioMasterOutputPeakRight() const { return m_audioMasterOutputPeakRight; }

    quint8 audioChannelCount() const { return m_audioChannelCount; }
    bool hasAudioMonitor() const { return m_hasAudioMonitor; }

    /// Aquire the media pool lock with ID @p id. @returns false if the lock is already locked.
    bool aquireMediaLock(quint8 id, quint8 index);
    /// Unlock the media pool lock with ID @p id.
    void unlockMediaLock(quint8 id);
    /// @returns the state of the media pool lock with ID @p id.
    bool mediaLockState(quint8 id) const { return m_mediaLocks.value(id); }

    void aquireLock(quint8 storeId);

    /**
     * @brief Send data to a store in the switcher.
     * @param storeId 0 = Still store, 1 = Clip store, 2 = Sound store, 3 = Multiview labels, 255 = Macros
     * @param index Index in the store
     * @param name Name shown to the user
     * @param data Actual pixel or sound data
     * @return Returns the ID of the data transfer if success else 0
     */
    quint16 sendDataToSwitcher(quint8 storeId, quint8 index, const QByteArray &name, const QByteArray &data);
    bool transferActive() const { return m_transferActive; }
    quint16 transferId () const { return m_transferId; }
    int remainingTransferDataSize() const { return m_transferData.size(); }
    quint16 getDataFromSwitcher(quint8 storeId, quint8 index);
    QByteArray transferData() const { return m_transferData; }

    /**
     * Convert @p image to a QByteArray suitable for sending to the switcher.
     * @param image The image to convert
     * @param width Width of the frame the image will be converted to
     * @param height Height of the frame the image will be converted to
     */
    static QByteArray prepImageForSwitcher(QImage &image, const int width, const int height);

    QAtem::Topology topology() const { return m_topology; }

    QAtemMixEffect *mixEffect(quint8 me) const;

    void registerCommand(const QByteArray &command, QObject *object, const QByteArray &slot);
    void unregisterCommand(const QByteArray &command, QObject *object);

    /// @returns the power status as a bitmask. Bit 0: Main power on/off, 1: Backup power on/off
    quint8 powerStatus() const { return m_powerStatus; }

    QAtemCameraControl *cameraControl() const { return m_cameraControl; }

    QAtem::MacroInfo macroInfo(quint8 index) const { return m_macroInfos.at(index); }
    QVector<QAtem::MacroInfo> macroInfos () const { return m_macroInfos; }

    QAtem::MacroRunningState macroRunningState() const { return m_macroRunningState; }
    bool macroRepeating() const { return m_macroRepeating; }
    quint8 runningMacro() const { return m_runningMacro; }
    bool macroRecording() const { return m_macroRecording; }
    quint8 recordingMacro() const { return m_recordingMacro; }

public slots:
    void setDownstreamKeyOn(quint8 keyer, bool state);
    void setDownstreamKeyTie(quint8 keyer, bool state);
    void doDownstreamKeyAuto(quint8 keyer);
    void setDownstreamKeyFillSource(quint8 keyer, quint16 source);
    void setDownstreamKeyKeySource(quint8 keyer, quint16 source);
    void setDownstreamKeyFrameRate(quint8 keyer, quint8 frames);
    void setDownstreamKeyInvertKey(quint8 keyer, bool invert);
    void setDownstreamKeyPreMultiplied(quint8 keyer, bool preMultiplied);
    void setDownstreamKeyClip(quint8 keyer, float clip);
    void setDownstreamKeyGain(quint8 keyer, float gain);
    void setDownstreamKeyEnableMask(quint8 keyer, bool enable);
    void setDownstreamKeyMask(quint8 keyer, float top, float bottom, float left, float right);

    void saveSettings();
    void clearSettings();

    void setColorGeneratorColor(quint8 generator, const QColor& color);

    void setMediaPlayerSource(quint8 player, bool clip, quint8 source);
    void setMediaPlayerLoop(quint8 player, bool loop);
    void setMediaPlayerPlay(quint8 player, bool play);
    void mediaPlayerGoToBeginning(quint8 player);
    void mediaPlayerGoFrameBackward(quint8 player);
    void mediaPlayerGoFrameForward(quint8 player);

    void setAuxSource(quint8 aux, quint16 source);

    /**
     * Set the type of input to use. 1 = SDI, 2 = HMDI, 4 = Component.
     * On TVS input 3 and 4 are selectable between HDMI and SDI.
     * On 1 M/E input 1 is selectable between HDMI and component.
     */
    void setInputType(quint16 input, quint8 type);
    void setInputLongName(quint16 input, const QString& name);
    void setInputShortName(quint16 input, const QString& name);

    void setVideoFormat(quint8 format);
    /// Set type of video down conversion to @p type. 0 = Center cut, 1 = Letterbox, 2 = Anamorphic
    void setVideoDownConvertType(quint8 type);

    /// Sets the size of media pool clip 1 to @p size, max is mediaPoolClipBankCount(). Clip 2 size will be mediaPoolClipBankCount() - @p size.
    void setMediaPoolClipSplit(quint16 size);

    /// Sets the layout of multi viewer @p multiView to @p layout.
    void setMultiViewLayout(quint8 multiView, quint8 layout);
    void setMultiViewInput(quint8 multiView, quint8 windowIndex, quint16 source);

    /// Set to true if you want audio data from the mixer
    void setAudioLevelsEnabled(bool enabled);
    /// Set the state of the audio input. 0 = Off, 1 = On, 2 = AFV
    void setAudioInputState(quint16 index, quint8 state);
    /// Set the balance of the audio input. @p balance is a value between -1.0 and +1.0.
    void setAudioInputBalance(quint16 index, float balance);
    /// Set the gain of the audio input @p index. @p left and @p right is between +6dB and -60dB (-infdB)
    void setAudioInputGain(quint16 index, float gain);
    /// Set the gain of the audio master output. @p left and @p right is between +6dB and -60dB (-infdB)
    void setAudioMasterOutputGain(float gain);
    /// Enables audio monitoring using the breakout cable.
    void setAudioMonitorEnabled(bool enabled);
    /// Set the gain of the audio monitor output. @p gain is between +6dB and -60dB (-infdB)
    void setAudioMonitorGain(float gain);
    void setAudioMonitorMuted(bool muted);
    void setAudioMonitorDimmed(bool dimmed);
    void setAudioMonitorSolo(qint8 solo);
    void resetAudioMasterOutputPeaks();
    void resetAudioInputPeaks(quint16 input);

    void runMacro(quint8 macroIndex);
    void setMacroRepeating(bool state);
    void startRecordingMacro(quint8 macroIndex, const QByteArray &name, const QByteArray &description);
    void stopRecordingMacro();
    void addMacroUserWait();
    void addMacroPause(quint32 frames);
    void setMacroName(quint8 macroIndex, const QByteArray &name);
    void setMacroDescription(quint8 macroIndex, const QByteArray &description);
    void removeMacro(quint8 macroIndex);
    void continueMacro();
    void stopMacro();

protected slots:
    void handleSocketData();

    void handleError(QAbstractSocket::SocketError);
    void handleConnectionTimeout();
    void emitConnectedSignal();

    void onTlIn(const QByteArray& payload);
    void onDskS(const QByteArray& payload);
    void onDskP(const QByteArray& payload);
    void onDskB(const QByteArray& payload);
    void onColV(const QByteArray& payload);
    void onMPCE(const QByteArray& payload);
    void onAuxS(const QByteArray& payload);
    void on_pin(const QByteArray& payload);
    void on_ver(const QByteArray& payload);
    void onInPr(const QByteArray& payload);
    void onMPSE(const QByteArray& payload);
    void onMPfe(const QByteArray& payload);
    void onMPCS(const QByteArray& payload);
    void onMvIn(const QByteArray& payload);
    void onMvPr(const QByteArray& payload);
    void onVidM(const QByteArray& payload);
    void onTime(const QByteArray& payload);
    void onDcOt(const QByteArray& payload);
    void onAMmO(const QByteArray& payload);
    void onMPSp(const QByteArray& payload);
    void onRCPS(const QByteArray& payload);
    void onAMLv(const QByteArray& payload);
    void onAMTl(const QByteArray& payload);
    void onAMIP(const QByteArray& payload);
    void onAMMO(const QByteArray& payload);
    void onLKST(const QByteArray& payload);
    void onFTCD(const QByteArray& payload);
    void onFTDC(const QByteArray& payload);
    void on_top(const QByteArray& payload);
    void onPowr(const QByteArray& payload);
    void onVMC(const QByteArray& payload);
    void onWarn(const QByteArray& payload);
    void on_mpl(const QByteArray& payload);
    void on_TlC(const QByteArray& payload);
    void onTlSr(const QByteArray& payload);
    void on_AMC(const QByteArray& payload);
    void onMPAS(const QByteArray& payload);
    void onMPfM(const QByteArray& payload);
    void onAuxP(const QByteArray& payload);
    void onMPrp(const QByteArray& payload);
    void onMRPr(const QByteArray& payload);
    void onMRcS(const QByteArray& payload);
    void on_MAC(const QByteArray& payload);
    void onFTDa(const QByteArray& payload);
    void onFTDE(const QByteArray& payload);
    void onLKOB(const QByteArray& payload);

    void initDownloadToSwitcher();
    void flushTransferBuffer(quint8 count);
    void acceptData();

protected:
    QByteArray createCommandHeader(Commands bitmask, quint16 payloadSize, quint16 uid, quint16 ackId);

    QAtemConnection::CommandHeader parseCommandHeader(const QByteArray& datagram) const;
    void parsePayLoad(const QByteArray& datagram);

    bool sendDatagram(const QByteArray& datagram);
    bool sendCommand(const QByteArray& cmd, const QByteArray &payload);

    void initCommandSlotHash();

    void sendData(quint16 id, const QByteArray &data);
    void sendFileDescription();
    void requestData();

    static float convertToDecibel(quint16 level);
    static quint16 convertFromDecibel(float level);

    void setInitialized(bool state);

private:
    struct ObjectSlot
    {
        ObjectSlot(QObject *o, const QByteArray &s) : object(o), slot(s) {}

        inline bool operator ==(const ObjectSlot &b) const
        {
            return (b.object == object && b.slot == slot);
        }

        QObject *object;
        QByteArray slot;
    };

    QUdpSocket* m_socket;
    QTimer* m_connectionTimer;

    QHostAddress m_address;
    quint16 m_port;

    quint16 m_packetCounter;
    bool m_isInitialized;
    quint16 m_currentUid;

    QMultiHash<QByteArray, ObjectSlot> m_commandSlotHash;

    bool m_debugEnabled;

    QVector<QAtemMixEffect*> m_mixEffects;

    QVector<quint8> m_tallyByIndex;

    quint16 m_tallyChannelCount;

    QHash<quint8, QDownstreamKeySettings> m_downstreamKeys;

    QHash<quint8, QColor> m_colorGeneratorColors;

    QHash<quint8, quint8> m_mediaPlayerType;
    QHash<quint8, quint8> m_mediaPlayerSelectedStill;
    QHash<quint8, quint8> m_mediaPlayerSelectedClip;
    QHash<quint8, QAtem::MediaPlayerState> m_mediaPlayerStates;

    QHash<quint8, quint16> m_auxSource;

    QString m_productInformation;
    quint16 m_majorversion;
    quint16 m_minorversion;

    QMap<quint16, QAtem::InputInfo> m_inputInfos;

    QHash<quint8, QAtem::MediaInfo> m_stillMediaInfos;
    QHash<quint8, QAtem::MediaInfo> m_clipMediaInfos;
    QHash<quint8, QAtem::MediaInfo> m_soundMediaInfos;

    QVector<QAtem::MultiView*> m_multiViews;

    quint8 m_videoFormat;
    quint8 m_videoDownConvertType;

    quint16 m_mediaPoolClip1Size;
    quint16 m_mediaPoolClip2Size;
    quint8 m_mediaPoolStillBankCount;
    quint8 m_mediaPoolClipBankCount;

    QHash<quint16, QAtem::AudioInput> m_audioInputs;
    QHash<quint16, bool> m_audioTally;
    QHash<quint16, QAtem::AudioLevel> m_audioLevels;

    bool m_audioMonitorEnabled;
    float m_audioMonitorGain;
    bool m_audioMonitorDimmed;
    bool m_audioMonitorMuted;
    qint8 m_audioMonitorSolo;
    float m_audioMonitorLevel;

    float m_audioMasterOutputLevelLeft;
    float m_audioMasterOutputLevelRight;
    float m_audioMasterOutputPeakLeft;
    float m_audioMasterOutputPeakRight;
    float m_audioMasterOutputGain;

    quint8 m_audioChannelCount;
    bool m_hasAudioMonitor;

    QHash<quint8, bool> m_mediaLocks;

    bool m_transferActive;
    QByteArray m_transferData;
    quint8 m_transferStoreId;
    quint8 m_transferIndex;
    QByteArray m_transferName;
    quint16 m_transferId;
    quint16 m_lastTransferId;
    QByteArray m_transferHash;

    QAtem::Topology m_topology;

    quint8 m_powerStatus;

    QMap<quint8, QAtem::VideoMode> m_availableVideoModes;

    QAtemCameraControl *m_cameraControl;

    QVector<QAtem::MacroInfo> m_macroInfos;
    QAtem::MacroRunningState m_macroRunningState;
    bool m_macroRepeating;
    quint8 m_runningMacro;
    bool m_macroRecording;
    quint8 m_recordingMacro;

signals:
    void connected();
    void disconnected();
    void socketError(const QString& errorString);

    void switcherWarning(const QString &warningString);

    void tallyStatesChanged();

    void downstreamKeyOnChanged(quint8 keyer, bool state);
    void downstreamKeyTieChanged(quint8 keyer, bool state);
    void downstreamKeyFrameCountChanged(quint8 keyer, quint8 count);
    void downstreamKeyFramesChanged(quint8 keyer, quint8 frames);
    void downstreamKeySourcesChanged(quint8 keyer, quint16 fill, quint16 key);
    void downstreamKeyInvertKeyChanged(quint8 keyer, bool invert);
    void downstreamKeyPreMultipliedChanged(quint8 keyer, bool preMultiplied);
    void downstreamKeyClipChanged(quint8 keyer, float clip);
    void downstreamKeyGainChanged(quint8 keyer, float gain);
    void downstreamKeyEnableMaskChanged(quint8 keyer, bool enable);
    void downstreamKeyTopMaskChanged(quint8 keyer, float value);
    void downstreamKeyBottomMaskChanged(quint8 keyer, float value);
    void downstreamKeyLeftMaskChanged(quint8 keyer, float value);
    void downstreamKeyRightMaskChanged(quint8 keyer, float value);

    void colorGeneratorColorChanged(quint8 generator, const QColor& color);

    void mediaPlayerChanged(quint8 player, quint8 type, quint8 still, quint8 clip);
    void mediaPlayerStateChanged(quint8 player, const QAtem::MediaPlayerState& state);

    void auxSourceChanged(quint8 aux, quint16 source);

    void inputInfoChanged(const QAtem::InputInfo& info);

    void mediaInfoChanged(const QAtem::MediaInfo& info);

    void productInformationChanged(const QString& info);
    void versionChanged(quint16 major, quint16 minor);

    void timeChanged(quint32 time);

    void videoFormatChanged(quint8 format);
    void videoDownConvertTypeChanged(quint8 type);

    void mediaPoolClip1SizeChanged(quint16 size);
    void mediaPoolClip2SizeChanged(quint16 size);

    void audioInputChanged(quint8 index, const QAtem::AudioInput& input);
    void audioMonitorEnabledChanged(bool enabled);
    void audioMonitorGainChanged(float gain);
    void audioMonitorMutedChanged(bool muted);
    void audioMonitorDimmedChanged(bool dimmed);
    void audioMonitorSoloChanged(qint8 solo);
    void audioMasterOutputGainChanged(float gain);
    void audioLevelsChanged();

    void mediaLockStateChanged(quint8 id, bool state);
    void getLockStateChanged(quint8 storeId, bool state);

    void dataTransferFinished(quint16 transferId);

    void topologyChanged(const QAtem::Topology &topology);

    void powerStatusChanged(quint8 status);

    void multiViewLayoutChanged(quint8 multiView, quint8 layout);
    void multiViewInputsChanged(quint8 multiView);

    void macroInfoChanged(quint8 index, const QAtem::MacroInfo &info);
    void macroRunningStateChanged(QAtem::MacroRunningState running, bool repeating, quint8 macroIndex);
    void macroRecordingStateChanged(bool recording, quint8 macroIndex);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QAtemConnection::Commands)

#endif //QATEMCONNECTION_H
