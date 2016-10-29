//
// Created by florian on 28.10.16.
//

#ifndef PSVR_VIDEOPLAYER_H
#define PSVR_VIDEOPLAYER_H

#include <QObject>
#include <QMutex>

#include <vlc/vlc.h>

class VideoPlayer : public QObject
{
	Q_OBJECT

	private:
		libvlc_instance_t *libvlc;
		libvlc_media_t *media;
		libvlc_media_player_t *media_player;

		unsigned int width, height;
		unsigned char *video_data;
		QMutex data_mutex;

	public:
		VideoPlayer(QObject *parent = 0);
		~VideoPlayer();

		void PlayVideo(const char *path);

		unsigned char *GetVideoData()		{ return video_data; }
		void LockVideoData()				{ data_mutex.lock(); }
		void UnlockVideoData()				{ data_mutex.unlock(); }

		void *VLC_Lock(void **p_pixels);
		void VLC_Unlock(void *id, void *const *p_pixels);
		void VLC_Display(void *id);

	signals:
		void DisplayVideoFrame();
};

#endif //PSVR_VIDEOPLAYER_H
