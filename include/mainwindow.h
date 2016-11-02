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

#ifndef PSVR_MAINWINDOW_H
#define PSVR_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "psvrthread.h"
#include "videoplayer.h"
#include "psvr.h"

namespace Ui
{
	class MainWindow;
}

class HMDWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	private:
		Ui::MainWindow *ui;

		struct hid_device_info *hid_device_infos;

		PSVRThread *psvr_thread;

		VideoPlayer *video_player;
		PSVR *psvr;

		HMDWindow *hmd_window;

		float player_position_delayed;
		QTimer player_position_delay_timer;

	public:
		MainWindow(VideoPlayer *video_player, PSVR *psvr, PSVRThread *psvr_thread, QWidget *parent = 0);
		~MainWindow();

		void SetHMDWindow(HMDWindow *hmd_window);

	protected slots:
		void RefreshHIDDevices();
		void ConnectPSVR();

		void PSVRUpdate();
		void FOVValueChanged(double v);

		void ResetView();

		void OpenVideoFile();

		void UIPlayerPlay();
		void UIPlayerStop();
		void UIPlayerPositionChanged(int value);

		void UIPlayerPositionChangedDelayed();

		void PlayerPlaying();
		void PlayerPaused();
		void PlayerStopped();
		void PlayerPositionChanged(float pos);

		void UpdateVideoAngle();
		void UpdateVideoProjection();

		void SetRGBWorkaround(bool enabled);

	protected:
		void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
		void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
};


#endif //PSVR_MAINWINDOW_H
