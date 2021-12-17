#include "IDGXIFrameGenerator.hpp"

#include "frameManager.hpp"
#include "util/IDGXI/OutputManager.h"
#include "util/IDGXI/ThreadManager.h"

typedef struct
{
    UINT    WaitTime;
    UINT    WaitCount;
}WAIT_BAND;

#define WAIT_BAND_COUNT 3
#define WAIT_BAND_STOP 0

class DYNAMIC_WAIT
{
public:
    DYNAMIC_WAIT();
    ~DYNAMIC_WAIT();

    void Wait();

private:

    static const WAIT_BAND   m_WaitBands[WAIT_BAND_COUNT];

    // Period in seconds that a new wait call is considered part of the same wait sequence
    static const UINT       m_WaitSequenceTimeInSeconds = 2;

    UINT                    m_CurrentWaitBandIdx;
    UINT                    m_WaitCountInCurrentBand;
    LARGE_INTEGER           m_QPCFrequency;
    LARGE_INTEGER           m_LastWakeUpTime;
    BOOL                    m_QPCValid;
};
const WAIT_BAND DYNAMIC_WAIT::m_WaitBands[WAIT_BAND_COUNT] = {
                                                                 {250, 20},
                                                                 {2000, 60},
                                                                 {5000, WAIT_BAND_STOP}   // Never move past this band
};

DYNAMIC_WAIT::DYNAMIC_WAIT() : m_CurrentWaitBandIdx(0), m_WaitCountInCurrentBand(0)
{
    m_QPCValid = QueryPerformanceFrequency(&m_QPCFrequency);
    m_LastWakeUpTime.QuadPart = 0L;
}

DYNAMIC_WAIT::~DYNAMIC_WAIT()
{
}

