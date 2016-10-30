
#include <QApplication>
#include <QWindow>

#include "videoplayer.h"
#include "psvr.h"
#include "mainwindow.h"
#include "hmdwindow.h"

int main(int argc, char *argv[])
{
	QSurfaceFormat format;
	format.setMajorVersion(3);
	format.setMinorVersion(3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	QApplication app(argc, argv);

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

	psvr_thread->start();

	int r = app.exec();

	delete psvr_thread;

	return r;
}

