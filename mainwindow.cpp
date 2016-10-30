
#include <QTimer>
#include <QThreadPool>

#include "hmdwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(VideoPlayer *video_player, PSVR *psvr, PSVRThread *psvr_thread, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	this->video_player = video_player;
	this->psvr = psvr;
	this->psvr_thread = psvr_thread;
	this->hmd_window = 0;

	ui->setupUi(this);

	ui->DebugLabel->setText("test");

	connect(psvr_thread, SIGNAL(PSVRUpdate()), this, SLOT(PSVRUpdate()));

	connect(ui->FOVDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(FOVValueChanged(double)));
}

MainWindow::~MainWindow()
{
	psvr_thread->Stop();
	psvr_thread->wait();
	delete ui;
}

void MainWindow::SetHMDWindow(HMDWindow *hmd_window)
{
	this->hmd_window = hmd_window;
	ui->FOVDoubleSpinBox->setValue(hmd_window->GetHMDWidget()->GetFOV());
}

void MainWindow::PSVRUpdate()
{
	ui->DebugLabel->setText(QString::asprintf("%d\t%d\t%d", psvr->GetAccelerationX(), psvr->GetAccelerationY(), psvr->GetAccelerationZ()));
}

void MainWindow::FOVValueChanged(double v)
{
	if(hmd_window)
		hmd_window->GetHMDWidget()->SetFOV((float)v);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(hmd_window)
		hmd_window->close();

	QMainWindow::closeEvent(event);
}