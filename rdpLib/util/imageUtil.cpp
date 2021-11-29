#include "imageUtil.hpp"
using namespace Gdiplus;
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
 void ImageUtil::ConvertImage(const std::string &destFormat, HBITMAP input,unsigned char* output,unsigned int* len)
{
    // Initialize GDI+.
       GdiplusStartupInput gdiplusStartupInput;
       ULONG_PTR gdiplusToken;
       GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

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

       GdiplusShutdown(gdiplusToken);

}
