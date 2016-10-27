//
// Created by florian on 27.10.16.
//

#ifndef PSVR_PSVR_TASK_H
#define PSVR_PSVR_TASK_H

#include <QThread>

class PSVRThread : public QThread
{
	Q_OBJECT

	public:
		PSVRThread(QObject *parent = 0);
		~PSVRThread();

	signals:
		void PSVRAcceleration(short x, short y, short z);

	protected:
		void run() Q_DECL_OVERRIDE;
};

#endif //PSVR_PSVR_TASK_H
