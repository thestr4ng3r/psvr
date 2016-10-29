

#include "psvrthread.h"

#include "psvr.h"

PSVRThread::PSVRThread(PSVR *psvr, QObject *parent) : QThread(parent)
{
	this->psvr = psvr;
}

PSVRThread::~PSVRThread()
{
}

void PSVRThread::run()
{
	running = true;
	psvr->Open();

	while(running)
	{
		while(psvr->Read())
			emit PSVRUpdate();

		msleep(1);
	}

	psvr->Close();
}