#include "mallocIstream.hpp"

MallocIStream::MallocIStream(void* buffer, unsigned len, bool owner)
	:buffer_{ buffer }, len_{ len }, owner_{ owner }
{
}
//=====================================================================================
MallocIStream::~MallocIStream()
{
	if (owner_)
	{
		free(buffer_);
	}
}
//=====================================================================================
HRESULT MallocIStream::QueryInterface(const IID& iid, void** ppvObject)
{
	if (iid == __uuidof(IUnknown)
		|| iid == __uuidof(IStream)
		|| iid == __uuidof(ISequentialStream))
	{
		*ppvObject = static_cast<IStream*>(this);
		AddRef();
		return S_OK;
	}
	else
		return E_NOINTERFACE;
}
//=====================================================================================
ULONG MallocIStream::AddRef()
{
	return (ULONG)InterlockedIncrement(&refCount_);
}
//=====================================================================================
ULONG MallocIStream::Release()
{
	ULONG res = (ULONG)InterlockedDecrement(&refCount_);
	if (res == 0)
		delete this;
	return res;

}
//=====================================================================================
HRESULT MallocIStream::Read(void* pv, ULONG cb, ULONG* pcbRead)
{
	unsigned int readed;
	if (cb <= len_ - currentCursor_)
	{
		readed = cb;
	}
	else
	{
		readed = len_ - currentCursor_;
	}
	memcpy(pv, static_cast<char*>(buffer_) + currentCursor_, readed);
	if (pcbRead != nullptr)
	{
		*pcbRead = readed;
	}
	currentCursor_ += readed;
	return S_OK;
}
//=====================================================================================
HRESULT MallocIStream::Write(const void* pv, ULONG cb, ULONG* pcbWritten)
{
	unsigned int written;
	if (cb <= len_ - currentCursor_)
	{
		written = cb;
	}
	else
	{
		written = len_ - currentCursor_;
	}

	memcpy(static_cast<char*>(buffer_) + currentCursor_, pv, written);
	if (pcbWritten != nullptr)
	{
		*pcbWritten = written;
	}
	currentCursor_ += written;
	return S_OK;
}
//=====================================================================================
HRESULT MallocIStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition)
{
	switch (dwOrigin)
	{
	case STREAM_SEEK_SET:
	{
			
		if (dlibMove.QuadPart <= static_cast<LONGLONG>(len_))
		{
			currentCursor_ = dlibMove.QuadPart;
			if (plibNewPosition != nullptr)
			{
				plibNewPosition->QuadPart= currentCursor_;
			}
			return S_OK;
		}

		return STG_E_INVALIDFUNCTION;


	}
	case STREAM_SEEK_CUR:
	{
			
		if (dlibMove.QuadPart == 0)
		{
			if (plibNewPosition != nullptr)
			{
				plibNewPosition->QuadPart = currentCursor_;
			}
			return S_OK;
		}
		if (dlibMove.QuadPart > 0)
		{

			if (dlibMove.QuadPart <= static_cast<LONGLONG>(len_ - currentCursor_))
			{
				currentCursor_ += dlibMove.QuadPart;
			}
			else
			{
				return STG_E_INVALIDFUNCTION;
			}
			if (plibNewPosition != nullptr)
			{
				plibNewPosition->QuadPart = currentCursor_;
			}
		}
		else
		{
			if (dlibMove.QuadPart+static_cast<LONGLONG>(currentCursor_) <0)
			{
				return STG_E_INVALIDFUNCTION;
			}
			else
			{
				currentCursor_ += dlibMove.QuadPart;
			}
			if (plibNewPosition != nullptr)
			{
				plibNewPosition->QuadPart = currentCursor_;
			}
		}
		


		return S_OK;

	}
	case STREAM_SEEK_END:
	{

		if (dlibMove.QuadPart <=static_cast<LONGLONG>(len_ - currentCursor_))
		{
			currentCursor_ += dlibMove.QuadPart;
		}
		else
		{
			return STG_E_INVALIDFUNCTION;
		}
		if (plibNewPosition != nullptr)
		{
			plibNewPosition->QuadPart = currentCursor_;
		}


		return S_OK;
	}
	default:
		return STG_E_INVALIDFUNCTION;

	}
}
//=====================================================================================
HRESULT MallocIStream::SetSize(ULARGE_INTEGER libNewSize)
{
	return E_NOTIMPL;
}
//=====================================================================================
HRESULT MallocIStream::CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten)
{
	return E_NOTIMPL;
}
//=====================================================================================
HRESULT MallocIStream::Commit(DWORD grfCommitFlags)
{
	return S_OK;
}
//=====================================================================================
HRESULT MallocIStream::Revert()
{
	return S_OK;
}
//=====================================================================================
HRESULT MallocIStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	return E_NOTIMPL;
}
//=====================================================================================
HRESULT MallocIStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
	return E_NOTIMPL;
}
//=====================================================================================
HRESULT MallocIStream::Stat(STATSTG* pstatstg, DWORD grfStatFlag)
{
	if (pstatstg != nullptr)
	{
		pstatstg->cbSize.QuadPart = len_;
		
	}
	return S_OK;
}
//=====================================================================================
HRESULT MallocIStream::Clone(IStream** ppstm)
{
	return E_NOTIMPL;
}
//=====================================================================================

