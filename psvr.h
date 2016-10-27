//
// Created by florian on 27.10.16.
//

#ifndef PSVR_PSVR_H
#define PSVR_PSVR_H

#include <hidapi/hidapi.h>

#define PSVR_BUFFER_SIZE 64

class PSVR
{
	private:
		hid_device *psvr_device;

		unsigned char buffer[PSVR_BUFFER_SIZE];
		short x_acc, y_acc, z_acc;

		float rot_x, rot_y, rot_z;

	public:
		PSVR();
		~PSVR();

		void Open();
		void Close();

		void Read();

		short GetAccelerationX(void)	{ return x_acc; }
		short GetAccelerationY(void)	{ return y_acc; }
		short GetAccelerationZ(void)	{ return z_acc; }

		float GetRotationX(void)		{ return rot_x; }
		float GetRotationY(void)		{ return rot_y; }
		float GetRotationZ(void)		{ return rot_z; }

};

extern PSVR psvr;

#endif //PSVR_PSVR_H
