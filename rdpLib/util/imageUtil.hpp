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
    static void ConvertImage(const std::string& destFormat,HBITMAP input,unsigned char* output,unsigned int* len);
private:
    static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
};
