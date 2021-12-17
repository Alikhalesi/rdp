#include "networkOutManager.hpp"
//====================================================================================
DUPL_RETURN NetworkOutmanager::UpdateFrameBuffer() const
{
    HRESULT hr = m_KeyMutex->AcquireSync(1, INFINITE);
   if (FAILED(hr))
    {
        return ProcessFailure(m_Device, L"Failed to acquire Keyed mutex in OUTPUTMANAGER", L"Error", hr, SystemTransitionsExpectedErrors);
    }
    {
     auto frameWriter=   FrameManager::GetInstance().GetBufferForWrite();
     unsigned int len = FrameManager::GetInstance().AllocationLen();
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