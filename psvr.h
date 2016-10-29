
#ifndef PSVR_PSVR_H
#define PSVR_PSVR_H

#include <hidapi/hidapi.h>
#include <QMatrix4x4>

#define PSVR_BUFFER_SIZE 64

class PSVR
{
	private:
		hid_device *psvr_device;

		unsigned char buffer[PSVR_BUFFER_SIZE];
		short x_acc, y_acc, z_acc;

		QMatrix4x4 modelview_matrix;

	public:
		PSVR();
		~PSVR();

		void Open();
		void Close();

		bool Read();

		void Recenter();

		short GetAccelerationX(void)	{ return x_acc; }
		short GetAccelerationY(void)	{ return y_acc; }
		short GetAccelerationZ(void)	{ return z_acc; }

		QMatrix4x4 GetModelViewMatrix(void)	{ return modelview_matrix; }

};

#endif //PSVR_PSVR_H
