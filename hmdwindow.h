
#ifndef PSVR_HMDWINDOW_H
#define PSVR_HMDWINDOW_H

#include <QMainWindow>

#include "hmdwidget.h"

class HMDWindow : public QMainWindow
{
	Q_OBJECT

	private:
		HMDWidget *hmd_widget;

	public:
		HMDWindow(QWidget *parent = 0);
		~HMDWindow();

};


#endif //PSVR_HMDWINDOW_H
