#ifndef VAR_H_INCLUDED
#define VAR_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

#define JV_VER_MAJOR	1
#define JV_VER_MINOR	0

typedef struct jv_arg
{
	bool arg_input;
	bool arg_output;
} JV_ARG;

/*
#pragma pack(push, 1)
struct packed_section
{
	char name[8]; //Section name
	DWORD virtual_size; //Virtual size
	DWORD virtual_address; //Virtual address (RVA)
	DWORD size_of_raw_data; //Raw data size
	DWORD pointer_to_raw_data; //Raw data file offset
	DWORD characteristics; //Section characteristics
};
#pragma pack(pop)
*/


#endif // VAR_H_INCLUDED
