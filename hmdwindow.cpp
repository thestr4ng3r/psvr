
#include <QBoxLayout>

#include "hmdwindow.h"

HMDWindow::HMDWindow(QWidget *parent) : QMainWindow(parent)
{
	resize(800, 600);

	hmd_widget = new HMDWidget();
	setCentralWidget(hmd_widget);
}

HMDWindow::~HMDWindow()
{
}
