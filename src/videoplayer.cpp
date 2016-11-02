

/*
 * Created by Florian Märkl <info@florianmaerkl.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

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


static unsigned int setup(void **opaque, char *chroma, unsigned int *width, unsigned int *height, unsigned int *pitches, unsigned int *lines)
{
	return ((VideoPlayer *) *opaque)->VLC_Setup(chroma, width, height, pitches, lines);
}

static void cleanup(void *opaque)
{
	((VideoPlayer *) opaque)->VLC_Cleanup();
}


static void vlc_event(const struct libvlc_event_t *event, void *data)
{
	((VideoPlayer *)data)->VLC_Event(event);
}





VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent)
{
	this->width = 0;
	this->height = 0;

	video_data = 0;

	media = 0;
	media_player = 0;
	event_manager = 0;

	const char *vlc_argv[] =
		{
			"--no-xlib",
			"--verbose=2"
		};

	libvlc = libvlc_new(2, vlc_argv); // vlc_argv);

	if(!libvlc)
	{
		fprintf(stderr, "Failed to initialize LibVLC\n");
	}
}

VideoPlayer::~VideoPlayer()
{
	UnloadVideo();
	libvlc_release(libvlc);
	delete[] video_data;
}

bool VideoPlayer::LoadVideo(const char *path)
{
	UnloadVideo();

	media = libvlc_media_new_path(libvlc, path);
	if(!media)
	{
		fprintf(stderr, "Failed to open video file \"%s\": %s\n", path, libvlc_errmsg());
		return false;
	}

	media_player = libvlc_media_player_new_from_media(media);
	if(!media_player)
	{
		libvlc_media_release(media);
		media = 0;
		return false;
	}

	libvlc_video_set_callbacks(media_player, lock, unlock, display, this);
	libvlc_video_set_format_callbacks(media_player, setup, cleanup);
	//libvlc_video_set_format(media_player, "RV24", width, height, width*3);

	event_manager = libvlc_media_player_event_manager(media_player);
	libvlc_event_attach(event_manager, libvlc_MediaPlayerPositionChanged, vlc_event, this);
	libvlc_event_attach(event_manager, libvlc_MediaPlayerStopped, vlc_event, this);
	libvlc_event_attach(event_manager, libvlc_MediaPlayerPlaying, vlc_event, this);
	libvlc_event_attach(event_manager, libvlc_MediaPlayerPaused, vlc_event, this);

	libvlc_media_player_play(media_player);

	return true;
}

void VideoPlayer::UnloadVideo()
{
	if(media_player)
	{
		libvlc_media_player_stop(media_player);
		libvlc_media_player_release(media_player);
	}
	if(media)
		libvlc_media_release(media);

	//data_mutex.unlock();

	media = 0;
	media_player = 0;
	event_manager = 0;
}

void VideoPlayer::Play()
{
	if(!media_player)
		return;
	libvlc_media_player_play(media_player);
}

void VideoPlayer::Pause()
{
	if(!media_player)
		return;
	libvlc_media_player_pause(media_player);
}

void VideoPlayer::Stop()
{
	if(!media_player)
		return;
	libvlc_media_player_stop(media_player);
}

void VideoPlayer::SetPosition(float pos)
{
	if(!media_player)
		return;
	libvlc_media_player_set_position(media_player, pos);
}

bool VideoPlayer::IsPlaying()
{
	if(!media_player)
		return false;
	return libvlc_media_player_is_playing(media_player) != 0;
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

void VideoPlayer::VLC_Event(const struct libvlc_event_t *event)
{
	switch(event->type)
	{
		case libvlc_MediaPlayerPositionChanged:
			emit PositionChanged(event->u.media_player_position_changed.new_position);
			break;
		case libvlc_MediaPlayerPlaying:
			emit Playing();
			break;
		case libvlc_MediaPlayerPaused:
			emit Paused();
			break;
		case libvlc_MediaPlayerStopped:
			emit Stopped();
			break;
		default:
			break;
	}
}