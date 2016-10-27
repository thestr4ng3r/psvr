

#include "psvrthread.h"

#include "psvr.h"

PSVRThread::PSVRThread(QObject *parent) : QThread(parent)
{
}

PSVRThread::~PSVRThread()
{
}

void PSVRThread::run()
{
	while(1)
	{
		psvr.Read();
		emit PSVRAcceleration(psvr.GetAccelerationX(), psvr.GetAccelerationY(), psvr.GetAccelerationZ());
	}
}