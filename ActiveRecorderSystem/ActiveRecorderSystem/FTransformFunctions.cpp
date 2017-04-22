#include "FTransformFunctions.h"
#include <fstream>
#include <string>

#ifdef __cplusplus  
extern "C"
{
#include <libavutil/pixfmt.h>                                                  
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
}
#endif

/*void FTransformFunctions::YUV_16Bit::YUY2_RGB(const byte *YUY2buff, byte *RGBbuff, unsigned int frameWidth, 
	unsigned int frameHeight, const unsigned int bit)
{
	//C = Y - 16
	//D = U - 128
	//E = V - 128
	//R = clip((298 * C + 409 * E + 128) >> 8)
	//G = clip((298 * C - 100 * D - 208 * E + 128) >> 8)
	//B = clip((298 * C + 516 * D + 128) >> 8)
	unsigned int buffSize(frameWidth * frameHeight * bit / 8);
	long C1, D1, C2, E1;
	int R1, G1, B1, R2, G2, B2;
	for (unsigned long count(0); count < buffSize ; count += 4)
	{
		//get Y0 U0 Y1 V0
		C1 = *(YUY2buff++) - 16;
		D1 = *(YUY2buff++) - 128;
		C2 = *(YUY2buff++) - 16;
		E1 = *(YUY2buff++) - 128;

		//transform to RGB0 & RBG1
		R1 = (75 * C1 + 102 * E1 + 32) >> 6;
		G1 = (74 * C1 - 25 * D1 - 52 * E1 + 32) >> 6;
		B1 = (75 * C1 + 129 * D1 + 32) >> 6;
		R2 = (74 * C2 + 102 * E1 + 32) >> 6;
		G2 = (75 * C2 - 25 * D1 - 52 * E1 + 32) >> 6;
		B2 = (74 * C2 + 129 * D1 + 32) >> 6;
		*(RGBbuff++) = (R1 > 255) ? 255 : (R1 < 0) ? 0 : R1;
		*(RGBbuff++) = (G1 > 255) ? 255 : (G1 < 0) ? 0 : G1;
		*(RGBbuff++) = (B1 > 255) ? 255 : (B1 < 0) ? 0 : B1;
		*(RGBbuff++) = (R2 > 255) ? 255 : (R2 < 0) ? 0 : R2;
		*(RGBbuff++) = (G2 > 255) ? 255 : (G2 < 0) ? 0 : G2;
		*(RGBbuff++) = (B2 > 255) ? 255 : (B2 < 0) ? 0 : B2;
	}
}*/

void FTransformFunctions::YUV_16Bit::YUY2_RGB(const byte *YUY2buff, byte *RGBbuff, unsigned int frameWidth,
	unsigned int frameHeight, const unsigned int bit)
{
	//int srcNumBytes,dstNumBytes;
	//uint8_t *pSrc,*pDst;
	AVPicture pFrameYUV, pFrameRGB;

	//pFrameYUV = avpicture_alloc();
	//srcNumBytes = avpicture_get_size(PIX_FMT_YUV420P,width,height);
	//pSrc = (uint8_t *)malloc(sizeof(uint8_t) * srcNumBytes);
	avpicture_fill(&pFrameYUV, YUY2buff, AV_PIX_FMT_YUYV422, frameWidth, frameHeight);

	//U,V»¥»»
	uint8_t * ptmp = pFrameYUV.data[1];
	pFrameYUV.data[1] = pFrameYUV.data[2];
	pFrameYUV.data[2] = ptmp;

	//pFrameBGR = avcodec_alloc_frame();
	//dstNumBytes = avpicture_get_size(PIX_FMT_BGR24,width,height);
	//pDst = (uint8_t *)malloc(sizeof(uint8_t) * dstNumBytes);
	avpicture_fill(&pFrameRGB, RGBbuff, AV_PIX_FMT_RGB24, frameWidth, frameHeight);

	struct SwsContext* imgCtx = nullptr;
	imgCtx = sws_getContext(frameWidth, frameHeight, AV_PIX_FMT_YUYV422, frameWidth, frameHeight, AV_PIX_FMT_RGB24, SWS_BILINEAR, 0, 0, 0);

	if (imgCtx != nullptr) 
	{
		sws_scale(imgCtx, pFrameYUV.data, pFrameYUV.linesize, 0, frameHeight, pFrameRGB.data, pFrameRGB.linesize);
		if (imgCtx) 
		{
			sws_freeContext(imgCtx);
			imgCtx = nullptr;
		}
	}
	else 
	{
		sws_freeContext(imgCtx);
		imgCtx = nullptr;
	}
}