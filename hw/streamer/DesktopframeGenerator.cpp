#include "DesktopFrameGenerator.hpp"




DesktopFrameGenerator::DesktopFrameGenerator()
{
    //stopped_.test_and_set();
}
//=============================================================================================================
void DesktopFrameGenerator::Start(QString cnnInfo)
{
    stopped_.test_and_set();
    while(stopped_.test_and_set())
    {
         QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

         if(!stopped_.test_and_set())
         {
             return;
         }

        //get desktop snapshot
        //emit signal
      //   Sleep(1000);
        UpdateCurrentFrame();

        //emit NewFrame(frameData.first,frameData.second);
       // qDebug()<<"frame generate\n";
    }
   // emit stopped();

}
//=============================================================================================================
void DesktopFrameGenerator::Stop()
{
    stopped_.clear();

}
//=============================================================================================================
void DesktopFrameGenerator::UpdateCurrentFrame()
{
    HDC hdcScreen;
       HDC hdcWindow;
       HDC hdcMemDC = NULL;
       HBITMAP hbmScreen = NULL;
       BITMAP bmpScreen;
       DWORD dwBytesWritten = 0;
       DWORD dwSizeofDIB = 0;
       HANDLE hFile = NULL;
       unsigned char* lpbitmap = NULL;
       HANDLE hDIB = NULL;
       DWORD dwBmpSize = 0;

       // Retrieve the handle to a display device context for the client
       // area of the window.
       auto hWnd=GetDesktopWindow();
       hdcScreen = GetDC(NULL);
       hdcWindow = GetDC(hWnd);

       // Create a compatible DC, which is used in a BitBlt from the window DC.
       hdcMemDC = CreateCompatibleDC(hdcWindow);

       if (!hdcMemDC)
       {
           MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
          // goto done;
       }

       // Get the client area for size calculation.
       RECT rcClient;
       GetClientRect(hWnd, &rcClient);

       // This is the best stretch mode.
       SetStretchBltMode(hdcWindow, HALFTONE);

       // The source DC is the entire screen, and the destination DC is the current window (HWND).

       if (!StretchBlt(hdcWindow,
           0, 0,
           rcClient.right, rcClient.bottom,
           hdcScreen,
           0, 0,
           GetSystemMetrics(SM_CXSCREEN),
           GetSystemMetrics(SM_CYSCREEN),
           SRCCOPY))
       {
           MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
           //goto done;
       }

       //write cursor to image
       CURSORINFO cursorInfo;
       cursorInfo.cbSize=sizeof (CURSORINFO);
       if(GetCursorInfo(&cursorInfo))
       {
          auto hicon=CopyIcon(cursorInfo.hCursor);
          ICONINFO iconInfo;

          if(GetIconInfo(hicon,&iconInfo))
          {
            int  x = cursorInfo.ptScreenPos.x - ((int)iconInfo.xHotspot);
            int  y = cursorInfo.ptScreenPos.y - ((int)iconInfo.yHotspot);

            DrawIcon(hdcWindow,x,y,hicon);

            }

       }

       // Create a compatible bitmap from the Window DC.
       hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

       if (!hbmScreen)
       {
           MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
           //goto done;
       }

       // Select the compatible bitmap into the compatible memory DC.
       SelectObject(hdcMemDC, hbmScreen);

       // Bit block transfer into our compatible memory DC.
       if (!BitBlt(hdcMemDC,
           0, 0,
           rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
           hdcWindow,
           0, 0,
           SRCCOPY))
       {
           MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
           //goto done;
       }

      qDebug()<<"image size Before convert: "<< FrameManager::GetInstance().GetFrameLen();
       auto frameWriter =FrameManager::GetInstance().GetBufferForWrite();
       lpbitmap=*frameWriter;
       unsigned int len=2;


       DeleteObject(hdcMemDC);
       ReleaseDC(NULL, hdcScreen);
       ReleaseDC(hWnd, hdcWindow);


       ImageUtil::ConvertImage("image/jpeg",hbmScreen,lpbitmap,&len);
       frameWriter.SetFrameLen(len);
       DeleteObject(hbmScreen);
qDebug()<<"image size after convert: "<<len;
return;






//       // Get the BITMAP from the HBITMAP.
//       GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

//       BITMAPFILEHEADER   bmfHeader;
//       BITMAPINFOHEADER   bi;

//       bi.biSize = sizeof(BITMAPINFOHEADER);
//       bi.biWidth = bmpScreen.bmWidth;
//       bi.biHeight = bmpScreen.bmHeight;
//       bi.biPlanes = 1;
//       bi.biBitCount = 32;
//       bi.biCompression = BI_RGB;
//       bi.biSizeImage = 0;
//       bi.biXPelsPerMeter = 0;
//       bi.biYPelsPerMeter = 0;
//       bi.biClrUsed = 0;
//       bi.biClrImportant = 0;

//       dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

//       // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that
//       // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc
//       // have greater overhead than HeapAlloc.
//       dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
//{
//       auto frameWriter =FrameManager::GetInstance().GetBufferForWrite();
//       lpbitmap=*frameWriter;


//              // Offset to where the actual bitmap bits start.
//       bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

//              // Size of the file.
//       bmfHeader.bfSize = dwSizeofDIB;

//              // bfType must always be BM for Bitmaps.
//       bmfHeader.bfType = 0x4D42; // BM.

//       memcpy(lpbitmap,&bmfHeader,sizeof (BITMAPFILEHEADER));
//       memcpy(lpbitmap+sizeof (BITMAPFILEHEADER),&bi,sizeof (BITMAPINFOHEADER));

//       // Gets the "bits" from the bitmap, and copies them into a buffer
//       // that's pointed to by lpbitmap.
//       GetDIBits(hdcWindow, hbmScreen, 0,
//           (UINT)bmpScreen.bmHeight,
//           lpbitmap+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER),
//           (BITMAPINFO*)&bi, DIB_RGB_COLORS);
//}
//      // A file is created, this is where we will save the screen capture.
////       hFile = CreateFile(L"captureqwsx.bmp",
////           GENERIC_WRITE,
////           0,
////           NULL,
////           CREATE_ALWAYS,
////           FILE_ATTRIBUTE_NORMAL, NULL);

////       // Add the size of the headers to the size of the bitmap to get the total file size.
////       dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

////       // Offset to where the actual bitmap bits start.
////       bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

////       // Size of the file.
////       bmfHeader.bfSize = dwSizeofDIB;

////       // bfType must always be BM for Bitmaps.
////       bmfHeader.bfType = 0x4D42; // BM.

////       WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
////       WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
////       WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

////     //   Unlock and Free the DIB from the heap.
////   //    GlobalUnlock(hDIB);
////     //  GlobalFree(hDIB);

////       // Close the handle for the file that was created.
////       CloseHandle(hFile);

//       // Clean up.
//  // done:
//       DeleteObject(hbmScreen);
//       DeleteObject(hdcMemDC);
//       ReleaseDC(NULL, hdcScreen);
//       ReleaseDC(hWnd, hdcWindow);


}
