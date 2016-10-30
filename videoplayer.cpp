

#include <vlc/vlc.h>

#include "videoplayer.h"



static void *lock(void *data, void **p_pixels)
{
	return ((VideoPlayer *) data)->VLC_Lock(p_pixels);
}

static void unlock(void *data, void *id, void *const *p_pixels)
{
	((VideoPlayer *) data)->VLC_Unlock(id, p_pixels);
}

static void display(void *data, void *id)
{
	((VideoPlayer *) data)->VLC_Display(id);
}


unsigned int setup(void **opaque, char *chroma, unsigned int *width, unsigned int *height, unsigned int *pitches, unsigned int *lines)
{
	return ((VideoPlayer *) *opaque)->VLC_Setup(chroma, width, height, pitches, lines);
}

void cleanup(void *opaque)
{
	((VideoPlayer *) opaque)->VLC_Cleanup();
}





VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent)
{
	this->width = 0;
	this->height = 0;

	video_data = 0;

	const char *vlc_argv[] =
		{
			"--no-audio",
			"--no-xlib"
		};

	libvlc = libvlc_new(2, vlc_argv);
}

VideoPlayer::~VideoPlayer()
{
	libvlc_media_player_stop(media_player);
	libvlc_media_player_release(media_player);
	libvlc_release(libvlc);
	data_mutex.unlock();
	delete[] video_data;
}

void VideoPlayer::PlayVideo(const char *path)
{
	media = libvlc_media_new_path(libvlc, path);
	media_player = libvlc_media_player_new_from_media(media);

	libvlc_video_set_callbacks(media_player, lock, unlock, display, this);
	libvlc_video_set_format_callbacks(media_player, setup, cleanup);
	//libvlc_video_set_format(media_player, "RV24", width, height, width*3);
	libvlc_media_player_play(media_player);
}

void *VideoPlayer::VLC_Lock(void **p_pixels)
{
	//printf("lock\n");
	data_mutex.lock();
	*p_pixels = video_data;
	return 0;
}

void VideoPlayer::VLC_Unlock(void *id, void *const *p_pixels)
{
	//printf("unlock\n");
	data_mutex.unlock();
}

void VideoPlayer::VLC_Display(void *id)
{
	emit DisplayVideoFrame();
	//printf("display\n");
}

unsigned int VideoPlayer::VLC_Setup(char *chroma, unsigned int *width, unsigned int *height, unsigned int *pitches, unsigned int *lines)
{
	this->width = *width;
	this->height = *height;

	strcpy(chroma, "RV24");
	*pitches = this->width * 3;
	*lines = this->height;

	delete[] video_data;
	video_data = new unsigned char[this->width * this->height * 3];

	return 1;
}

void VideoPlayer::VLC_Cleanup()
{
	delete[] video_data;
	video_data = 0;
}