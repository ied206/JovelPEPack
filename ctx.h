#ifndef CTX_H_INCLUDED
#define CTX_H_INCLUDED

#define JV_CTX_MODE_COMPRESS	1
#define JV_CTX_MODE_PROTECT		2

#define JV_CTX_MAX_PATH	260

typedef struct jv_ctx
{
	int mode;
    wchar_t inFileName[JV_CTX_MAX_PATH];
    wchar_t outFileName[JV_CTX_MAX_PATH];
    uint32_t rawSize;
    uint8_t* rawData;
    uint32_t compSize;
    uint32_t compMaxSize;
    uint8_t* compData;
} JV_CTX;

void JVCTX_Init(JV_CTX* ctx, wchar_t inFileName[], wchar_t outFileName[], int mode);
void JVCTX_Close(JV_CTX* ctx);
int JVCTX_ReadFile(JV_CTX* ctx);
int JVCTX_WriteFile(JV_CTX* ctx);
int JVCTX_Compress(JV_CTX* ctx);
int JVCTX_Decompress(JV_CTX* ctx);

#endif // CTX_H_INCLUDED
