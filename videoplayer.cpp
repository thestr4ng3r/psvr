

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



VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent)
{
	this->width = 512;
	this->height = 512;

	const char *vlc_argv[] =
		{
			/*"--no-audio",*/
			"--no-xlib"
		};

	libvlc = libvlc_new(1, vlc_argv);

	video_data = new unsigned char[width * height * 3];
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
	libvlc_video_set_format(media_player, "RV24", width, height, width*3);
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