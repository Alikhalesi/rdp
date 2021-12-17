#pragma once
#include <exception>
#include <shlwapi.h>

class MallocIStream:public IStream
{
public :
	MallocIStream(void* buffer, unsigned int len,bool owner=false);
	
	virtual ~MallocIStream();
	HRESULT QueryInterface(const IID& riid, void** ppvObject) override;
	ULONG AddRef() override;
	ULONG Release() override;
	HRESULT Read(void* pv, ULONG cb, ULONG* pcbRead) override;
	HRESULT Write(const void* pv, ULONG cb, ULONG* pcbWritten) override;
	HRESULT Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition) override;
	HRESULT SetSize(ULARGE_INTEGER libNewSize) override;
	HRESULT CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten) override;
	HRESULT Commit(DWORD grfCommitFlags) override;
	HRESULT Revert() override;
	HRESULT LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) override;
	HRESULT UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) override;
	HRESULT Stat(STATSTG* pstatstg, DWORD grfStatFlag) override;
	HRESULT Clone(IStream** ppstm) override;
private:
	void* buffer_=nullptr;
	size_t len_ = -1;
	bool owner_ = false;
	ULONG refCount_ = 1;
	size_t currentCursor_ = 0;
	
};