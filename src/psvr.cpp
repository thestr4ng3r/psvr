

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

void PSVR::Open()
{
	int res;
	unsigned char buf[65];
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;

	//char *path = 0;
	//int w = 0;

	// Enumerate and print the HID devices on the system
	struct hid_device_info *devs, *cur_dev;

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;
	while(cur_dev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
			   cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
		printf("  Product:      %ls\n", cur_dev->product_string);
		printf("\n");

		/*if(cur_dev->vendor_id == PSVR_VENDOR_ID && cur_dev->product_id == PSVR_PRODUCT_ID)
		{
			if(w == 2)
			{
				path = cur_dev->path;
			}

			w++;
		}*/

		cur_dev = cur_dev->next;
	}




	psvr_device = hid_open(PSVR_VENDOR_ID, PSVR_PRODUCT_ID, 0);
	//psvr_device = hid_open_path(path);
	hid_free_enumeration(devs);
	if(!psvr_device)
	{
		fprintf(stderr, "Failed to open PSVR HID device.\n");
		return;
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