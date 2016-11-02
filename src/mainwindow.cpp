
#include <QTimer>
#include <QThreadPool>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>

#include "hmdwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(VideoPlayer *video_player, PSVR *psvr, PSVRThread *psvr_thread, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	this->video_player = video_player;
	this->psvr = psvr;
	this->psvr_thread = psvr_thread;

	hmd_window = 0;
	hid_device_infos = 0;

	ui->setupUi(this);

	player_position_delay_timer.setInterval(16);
	player_position_delay_timer.setSingleShot(true);
	connect(&player_position_delay_timer, SIGNAL(timeout()), this, SLOT(UIPlayerPositionChangedDelayed()));

	connect(psvr_thread, SIGNAL(PSVRUpdate()), this, SLOT(PSVRUpdate()));

	connect(video_player, SIGNAL(PositionChanged(float)), this, SLOT(PlayerPositionChanged(float)));
	connect(video_player, SIGNAL(Playing()), this, SLOT(PlayerPlaying()));
	connect(video_player, SIGNAL(Paused()), this, SLOT(PlayerPaused()));
	connect(video_player, SIGNAL(Stopped()), this, SLOT(PlayerStopped()));


	connect(ui->RefreshHIDDevicesButton, SIGNAL(clicked()), this, SLOT(RefreshHIDDevices()));
	connect(ui->ConnectHIDDeviceButton, SIGNAL(clicked()), this, SLOT(ConnectPSVR()));

	connect(ui->OpenButton, SIGNAL(clicked()), this, SLOT(OpenVideoFile()));

	connect(ui->PlayButton, SIGNAL(clicked()), this, SLOT(UIPlayerPlay()));
	connect(ui->StopButton, SIGNAL(clicked()), this, SLOT(UIPlayerStop()));
	connect(ui->PlayerSlider, SIGNAL(sliderMoved(int)), this, SLOT(UIPlayerPositionChanged(int)));

	connect(ui->FOVDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(FOVValueChanged(double)));
	connect(ui->ResetViewButton, SIGNAL(clicked()), this, SLOT(ResetView()));


	RefreshHIDDevices();
}

MainWindow::~MainWindow()
{
	delete ui;

	if(hid_device_infos)
		hid_free_enumeration(hid_device_infos);
}

void MainWindow::SetHMDWindow(HMDWindow *hmd_window)
{
	this->hmd_window = hmd_window;

	if(hmd_window)
		ui->FOVDoubleSpinBox->setValue(hmd_window->GetHMDWidget()->GetFOV());
}

void MainWindow::RefreshHIDDevices()
{
	if(hid_device_infos)
		hid_free_enumeration(hid_device_infos);

	hid_device_infos = hid_enumerate(0x0, 0x0);

	QStringList items;
	for(struct hid_device_info *dev = hid_device_infos; dev; dev=dev->next)
	{
		items.append(QString(dev->path));
	}

	ui->HIDDevicesListWidget->clear();
	ui->HIDDevicesListWidget->addItems(items);
}

void MainWindow::ConnectPSVR()
{
}

void MainWindow::PSVRUpdate()
{
	//ui->DebugLabel->setText(QString::asprintf("%d\t%d\t%d", psvr->GetAccelerationX(), psvr->GetAccelerationY(), psvr->GetAccelerationZ()));
}

void MainWindow::FOVValueChanged(double v)
{
	if(hmd_window)
		hmd_window->GetHMDWidget()->SetFOV((float)v);
}

void MainWindow::ResetView()
{
	psvr->ResetView();
}

void MainWindow::OpenVideoFile()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open Video"));

	if(file.isNull())
		return;

	if(!video_player->LoadVideo(QDir::toNativeSeparators(file).toLocal8Bit().constData()))
	{
		QMessageBox::critical(this, tr("PSVR Player"), tr("Failed to open video file."));
		return;
	}

	ui->PlayerControlsWidget->setEnabled(true);
}

void MainWindow::UIPlayerPlay()
{
	if(video_player->IsPlaying())
		video_player->Pause();
	else
		video_player->Play();
}

void MainWindow::UIPlayerStop()
{
	video_player->Stop();
}

void MainWindow::UIPlayerPositionChanged(int value)
{
	float pos = (float)value / (float)ui->PlayerSlider->maximum();

	if(player_position_delay_timer.isActive())
	{
		player_position_delayed = pos;
		return;
	}

	video_player->SetPosition(pos);
	player_position_delay_timer.start();
}


void MainWindow::PlayerPlaying()
{
	ui->StopButton->setEnabled(true);
	ui->PlayButton->setText(tr("Pause"));
}

void MainWindow::PlayerPaused()
{
	ui->PlayButton->setText(tr("Play"));
}

void MainWindow::PlayerStopped()
{
	ui->StopButton->setEnabled(false);
	ui->PlayerSlider->setValue(0);
	ui->PlayButton->setText(tr("Play"));
}

void MainWindow::PlayerPositionChanged(float pos)
{
	if(!ui->PlayerSlider->isSliderDown() || player_position_delay_timer.isActive())
		ui->PlayerSlider->setValue((int)((float)ui->PlayerSlider->maximum() * pos));
}

void MainWindow::UIPlayerPositionChangedDelayed()
{
	if(player_position_delayed >= 0.0f)
	{
		video_player->SetPosition(player_position_delayed);
		player_position_delayed = -1.0f;
	}
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_R)
	{
		psvr->ResetView();
	}
	else
	{
		QMainWindow::keyPressEvent(event);
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	psvr_thread->Stop();
	psvr_thread->wait();

	if(hmd_window)
	{
		hmd_window->SetMainWindow(0);
		hmd_window->close();
	}

	QMainWindow::closeEvent(event);
}