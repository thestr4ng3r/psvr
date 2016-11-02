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

#include <QApplication>
#include <QWindow>

#include "videoplayer.h"
#include "psvr.h"
#include "mainwindow.h"
#include "hmdwindow.h"

#include "project_version.h"

int main(int argc, char *argv[])
{
	printf("PSVR Player Version %s\n", PROJECT_VERSION);

	QSurfaceFormat format;
	format.setMajorVersion(3);
	format.setMinorVersion(3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	QApplication app(argc, argv);

	hid_init();

	PSVR psvr;
	PSVRThread *psvr_thread = new PSVRThread(&psvr);

	VideoPlayer video_player;

	MainWindow main_window(&video_player, &psvr, psvr_thread);
	main_window.show();

	HMDWindow hmd_window(&video_player, &psvr);
	hmd_window.show();
	//hmd_window.showFullScreen();
	//hmd_window.windowHandle()->setScreen(app.screens()[1]);
	//hmd_window.setGeometry(1920, 0, 1920, 1080);

	main_window.SetHMDWindow(&hmd_window);
	hmd_window.SetMainWindow(&main_window);

	//video_player.LoadVideo("test.webm");

	//psvr_thread->start();

	int r = app.exec();

	delete psvr_thread;

	hid_exit();

	return r;
}

