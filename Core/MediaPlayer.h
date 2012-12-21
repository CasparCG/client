#pragma once

#include "Shared.h"

#include <QtGui/QWidget>

//#include <Phonon/VideoPlayer>

class CORE_EXPORT MediaPlayer : public QWidget
{
    Q_OBJECT

    public:
        explicit MediaPlayer(QWidget* parent);

        bool isPlaying() const;

        void stop();
        //void play(const Phonon::MediaSource& source);

    private:
        //Phonon::VideoPlayer* player;
};
