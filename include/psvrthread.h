//
// Created by florian on 27.10.16.
//

#ifndef PSVR_PSVR_TASK_H
#define PSVR_PSVR_TASK_H

#include <QThread>
#include "psvr.h"

class PSVRThread : public QThread
{
	Q_OBJECT

	private:
		PSVR *psvr;

	public:
		PSVRThread(PSVR *psvr, QObject *parent = 0);
		~PSVRThread();

	signals:
		void PSVRUpdate();

	protected:
		void run() Q_DECL_OVERRIDE;
};

#endif //PSVR_PSVR_TASK_H
