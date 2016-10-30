
#ifndef PSVR_HMDWINDOW_H
#define PSVR_HMDWINDOW_H

#include <QMainWindow>

#include "hmdwidget.h"
#include "mainwindow.h"

class HMDWindow : public QMainWindow
{
	Q_OBJECT

	private:
		HMDWidget *hmd_widget;

		VideoPlayer *video_player;
		PSVR *psvr;

	public:
		HMDWindow(VideoPlayer *video_player, PSVR *psvr, QWidget *parent = 0);
		~HMDWindow();

		HMDWidget *GetHMDWidget()	{ return hmd_widget; }

	protected:
		void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

};


#endif //PSVR_HMDWINDOW_H