void DYNAMIC_WAIT::Wait()
{
    LARGE_INTEGER CurrentQPC = { 0 };

    // Is this wait being called with the period that we consider it to be part of the same wait sequence
    QueryPerformanceCounter(&CurrentQPC);
    if (m_QPCValid && (CurrentQPC.QuadPart <= (m_LastWakeUpTime.QuadPart + (m_QPCFrequency.QuadPart * m_WaitSequenceTimeInSeconds))))
    {
        // We are still in the same wait sequence, lets check if we should move to the next band
        if ((m_WaitBands[m_CurrentWaitBandIdx].WaitCount != WAIT_BAND_STOP) && (m_WaitCountInCurrentBand > m_WaitBands[m_CurrentWaitBandIdx].WaitCount))
        {
            m_CurrentWaitBandIdx++;
            m_WaitCountInCurrentBand = 0;
        }
    }
    else
    {
        // Either we could not get the current time or we are starting a new wait sequence
        m_WaitCountInCurrentBand = 0;
        m_CurrentWaitBandIdx = 0;
    }

    // Sleep for the required period of time
    Sleep(m_WaitBands[m_CurrentWaitBandIdx].WaitTime);

    // Record the time we woke up so we can detect wait sequences
    QueryPerformanceCounter(&m_LastWakeUpTime);
    m_WaitCountInCurrentBand++;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE:
    {
        // Tell output manager that window size has changed
      //  OutMgr.WindowResize();
        break;
    }
    case WM_DISPLAYCHANGE:
	    {
        ProcessFailure(nullptr, L"ExpectedErrorEvent creation failed", L"Error", E_UNEXPECTED);
	    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
//=============================================================================================
int IDGXIFrameGenerator::StartWork()
{
   
      


    return 0;
}
//=============================================================================================
void IDGXIFrameGenerator::Run()
{
    if (WaitForSingleObjectEx(UnexpectedErrorEvent, 0, FALSE) == WAIT_OBJECT_0)
    {
        // Unexpected error occurred so exit the application
        throw std::exception{ "UnexpectedErrorEvent" };

    }
    if (WaitForSingleObjectEx(ExpectedErrorEvent, 0, FALSE) == WAIT_OBJECT_0)
    {
        InitDX();
    }

    auto res = OutMgr.UpdateFrameBuffer();

	
}
//=============================================================================================
void IDGXIFrameGenerator::Start(QString s)
{
    InitDX();
    IDeamon::Start(s);
}
//=============================================================================================
void IDGXIFrameGenerator::Stop()
{
    
    // Make sure all other threads have exited
    if (SetEvent(TerminateThreadsEvent))
    {
        ThreadMgr.WaitForThreadTermination();
    }
    IDeamon::Stop();
}

//=============================================================================================
IDGXIFrameGenerator::IDGXIFrameGenerator()
{
    
    // Event used by the threads to signal an unexpected error and we want to quit the app
    UnexpectedErrorEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (!UnexpectedErrorEvent)
    {
        ProcessFailure(nullptr, L"UnexpectedErrorEvent creation failed", L"Error", E_UNEXPECTED);
        // return 0;
    }

    // Event for when a thread encounters an expected error
    ExpectedErrorEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (!ExpectedErrorEvent)
    {
        ProcessFailure(nullptr, L"ExpectedErrorEvent creation failed", L"Error", E_UNEXPECTED);
        // return 0;
    }

    // Event to tell spawned threads to quit
    TerminateThreadsEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (!TerminateThreadsEvent)
    {
        ProcessFailure(nullptr, L"TerminateThreadsEvent creation failed", L"Error", E_UNEXPECTED);
        //  return 0;
    }

}
//=============================================================================================
IDGXIFrameGenerator::~IDGXIFrameGenerator()
{
    CloseHandle(UnexpectedErrorEvent);
    CloseHandle(ExpectedErrorEvent);
    CloseHandle(TerminateThreadsEvent);
}

//=====================================================================================================================

void IDGXIFrameGenerator::InitDX()
{
    INT SingleOutput = -1;
    DYNAMIC_WAIT DynamicWait;
    RECT DeskBounds;
    UINT OutputCount;
    SetEvent(TerminateThreadsEvent);
    ThreadMgr.WaitForThreadTermination();
    ResetEvent(TerminateThreadsEvent);
    ResetEvent(ExpectedErrorEvent);

    // Clean up
    ThreadMgr.Clean();
    OutMgr.CleanRefs();

    // As we have encountered an error due to a system transition we wait before trying again, using this dynamic wait
    // the wait periods will get progressively long to avoid wasting too much system resource if this state lasts a long time
    DynamicWait.Wait();
	
   auto Ret = OutMgr.InitOutput(SingleOutput, &OutputCount, &DeskBounds);
   if (Ret == DUPL_RETURN_SUCCESS)
   {
       HANDLE SharedHandle = OutMgr.GetSharedHandle();
       if (SharedHandle)
       {
           Ret = ThreadMgr.Initialize(SingleOutput, OutputCount, UnexpectedErrorEvent, ExpectedErrorEvent, TerminateThreadsEvent, SharedHandle, &DeskBounds);
         if  (Ret == DUPL_RETURN_SUCCESS)
       	{
             return;
       	}
       }
   }

   throw std::exception{ "InitDX" };
	
}

DWORD WINAPI DDProc(_In_ void* Param)
{
    // Classes
    DISPLAYMANAGER DispMgr;
    DUPLICATIONMANAGER DuplMgr;

    // D3D objects
    ID3D11Texture2D* SharedSurf = nullptr;
    IDXGIKeyedMutex* KeyMutex = nullptr;

    // Data passed in from thread creation
    THREAD_DATA* TData = reinterpret_cast<THREAD_DATA*>(Param);

    // Get desktop
    DUPL_RETURN Ret;
    HRESULT hr;
    HDESK CurrentDesktop = nullptr;
    bool DesktopAttached = false;
    bool WaitToProcessCurrentFrame = false;
    CurrentDesktop = OpenInputDesktop(0, FALSE, GENERIC_ALL);
    if (!CurrentDesktop)
    {
        // We do not have access to the desktop so request a retry
        SetEvent(TData->ExpectedErrorEvent);
        Ret = DUPL_RETURN_ERROR_EXPECTED;
        goto Exit;
    }

    // Attach desktop to this thread
     DesktopAttached = SetThreadDesktop(CurrentDesktop) != 0;
    CloseDesktop(CurrentDesktop);
    CurrentDesktop = nullptr;
    if (!DesktopAttached)
    {
        // We do not have access to the desktop so request a retry
        Ret = DUPL_RETURN_ERROR_EXPECTED;
        goto Exit;
    }

    // New display manager
    DispMgr.InitD3D(&TData->DxRes);

    // Obtain handle to sync shared Surface
     hr = TData->DxRes.Device->OpenSharedResource(TData->TexSharedHandle, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&SharedSurf));
    if (FAILED(hr))
    {
        Ret = ProcessFailure(TData->DxRes.Device, L"Opening shared texture failed", L"Error", hr, SystemTransitionsExpectedErrors);
        goto Exit;
    }

    hr = SharedSurf->QueryInterface(__uuidof(IDXGIKeyedMutex), reinterpret_cast<void**>(&KeyMutex));
    if (FAILED(hr))
    {
        Ret = ProcessFailure(nullptr, L"Failed to get keyed mutex interface in spawned thread", L"Error", hr);
        goto Exit;
    }

    // Make duplication manager
    Ret = DuplMgr.InitDupl(TData->DxRes.Device, TData->Output);
    if (Ret != DUPL_RETURN_SUCCESS)
    {
        goto Exit;
    }

    // Get output description
    DXGI_OUTPUT_DESC DesktopDesc;
    RtlZeroMemory(&DesktopDesc, sizeof(DXGI_OUTPUT_DESC));
    DuplMgr.GetOutputDesc(&DesktopDesc);

    // Main duplication loop
  
    FRAME_DATA CurrentData;

    while ((WaitForSingleObjectEx(TData->TerminateThreadsEvent, 0, FALSE) == WAIT_TIMEOUT))
    {
        if (!WaitToProcessCurrentFrame)
        {
            // Get new frame from desktop duplication
            bool TimeOut;
            Ret = DuplMgr.GetFrame(&CurrentData, &TimeOut);
            if (Ret != DUPL_RETURN_SUCCESS)
            {
                // An error occurred getting the next frame drop out of loop which
                // will check if it was expected or not
                break;
            }

            // Check for timeout
            if (TimeOut)
            {
                // No new frame at the moment
                continue;
            }
        }

        // We have a new frame so try and process it
        // Try to acquire keyed mutex in order to access shared surface
        hr = KeyMutex->AcquireSync(0, 1000);
        if (hr == static_cast<HRESULT>(WAIT_TIMEOUT))
        {
            // Can't use shared surface right now, try again later
            WaitToProcessCurrentFrame = true;
            continue;
        }
        else if (FAILED(hr))
        {
            // Generic unknown failure
            Ret = ProcessFailure(TData->DxRes.Device, L"Unexpected error acquiring KeyMutex", L"Error", hr, SystemTransitionsExpectedErrors);
            DuplMgr.DoneWithFrame();
            break;
        }

        // We can now process the current frame
        WaitToProcessCurrentFrame = false;

        // Get mouse info
        Ret = DuplMgr.GetMouse(TData->PtrInfo, &(CurrentData.FrameInfo), TData->OffsetX, TData->OffsetY);
        if (Ret != DUPL_RETURN_SUCCESS)
        {
            DuplMgr.DoneWithFrame();
            KeyMutex->ReleaseSync(1);
            break;
        }

        // Process new frame
        Ret = DispMgr.ProcessFrame(&CurrentData, SharedSurf, TData->OffsetX, TData->OffsetY, &DesktopDesc);
        if (Ret != DUPL_RETURN_SUCCESS)
        {
            DuplMgr.DoneWithFrame();
            KeyMutex->ReleaseSync(1);
            break;
        }

        // Release acquired keyed mutex
        hr = KeyMutex->ReleaseSync(1);
        if (FAILED(hr))
        {
            Ret = ProcessFailure(TData->DxRes.Device, L"Unexpected error releasing the keyed mutex", L"Error", hr, SystemTransitionsExpectedErrors);
            DuplMgr.DoneWithFrame();
            break;
        }

        // Release frame back to desktop duplication
        //convert to jpg
        //
      //  


    	//
        Ret = DuplMgr.DoneWithFrame();
        if (Ret != DUPL_RETURN_SUCCESS)
        {
            break;
        }
    }

Exit:
    if (Ret != DUPL_RETURN_SUCCESS)
    {
        if (Ret == DUPL_RETURN_ERROR_EXPECTED)
        {
            // The system is in a transition state so request the duplication be restarted
            SetEvent(TData->ExpectedErrorEvent);
        }
        else
        {
            // Unexpected error so exit the application
            SetEvent(TData->UnexpectedErrorEvent);
        }
    }

    if (SharedSurf)
    {
        SharedSurf->Release();
        SharedSurf = nullptr;
    }

    if (KeyMutex)
    {
        KeyMutex->Release();
        KeyMutex = nullptr;
    }

    return 0;
}