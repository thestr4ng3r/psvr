

#include <stdio.h>
#include <hidapi/hidapi.h>
#include <cstdint>
#include <cstring>

#include "psvr.h"

PSVR psvr;


#define MAX_STR			255

int16_t read_int16(unsigned char *buffer, int offset);

#define PSVR_VENDOR_ID	0x054c
#define PSVR_PRODUCT_ID	0x09af

#define ACCELERATION_COEF 0.00003125f

PSVR::PSVR()
{
	psvr_device = 0;
	memset(buffer, 0, sizeof(buffer));

	rot_x = rot_y = rot_z = 0.0f;
}

PSVR::~PSVR()
{
}

void PSVR::Open()
{
	psvr_device = hid_open(PSVR_VENDOR_ID, PSVR_PRODUCT_ID, 0);
	if(!psvr_device)
	{
		fprintf(stderr, "Failed to open PSVR HID device.\n");
		return;
	}

	int r;
	wchar_t wstr[MAX_STR];

	r = hid_get_manufacturer_string(psvr_device, wstr, MAX_STR);
	if(r > 0)
		printf("Manufacturer: %ls\n", wstr);

	r = hid_get_product_string(psvr_device, wstr, MAX_STR);
	if(r > 0)
		printf("Product: %ls\n", wstr);

	r = hid_get_serial_number_string(psvr_device, wstr, MAX_STR);
	if(r > 0)
		printf("SN: %ls\n", wstr);
}


void PSVR::Close()
{
	if(!psvr_device)
		return;

	hid_close(psvr_device);
	psvr_device = 0;
}

void PSVR::Read()
{
	int size = hid_read(psvr_device, buffer, PSVR_BUFFER_SIZE);

	x_acc = read_int16(buffer, 20) + read_int16(buffer, 36);
	y_acc = read_int16(buffer, 22) + read_int16(buffer, 38);
	z_acc = read_int16(buffer, 24) + read_int16(buffer, 40);

	rot_x += x_acc * ACCELERATION_COEF;
	rot_y += y_acc * ACCELERATION_COEF;
	rot_z += z_acc * ACCELERATION_COEF;
}

int16_t read_int16(unsigned char *buffer, int offset)
{
	int16_t v;
	v = buffer[offset];
	v |= buffer[offset+1] << 8;
	return v;
}