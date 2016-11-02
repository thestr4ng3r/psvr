

#include <stdio.h>
#include <hidapi/hidapi.h>
#include <cstdint>
#include <cstring>

#include "psvr.h"



#define MAX_STR			255

int16_t read_int16(unsigned char *buffer, int offset);

#define PSVR_VENDOR_ID	0x054c
#define PSVR_PRODUCT_ID	0x09af

#define ACCELERATION_COEF 0.00003125f

PSVR::PSVR()
{
	psvr_device = 0;
	memset(buffer, 0, sizeof(buffer));

	modelview_matrix.setToIdentity();
}

PSVR::~PSVR()
{
}

hid_device_info *PSVR::EnumerateDevices()
{
	//return hid_enumerate(0x0, 0x0);
	return hid_enumerate(PSVR_VENDOR_ID, PSVR_PRODUCT_ID);
}

bool PSVR::Open(const char *path)
{
	if(path)
		psvr_device = hid_open_path(path);
	else
		psvr_device = hid_open(PSVR_VENDOR_ID, PSVR_PRODUCT_ID, 0);

	if(!psvr_device)
	{
		fprintf(stderr, "Failed to open PSVR HID device.\n");
		return false;
	}

	/*int r;
	wchar_t wstr[MAX_STR];

	r = hid_get_manufacturer_string(psvr_device, wstr, MAX_STR);
	if(r > 0)
		printf("Manufacturer: %ls\n", wstr);

	r = hid_get_product_string(psvr_device, wstr, MAX_STR);
	if(r > 0)
		printf("Product: %ls\n", wstr);

	r = hid_get_serial_number_string(psvr_device, wstr, MAX_STR);
	if(r > 0)
		printf("SN: %ls\n", wstr);*/

	//hid_set_nonblocking(psvr_device, 1);

	return true;
}


void PSVR::Close()
{
	if(!psvr_device)
		return;

	hid_close(psvr_device);
	psvr_device = 0;
}

bool PSVR::Read(int timeout)
{
	if(!psvr_device)
		return false;

	int size = hid_read_timeout(psvr_device, buffer, PSVR_BUFFER_SIZE, timeout);
	
	if(size == 64)
	{
		x_acc = read_int16(buffer, 20) + read_int16(buffer, 36);
		y_acc = read_int16(buffer, 22) + read_int16(buffer, 38);
		z_acc = read_int16(buffer, 24) + read_int16(buffer, 40);

		modelview_matrix.rotate(-y_acc * ACCELERATION_COEF, QVector3D(1.0, 0.0, 0.0) * modelview_matrix);
		modelview_matrix.rotate(-x_acc * ACCELERATION_COEF, QVector3D(0.0, 1.0, 0.0) * modelview_matrix);
		modelview_matrix.rotate(z_acc * ACCELERATION_COEF, QVector3D(0.0, 0.0, 1.0) * modelview_matrix);

		return true;
	}
	else if(size < 0)
	{
		printf("read failed \"%S\"\n", hid_error(psvr_device));
	}

	return false;
}

void PSVR::ResetView()
{
	modelview_matrix.setToIdentity();
}

int16_t read_int16(unsigned char *buffer, int offset)
{
	int16_t v;
	v = buffer[offset];
	v |= buffer[offset+1] << 8;
	return v;
}