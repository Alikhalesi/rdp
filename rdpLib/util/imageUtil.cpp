#include "imageUtil.hpp"
#include <gdiplus.h>
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
   // auto hr= CreateStreamOnHGlobal(input,false,&srcstream);
    HRESULT hr;
    srcstream = SHCreateMemStream(input, len);
    Image*   image = Image::FromStream(srcstream);
    qDebug()<<"stream img height: "<< image->GetHeight();


    CComPtr<IStream> desstream=SHCreateMemStream(nullptr,0);


    GetEncoderClsid(L"image/bmp", &encoderClsid);
   // image->Save(L"d:\\bmp.bmp", &encoderClsid);
    stat = image->Save(desstream, &encoderClsid, NULL);
   // assert(stat==S_OK);

      auto* resultBmp=  Bitmap::FromStream(desstream);
       Color color;
      // resultBmp->Save(L"d:\\bxmp.bmp", &encoderClsid);
       HBITMAP bitmap;
       stat= resultBmp->GetHBITMAP(color,&bitmap);
      //  assert(stat==S_OK);

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
//auto hr= CreateStreamOnHGlobal(output,false,&stream);
       HRESULT hr;
       stream = SHCreateMemStream(nullptr, 0);
//assert(hr==S_OK);


       stat = image->Save(stream, &encoderClsid, NULL);
    //stat = image->Save(L"d:\\alik.jpg", &encoderClsid, NULL);


       if(stat == Ok)
       {
           LARGE_INTEGER seekPos = {0};
             ULARGE_INTEGER imageSize;
             hr = stream->Seek(seekPos, STREAM_SEEK_CUR, &imageSize);
            // assert(hr == S_OK && imageSize.HighPart == 0);
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
 //==================================================================================================
 void ImageUtil::SaveTextureToJpg(unsigned char* input,unsigned int* len, ID3D11Texture2D* Texture)
 {
     HRESULT hr;

     // First verify that we can map the texture
     D3D11_TEXTURE2D_DESC desc;
     Texture->GetDesc(&desc);

     // translate texture format to WIC format. We support only BGRA and ARGB.
     GUID wicFormatGuid;
     switch (desc.Format) {
     case DXGI_FORMAT_R8G8B8A8_UNORM:
         wicFormatGuid = GUID_WICPixelFormat32bppRGBA;
         break;
     case DXGI_FORMAT_B8G8R8A8_UNORM:
         wicFormatGuid = GUID_WICPixelFormat32bppBGRA;
         break;
     default:
         throw(
             HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED),
             L"Unsupported DXGI_FORMAT: %d. Only RGBA and BGRA are supported.",
             desc.Format);
     }

     // Get the device context
    CComPtr<ID3D11Device> d3dDevice;
     Texture->GetDevice(&d3dDevice);
    CComPtr<ID3D11DeviceContext> d3dContext;
     d3dDevice->GetImmediateContext(&d3dContext);

     // map the texture
    CComPtr<ID3D11Texture2D> mappedTexture;
     D3D11_MAPPED_SUBRESOURCE mapInfo;
     mapInfo.RowPitch;
     hr = d3dContext->Map(
         Texture,
         0,  // Subresource
         D3D11_MAP_READ,
         0,  // MapFlags
         &mapInfo);

     if (FAILED(hr)) {
         // If we failed to map the texture, copy it to a staging resource
         if (hr == E_INVALIDARG) {
             D3D11_TEXTURE2D_DESC desc2;
             desc2.Width = desc.Width;
             desc2.Height = desc.Height;
             desc2.MipLevels = desc.MipLevels;
             desc2.ArraySize = desc.ArraySize;
             desc2.Format = desc.Format;
             desc2.SampleDesc = desc.SampleDesc;
             desc2.Usage = D3D11_USAGE_STAGING;
             desc2.BindFlags = 0;
             desc2.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
             desc2.MiscFlags = 0;

            CComPtr<ID3D11Texture2D> stagingTexture;
             hr = d3dDevice->CreateTexture2D(&desc2, nullptr, &stagingTexture);
             if (FAILED(hr)) {
                 throw std::exception("Failed to create staging texture");
             }

             // copy the texture to a staging resource
             d3dContext->CopyResource(stagingTexture, Texture);

             // now, map the staging resource
             hr = d3dContext->Map(
                 stagingTexture,
                 0,
                 D3D11_MAP_READ,
                 0,
                 &mapInfo);
             if (FAILED(hr)) {
                 throw std::exception("Failed to map staging texture");
             }

             mappedTexture = std::move(stagingTexture);
         }
         else {
             throw std::exception("Failed to map texture.");
         }
     }
     else {
         mappedTexture = Texture;
     }
     auto unmapResource = ([&] {
         d3dContext->Unmap(mappedTexture, 0);
         });

    CComPtr<IWICImagingFactory> wicFactory;
     hr = CoCreateInstance(
         CLSID_WICImagingFactory,
         nullptr,
         CLSCTX_INPROC_SERVER,
         __uuidof(wicFactory),
         reinterpret_cast<void**>(&wicFactory));
     if (FAILED(hr)) {
         throw std::exception(
             
             "Failed to create instance of WICImagingFactory");
     }

    CComPtr<IWICBitmapEncoder> wicEncoder;
     hr = wicFactory->CreateEncoder(
      //GUID_ContainerFormatBmp,
        GUID_ContainerFormatJpeg,
         nullptr,
         &wicEncoder);
     if (FAILED(hr)) {
         throw std::exception("Failed to create BMP encoder");
     }
     //
     CComPtr<IStream> srcstream=nullptr;
    srcstream =SHCreateMemStream(nullptr, 0);
   //  hr = CreateStreamOnHGlobal(input, false, &srcstream);

	//
    CComPtr<IWICStream> wicStream;
     hr = wicFactory->CreateStream(&wicStream);
     if (FAILED(hr)) {
         throw std::exception( "Failed to create IWICStream");
     }
  
         hr = wicStream->InitializeFromIStream(srcstream);
     if (FAILED(hr)) {
         throw std::exception( "Failed to initialize stream from file name");
     }

     hr = wicEncoder->Initialize(wicStream, WICBitmapEncoderNoCache);
     if (FAILED(hr)) {
         throw std::exception( "Failed to initialize bitmap encoder");
     }
     IPropertyBag2* pPropertybag = NULL;
     // Encode and commit the frame
     {
	    CComPtr<IWICBitmapFrameEncode> frameEncode;
         wicEncoder->CreateNewFrame(&frameEncode, &pPropertybag);
         if (FAILED(hr)) {
             throw std::exception( "Failed to create IWICBitmapFrameEncode");
         }
     
         //if (SUCCEEDED(hr))
         //{
         //    // This is how you customize the TIFF output.
         //    PROPBAG2 option = { 0 };
         //    wchar_t ppname[20]={0};
         //	
         //    wcscpy_s(ppname, L"ImageQuality");
         //	
         //    option.pstrName = ppname;
 
         //    VariantInit(&varValue);
         //    varValue.vt 
         //    varValue.bVal = 1;
         //    hr = pPropertybag->Write(1, &option, &varValue);
         //    if (SUCCEEDED(hr))
         //    {
         //        hr = frameEncode->Initialize(pPropertybag);
         //    }
         //}

     	
        hr = frameEncode->Initialize(nullptr);
         if (FAILED(hr)) {
             throw std::exception( "Failed to initialize IWICBitmapFrameEncode");
         }


         hr = frameEncode->SetPixelFormat(&wicFormatGuid);
         if (FAILED(hr)) {
             throw std::exception(
                 
                 "SetPixelFormat(%s) failed.");
         }

         hr = frameEncode->SetSize(desc.Width, desc.Height);
         if (FAILED(hr)) {
             throw std::exception( "SetSize(...) failed.");
         }

         hr = frameEncode->WritePixels(
             desc.Height,
             mapInfo.RowPitch,
             desc.Height * mapInfo.RowPitch,
             reinterpret_cast<BYTE*>(mapInfo.pData));
         if (FAILED(hr)) {
             throw std::exception( "frameEncode->WritePixels(...) failed.");
         }

         hr = frameEncode->Commit();
         if (FAILED(hr)) {
             throw std::exception( "Failed to commit frameEncode");
         }
     }

     hr = wicEncoder->Commit();
     if (FAILED(hr)) {
         throw std::exception( "Failed to commit encoder");
     }


	//get len
     LARGE_INTEGER seekPos = { 0 };
     ULARGE_INTEGER imageSize;
     hr = srcstream->Seek(seekPos, STREAM_SEEK_CUR, &imageSize);
   //  assert(hr == S_OK && imageSize.HighPart == 0);
     *len = imageSize.LowPart;
     hr = srcstream->Seek(seekPos, STREAM_SEEK_SET, &imageSize);

     srcstream->Read(input, *len, 0);
 //    ImageUtil img3{};
 //    Image* img = Bitmap::FromStream(srcstream, false);
 //    CLSID   encoderClsid;
 //    auto x = img->GetHeight();
	//img3.GetEncoderClsid(L"image/jpeg", &encoderClsid);
 //    img->Save(L"d:\\rrr.jpg", &encoderClsid, NULL);
   //  srcstream->Read(output, *len, 0);
 }