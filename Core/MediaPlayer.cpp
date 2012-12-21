#include "MediaPlayer.h"

//#include <Phonon/MediaSource>

MediaPlayer::MediaPlayer(QWidget* parent)
    : QWidget(parent)
{
    //this->player = new Phonon::VideoPlayer(Phonon::VideoCategory, parent);
    //this->player->resize(0, 0);
}

/*
void MediaPlayer::play(const Phonon::MediaSource& source)
{
    //this->player->resize(this->parentWidget()->size());
    //this->player->play(source);
}
*/

void MediaPlayer::stop()
{
    //this->player->resize(0, 0);
    //this->player->stop();
}

bool MediaPlayer::isPlaying() const
{
    return false;
    //return this->player->isPlaying();
}
