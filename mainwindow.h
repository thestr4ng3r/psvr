
#ifndef PSVR_MAINWINDOW_H
#define PSVR_MAINWINDOW_H

#include <QMainWindow>

#include "psvrthread.h"
#include "videoplayer.h"
#include "psvr.h"

namespace Ui
{
	class MainWindow;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT

	private:
		Ui::MainWindow *ui;

		PSVRThread *psvr_thread;

		VideoPlayer *video_player;
		PSVR *psvr;

	public:
		MainWindow(VideoPlayer *video_player, PSVR *psvr, PSVRThread *psvr_thread, QWidget *parent = 0);
		~MainWindow();

	protected slots:
		void PSVRAcceleration(short x, short y, short z);
};


#endif //PSVR_MAINWINDOW_H
