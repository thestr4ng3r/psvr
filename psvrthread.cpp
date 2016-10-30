

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
		if(psvr->Read(1))
			emit PSVRUpdate();
	}

	psvr->Close();
}