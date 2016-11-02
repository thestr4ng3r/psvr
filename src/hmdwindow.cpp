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