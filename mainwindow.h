
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

class HMDWindow;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	private:
		Ui::MainWindow *ui;

		PSVRThread *psvr_thread;

		VideoPlayer *video_player;
		PSVR *psvr;

		HMDWindow *hmd_window;

	public:
		MainWindow(VideoPlayer *video_player, PSVR *psvr, PSVRThread *psvr_thread, QWidget *parent = 0);
		~MainWindow();

		void SetHMDWindow(HMDWindow *hmd_window);

	protected slots:
		void PSVRUpdate();
		void FOVValueChanged(double v);

	protected:
		void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
};


#endif //PSVR_MAINWINDOW_H
