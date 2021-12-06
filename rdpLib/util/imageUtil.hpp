#pragma once
#include "rdpLib_global.h"
#include <string>
#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <shlwapi.h>
#include <atlbase.h>
#include <memory>
#include <QDebug>
class RDPLIB_EXPORT ImageUtil
{
public:
    ImageUtil();
     void ConvertHBitmapToJpegImage(HBITMAP input,unsigned char* output,unsigned int* len);
     void ConvertJPegToHbitmap(unsigned char* input,unsigned int len,HWND handle);

     ~ImageUtil();
private:
     int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
     BOOL DrawBitmap (HWND hDC,HBITMAP hBitmap);
      ULONG_PTR gdiplusToken_;
};
