#include "imageUtil.hpp"
using namespace Gdiplus;
ImageUtil::ImageUtil()
{
    // Initialize GDI+.
       GdiplusStartupInput gdiplusStartupInput;

       GdiplusStartup(&gdiplusToken_, &gdiplusStartupInput, NULL);
}
//====================================================================================================================
void ImageUtil::ConvertJPegToHbitmap(unsigned char *input,unsigned int len, HWND hwnd)
{
    CLSID   encoderClsid;
    Status  stat;
    CComPtr<IStream> srcstream;
    auto hr= CreateStreamOnHGlobal(input,false,&srcstream);
    Image*   image = Image::FromStream(srcstream);
    qDebug()<<"stream img height: "<< image->GetHeight();


    CComPtr<IStream> desstream=SHCreateMemStream(nullptr,0);


    GetEncoderClsid(L"image/bmp", &encoderClsid);
   // image->Save(L"d:\\bmp.bmp", &encoderClsid);
    stat = image->Save(desstream, &encoderClsid, NULL);
    assert(stat==S_OK);

      auto* resultBmp=  Bitmap::FromStream(desstream);
       Color color;
      // resultBmp->Save(L"d:\\bxmp.bmp", &encoderClsid);
       HBITMAP bitmap;
       stat= resultBmp->GetHBITMAP(color,&bitmap);
        assert(stat==S_OK);

        //bitblitingTo window
        DrawBitmap(hwnd,bitmap);
delete resultBmp;


    delete image;
}
//====================================================================================================================
ImageUtil::~ImageUtil()
{
    GdiplusShutdown(gdiplusToken_);
}
//====================================================================================================================
int ImageUtil::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}
//====================================================================================================================
 void ImageUtil::ConvertHBitmapToJpegImage( HBITMAP input,unsigned char* output,unsigned int* len)
{


       CLSID   encoderClsid;
       Status  stat;
       Image*   image = Bitmap::FromHBITMAP(input,nullptr);
qDebug()<<"hbitmap height: "<< image->GetHeight();
       // Get the CLSID of the jpg encoder.
       GetEncoderClsid(L"image/jpeg", &encoderClsid);
       CComPtr<IStream> stream;
auto hr= CreateStreamOnHGlobal(output,false,&stream);
assert(hr==S_OK);


       stat = image->Save(stream, &encoderClsid, NULL);
    //stat = image->Save(L"d:\\alik.jpg", &encoderClsid, NULL);


       if(stat == Ok)
       {
           LARGE_INTEGER seekPos = {0};
             ULARGE_INTEGER imageSize;
             hr = stream->Seek(seekPos, STREAM_SEEK_CUR, &imageSize);
             assert(hr == S_OK && imageSize.HighPart == 0);
            *len=imageSize.LowPart;
             hr = stream->Seek(seekPos, STREAM_SEEK_SET, &imageSize);
            stream->Read(output,*len,0);
          // printf("Bird.png was saved successfully\n");
       }
       else
       {
          //printf("Failure: stat = %d %d\n", stat,GetLastError());
       }

       delete image;


}
//===========================================================================================================
 BOOL ImageUtil::DrawBitmap (HWND hwnd, HBITMAP hBitmap)
 {
     HDC       hDCBits;
     BITMAP    Bitmap;
     BOOL      bResult;

     if (!hwnd || !hBitmap)
         return FALSE;
    auto hDC=GetDC(hwnd);
    RECT s;
    GetWindowRect(hwnd,&s);


     hDCBits = CreateCompatibleDC(hDC);
     GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
     SelectObject(hDCBits, hBitmap);
     bResult = BitBlt(hDC, 0,0, Bitmap.bmWidth, Bitmap.bmHeight, hDCBits, 0, 0, SRCCOPY);
      InvalidateRect(hwnd,&s,TRUE);



     DeleteDC(hDCBits);

     return bResult;
 }
