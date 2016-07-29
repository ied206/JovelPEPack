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
	1. PE 파일을 읽는다
	2. Compressor? Protector?
	3. Compressor
		1) decompress 어셈블리 코드
		2) 알고리즘은 압축해제 속도를 우선
	4. Protector
		1) 과제 : 어떻게 난독화를 해야 하는가?
		2) PE 헤더 꼬기 (PE 규격이랑 실제 윈도우 로더의 차이를 이용)
		3) Register 리네이밍
		4) IAT는 못 꼬나?
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

