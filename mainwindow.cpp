
#include <QTimer>
#include <QThreadPool>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(VideoPlayer *video_player, PSVR *psvr, PSVRThread *psvr_thread, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	this->video_player = video_player;
	this->psvr = psvr;
	this->psvr_thread = psvr_thread;

	ui->setupUi(this);

	ui->DebugLabel->setText("test");

	connect(psvr_thread, SIGNAL(PSVRUpdate()), this, SLOT(PSVRUpdate()));
}

MainWindow::~MainWindow()
{
	psvr_thread->Stop();
	psvr_thread->wait();
	delete ui;
}

void MainWindow::PSVRUpdate()
{
	ui->DebugLabel->setText(QString::asprintf("%d\t%d\t%d", psvr->GetAccelerationX(), psvr->GetAccelerationY(), psvr->GetAccelerationZ()));
}