#include "Var.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <windows.h>

#include "BasicIO.h"
#include "PE.h"
#include "lz4.h"
#include "ctx.h"

void JVCTX_Init(JV_CTX* ctx, wchar_t inFileName[], wchar_t outFileName[], int mode)
{
	HANDLE hFile = NULL;
	LARGE_INTEGER fileSize;

	// Wipe Context
	memset((void*) ctx, 0, sizeof(JV_CTX));

	wcsncpy(ctx->inFileName, inFileName, JV_CTX_MAX_PATH);
	ctx->inFileName[JV_CTX_MAX_PATH-1] = L'\0';
	wcsncpy(ctx->outFileName, outFileName, JV_CTX_MAX_PATH);
	ctx->outFileName[JV_CTX_MAX_PATH-1] = L'\0';
	ctx->mode = mode;

	// Open File and get Handle
	hFile = CreateFileW(ctx->inFileName,				// lpFileName
						GENERIC_READ | GENERIC_WRITE, 	// dwDesiredAccess
						FILE_SHARE_READ, 				// dwShareMode
						NULL, 							// lpSecurityAttributes
						OPEN_EXISTING, 					// dwCreationDisposition
						FILE_ATTRIBUTE_NORMAL,			// dwFlagsAndAttributes
						NULL);							// hTemplateFile
	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "[ERR] CreateFile() failed!\nError Code : %lu\n\n", GetLastError());
		exit(1);
	}

	// Get File Size
	if (!GetFileSizeEx(hFile, &fileSize))
	{
		fprintf(stderr, "[ERR] GetFileSizeEx() failed!\nError Code : %lu\n\n", GetLastError());
		exit(1);
	}
	// Larger than 2GB? Terminate
	if (fileSize.HighPart > 0 || (int) fileSize.LowPart < 0)
	{
		fprintf(stderr, "[ERR] %S is larger than 2GB, terminate.\n\n", ctx->inFileName);
		exit(1);
	}
	ctx->rawSize = fileSize.LowPart;
	ctx->compMaxSize = fileSize.LowPart;

	// Allocate Memory
	if (0 < ctx->rawSize)
		ctx->rawData = (uint8_t*) calloc(ctx->rawSize, sizeof(uint8_t));
	if (0 < ctx->compMaxSize)
		ctx->compData = (uint8_t*) calloc(ctx->compMaxSize, sizeof(uint8_t));
	if (!ctx->rawData || !ctx->compData)
	{
		fprintf(stderr, "[ERR] malloc() failed!\n\n");
		exit(1);
	}

	// Close Handle
	CloseHandle(hFile);

	return;
}

void JVCTX_Close(JV_CTX* ctx)
{
	if (0 < ctx->rawSize)
		free(ctx->rawData);
	if (0 < ctx->compMaxSize)
		free(ctx->compData);

	return;
}

int JVCTX_ReadFile(JV_CTX* ctx)
{
	HANDLE hFile = NULL;
	BOOL retVal = FALSE;
	DWORD readByte = 0;

	// Open File and get Handle
	hFile = CreateFileW(ctx->inFileName,				// lpFileName
						GENERIC_READ | GENERIC_WRITE, 	// dwDesiredAccess
						FILE_SHARE_READ, 				// dwShareMode
						NULL, 							// lpSecurityAttributes
						OPEN_EXISTING, 					// dwCreationDisposition
						FILE_ATTRIBUTE_NORMAL,			// dwFlagsAndAttributes
						NULL);							// hTemplateFile
	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "[ERR] CreateFile() failed!\nError Code : %lu\n\n", GetLastError());
		exit(1);
	}

	// Read PE File
	retVal = ReadFile(hFile, ctx->rawData, ctx->rawSize, &readByte, NULL);
	if (!retVal)
	{
		fprintf(stderr, "[ERR] ReadFile() failed!\nError Code : %lu\n\n", GetLastError());
		exit(1);
	}
	// Is all bytes successfully read?
	if (readByte != ctx->rawSize)
	{
		fprintf(stderr, "[ERR] ReadFile() read %luB, but %S is %"PRIu32"uB.\n\n", readByte, ctx->inFileName, ctx->rawSize);
		exit(1);
	}

	// Close Handle
	CloseHandle(hFile);

	return 0;
}

int JVCTX_WriteFile(JV_CTX* ctx)
{
	HANDLE hFile = NULL;
	BOOL retVal = FALSE;
	DWORD writtenByte = 0;

	// Open File and get Handle
	hFile = CreateFileW(ctx->outFileName,				// lpFileName
						GENERIC_READ | GENERIC_WRITE, 	// dwDesiredAccess
						0, 								// dwShareMode
						NULL, 							// lpSecurityAttributes
						CREATE_NEW, 					// dwCreationDisposition
						FILE_ATTRIBUTE_NORMAL,			// dwFlagsAndAttributes
						NULL);							// hTemplateFile
	if (hFile == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "[ERR] CreateFile() failed!\nError Code : %lu\n\n", GetLastError());
		exit(1);
	}

	// Read PE File
	retVal = WriteFile(hFile, ctx->compData, ctx->compSize, &writtenByte, NULL);
	if (!retVal)
	{
		fprintf(stderr, "[ERR] WriteFile() failed!\nError Code : %lu\n\n", GetLastError());
		exit(1);
	}
	// Is all bytes successfully written?
	if (writtenByte != ctx->compSize)
	{
		fprintf(stderr, "[ERR] WriteFile() wrote %luB, but %S should be %"PRIu32"uB.\n\n", writtenByte, ctx->outFileName, ctx->compSize);
		exit(1);
	}
	// Close Handle
	CloseHandle(hFile);

	return 0;
}

int JVCTX_Compress(JV_CTX* ctx)
{
	ctx->compSize = LZ4_compress_default((const char*) ctx->rawData, (char*) ctx->compData, ctx->rawSize, ctx->compMaxSize);
	if (ctx->compSize == 0)
	{
		fprintf(stderr, "[ERR] LZ4_compress_default() failed!\n\n");
		exit(1);
	}
	return 0;
}

int JVCTX_Decompress(JV_CTX* ctx)
{
	ctx->compSize = LZ4_decompress_safe((const char*) ctx->compData, (char*) ctx->rawData, ctx->compSize, ctx->compMaxSize);
	if (ctx->compSize < 0)
	{
		fprintf(stderr, "[ERR] LZ4_decompress_safe() failed!\nError Code : %d\n\n", ctx->compSize);
		exit(1);
	}
	return 0;
}
