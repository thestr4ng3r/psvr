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
		libvlc_event_manager_t *event_manager;

		unsigned int width, height;
		unsigned char *video_data;
		QMutex data_mutex;

		void UnloadVideo();

	public:
		VideoPlayer(QObject *parent = 0);
		~VideoPlayer();

		bool LoadVideo(const char *path);

		void Play();
		void Pause();
		void Stop();
		void SetPosition(float pos);

		bool IsPlaying();

		unsigned char *GetVideoData()		{ return video_data; }
		unsigned int GetWidth()				{ return width; }
		unsigned int GetHeight()			{ return height; }

		void LockVideoData()				{ data_mutex.lock(); }
		void UnlockVideoData()				{ data_mutex.unlock(); }

		void *VLC_Lock(void **p_pixels);
		void VLC_Unlock(void *id, void *const *p_pixels);
		void VLC_Display(void *id);

		unsigned int VLC_Setup(char *chroma, unsigned int *width, unsigned int *height, unsigned int *pitches, unsigned int *lines);
		void VLC_Cleanup();

		void VLC_Event(const struct libvlc_event_t *event);

	signals:
		void DisplayVideoFrame();

		void Playing();
		void Paused();
		void Stopped();
		void PositionChanged(float pos);
};

#endif //PSVR_VIDEOPLAYER_H
