
#include <QBoxLayout>
#include <QKeyEvent>

#include "psvr.h"
#include "hmdwindow.h"

HMDWindow::HMDWindow(VideoPlayer *video_player, PSVR *psvr, QWidget *parent) : QMainWindow(parent)
{
	this->video_player = video_player;
	this->psvr = psvr;

	main_window = 0;

	setWindowTitle(tr("PSVR HMD"));

	resize(640, 480);

	hmd_widget = new HMDWidget(video_player, psvr);
	setCentralWidget(hmd_widget);
}

HMDWindow::~HMDWindow()
{
	delete hmd_widget;
}

void HMDWindow::keyPressEvent(QKeyEvent *event)
{
	switch(event->key())
	{
		case Qt::Key_R:
			psvr->ResetView();
			break;
		case Qt::Key_F11:
			if(this->isFullScreen())
				showNormal();
			else
				showFullScreen();
			break;
		default:
			QMainWindow::keyPressEvent(event);
			break;

	}
}

void HMDWindow::closeEvent(QCloseEvent *event)
{
	if(main_window)
	{
		main_window->SetHMDWindow(0);
		main_window->close();
	}

	QMainWindow::closeEvent(event);
}