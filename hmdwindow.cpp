
#include <QBoxLayout>
#include <QKeyEvent>

#include "psvr.h"
#include "hmdwindow.h"

HMDWindow::HMDWindow(VideoPlayer *video_player, PSVR *psvr, QWidget *parent) : QMainWindow(parent)
{
	this->video_player = video_player;
	this->psvr = psvr;

	resize(800, 600);

	hmd_widget = new HMDWidget(video_player, psvr);
	setCentralWidget(hmd_widget);
}

HMDWindow::~HMDWindow()
{
	delete hmd_widget;
}

void HMDWindow::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_R)
	{
		psvr->Recenter();
	}
	else
	{
		QMainWindow::keyPressEvent(event);
	}
}