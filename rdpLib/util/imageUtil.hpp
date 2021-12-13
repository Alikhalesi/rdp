#pragma once
#include "rdpLib_global.h"
#include <string>

#include <windows.h>

#include <stdio.h>
#include <shlwapi.h>
#include <atlbase.h>
#include <memory>
#include <QDebug>
#include <atlutil.h>
#include <atlbase.h>
#include <d3d11.h>
#include <wincodec.h>
class RDPLIB_EXPORT ImageUtil
{
public:
    ImageUtil();
     void ConvertHBitmapToJpegImage(HBITMAP input,unsigned char* output,unsigned int* len);
     void ConvertJPegToHbitmap(unsigned char* input,unsigned int len,HWND handle);
   static  void SaveTextureToJpg(unsigned char* input, unsigned int* len, ID3D11Texture2D* Texture);
     ~ImageUtil();
private:
     int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
     BOOL DrawBitmap (HWND hDC,HBITMAP hBitmap);
      ULONG_PTR gdiplusToken_;
};
