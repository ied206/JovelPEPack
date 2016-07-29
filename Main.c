#include "Var.h"

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#include "BasicIO.h"
#include "PE.h"
#include "ctx.h"

int main(int argc, char* argv[])
{
	JV_CTX ctx;

	printf("Joveler's PE Packer v%02u.%02u (Build %04u%02u%02u)\n\n",
		   JV_VER_MAJOR, JV_VER_MINOR, CompileYear(), CompileMonth(), CompileDay());

	/*
	1. PE ������ �д´�
	2. Compressor? Protector?
	3. Compressor
		1) decompress ����� �ڵ�
		2) �˰����� �������� �ӵ��� �켱
	4. Protector
		1) ���� : ��� ����ȭ�� �ؾ� �ϴ°�?
		2) PE ��� ���� (PE �԰��̶� ���� ������ �δ��� ���̸� �̿�)
		3) Register �����̹�
		4) IAT�� �� ����?
	*/

	JVCTX_Init(&ctx, L"test.exe", L"test_out.exe", JV_CTX_MODE_COMPRESS);
	JVCTX_ReadFile(&ctx);
	JVCTX_Compress(&ctx);
	printf("RawSize = %d\n", ctx.rawSize);
	printf("CompSize = %d\n", ctx.compSize);
	JVCTX_WriteFile(&ctx);
	JVCTX_Close(&ctx);

	return 0;
}

