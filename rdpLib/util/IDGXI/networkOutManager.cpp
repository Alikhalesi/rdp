#include "networkOutManager.hpp"


#include "managers/frameManager.hpp"
#include "util/imageUtil.hpp"
//====================================================================================
DUPL_RETURN NetworkOutmanager::UpdateFrameBuffer(PTR_INFO* PointerInfo)
{
    HRESULT hr = m_KeyMutex->AcquireSync(1, INFINITE);
   if (FAILED(hr))
    {
        return ProcessFailure(m_Device, L"Failed to acquire Keyed mutex in OUTPUTMANAGER", L"Error", hr, SystemTransitionsExpectedErrors);
    }
    {
     auto frameWriter=   FrameManager::GetInstance().GetBufferForWrite();
     unsigned int len=-1;
     ImageUtil::SaveTextureToJpg(*frameWriter, &len, m_SharedSurf);
     frameWriter.SetFrameLen(len);
    }
    // Release keyed mutex
    hr = m_KeyMutex->ReleaseSync(0);
    if (FAILED(hr))
    {
        return ProcessFailure(m_Device, L"Failed to Release Keyed mutex in OUTPUTMANAGER", L"Error", hr, SystemTransitionsExpectedErrors);
    }
    return DUPL_RETURN_SUCCESS;
}
//====================================================================================