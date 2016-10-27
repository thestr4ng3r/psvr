
#ifndef PSVR_MAINWINDOW_H
#define PSVR_MAINWINDOW_H

#include <QMainWindow>

#include "psvrthread.h"

namespace Ui
{
	class MainWindow;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:
		Ui::MainWindow *ui;

		PSVRThread *psvr_thread;

	protected slots:
		void PSVRAcceleration(short x, short y, short z);
};


#endif //PSVR_MAINWINDOW_H
