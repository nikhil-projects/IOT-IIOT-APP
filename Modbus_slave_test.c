/*
 * Modbus_slave_test.c
 *
 *  Created on: May 26, 2017
 *      Author: Shilesh
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "../../libs/modbus/modbus.h"

#include "unit-test.h"

enum {
	TCP,
	TCP_PI,
	RTU
};

int main(int argc, char *argv[])
{
	int lp, ret;
	struct timeval response_timeout;
	response_timeout.tv_sec = 1;
	response_timeout.tv_usec = 0;

	char *usb_port_name = {0};
	unsigned int baud_rate = 0;
	unsigned int parity = 0;
	unsigned int data_bit = 0;
	unsigned int stop_bit = 0;
	unsigned int func_code = 0;
	unsigned int slave_add = 0;
	unsigned int reg_add = 0;
	unsigned int num_of_reg = 0;
	unsigned int wr_reg_value = 0;
	uint16_t reg[50]  = {0};// will store read registers values
	modbus_t *ctx = NULL;


	/* Validate input arg */
	if (argc != 11){
		printf("Invalid argument(%d) recieved\n", argc);
	}
	else {
	/* Copy all run time argument */
	usb_port_name 	= strdup(argv[1]);
	baud_rate 		= atoi(argv[2]);
	parity 			= atoi(argv[3]);
	data_bit 		= atoi(argv[4]);
	stop_bit 		= atoi(argv[5]);
	func_code 		= atoi(argv[6]);
	slave_add 		= atoi(argv[7]);
	reg_add 		= atoi(argv[8]);
	num_of_reg 		= atoi(argv[9]);
	wr_reg_value    = atoi(argv[10]);
	}
	printf("======================================================================================================================================\n");
	printf("====Port_Name====Baud_Id====Parity====Data_Bit====Stop_Bit====Func_Code====Slave_Add====Reg_Add====No_of_Reg_rd/rw====reg_wr_value====\n");
	printf("====    %s   ====  %d   ==== %d   ====  %d    ====  %d    ====  %d     ====  %d     ====   %d  ====   %d          ====   %d         ====\n",usb_port_name,baud_rate ,parity,data_bit,stop_bit,func_code,slave_add,reg_add,num_of_reg,wr_reg_value);
	printf("======================================================================================================================================\n");





	if (parity == 0){
		//Create a new RTU context with proper serial parameters (in this example,
		//device name /dev/ttyS0, baud rate 9600, no parity bit, 8 data bits, 1 stop bit)
		ctx = modbus_new_rtu(usb_port_name, baud_rate, 'E', data_bit, stop_bit);
		if (!ctx)
		{
			printf("Failed to create the context:\n");
			return -1;
		}

	}
	else if (parity == 1) {

		//Create a new RTU context with proper serial parameters (in this example,
		//device name /dev/ttyS0, baud rate 9600, no parity bit, 8 data bits, 1 stop bit)
		ctx = modbus_new_rtu(usb_port_name, baud_rate, 'O', data_bit, stop_bit);
		if (!ctx)
		{
			printf("Failed to create the context:\n");
			return -1;
		}

	}
	else {

		//Create a new RTU context with proper serial parameters (in this example,
		//device name /dev/ttyS0, baud rate 9600, no parity bit, 8 data bits, 1 stop bit)
		ctx = modbus_new_rtu(usb_port_name, baud_rate, 'N', data_bit, stop_bit);
		if (!ctx)
		{
			printf("Failed to create the context:\n");
			return -1;
		}

	}

	modbus_set_debug(ctx, TRUE);
	modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL);
	//Set the Modbus address of the remote slave (to 3)
	modbus_set_slave(ctx, slave_add);
	if (modbus_connect(ctx) == -1){
		printf("Unable to connect:\n");
		modbus_free(ctx);
		return -1;
	}

	//void modbus_get_response_timeout(modbus_t *ctx, struct timeval *timeout);
	//modbus_get_response_timeout(ctx, &response_timeout);
	if (func_code == 3){
		//Read 5 holding registers starting from address 101
		int num = modbus_read_registers(ctx, reg_add, num_of_reg , reg);
		//int num = modbus_read_input_registers(ctx, 100, 2, reg);
		if (!num)
		{// number of read registers is not the one expected
			fprintf(stderr, "Failed to read: %s\n", modbus_strerror(errno));
		}
	}
	else if(func_code == 4)
	{
		/* Read all  input registers */
		int ret = modbus_read_input_registers(ctx, reg_add, num_of_reg , reg);
		if (ret == -1){
			/* Fail to read Mos_bus registers */
			printf("modbus_read_input_registers = %d\n", ret);
		}
	}
	else if (func_code == 6){
		/* Write holding registers */
		int ret = modbus_write_register(ctx, reg_add , wr_reg_value);
		if (ret == -1){
			/* Fail to read Mos_bus registers */
			/* Reset mos_bus read flag */
			printf("modbus_read_input_registers = %d\n", ret );
		}
	}
	else{

	}

	printf("print holding register\n");
	for (lp = 0; lp < num_of_reg; lp++)
		printf("read_register_value [%d] = %d\n", lp, reg[lp]);

	modbus_close(ctx);
	modbus_free(ctx);
	return 0;
}
