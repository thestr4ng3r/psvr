
#include <QApplication>

#include "mainwindow.h"
#include "hmdwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow main_window;
	main_window.show();

	HMDWindow hmd_window;
	hmd_window.show();

	return app.exec();
}

