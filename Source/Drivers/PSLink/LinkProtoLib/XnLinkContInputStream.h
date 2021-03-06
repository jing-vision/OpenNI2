#ifndef __XNLINKCONTINPUTSTREAM_H__
#define __XNLINKCONTINPUTSTREAM_H__

#include "XnLinkMsgParser.h"
#include "XnLinkInputStream.h"
#include <XnStatus.h>
#include <XnEvent.h>
#include <XnDump.h>

#include "XnLinkLogParser.h"

typedef XnUInt32 XnStreamFormat;

namespace xn
{

class LinkContInputStream : public LinkInputStream
{
public:
	LinkContInputStream();
	virtual ~LinkContInputStream();
    virtual XnStatus Init(LinkControlEndpoint* pLinkControlEndpoint,
                          XnStreamType streamType,
                          XnUInt16 nStreamID, 
                          IConnection* pConnection);
    //LinkInputStream methods
    virtual XnBool IsInitialized() const;
	virtual void Shutdown();
	virtual XnStatus HandlePacket(const LinkPacketHeader& header, const XnUInt8* pData, XnBool& bPacketLoss);
	virtual const void* GetData() const;
	virtual XnUInt32 GetDataSize() const;
	virtual XnUInt64 GetTimestamp() const { return 0; }
	virtual const void* GetNextData() const;
	virtual XnUInt32 GetNextDataSize() const;
	virtual XnUInt64 GetNextTimestamp() const { return 0; }
	virtual XnBool IsNewDataAvailable() const;
	virtual XnStatus UpdateData();

    virtual XnBool IsStreaming() const;

	virtual XnStreamFragLevel GetStreamFragLevel() const { return XN_LINK_STREAM_FRAG_LEVEL_CONTINUOUS; }

	typedef void (XN_CALLBACK_TYPE* NewDataAvailableHandler)(void* pCookie);
	virtual XnStatus RegisterToNewDataAvailable(NewDataAvailableHandler pHandler, void* pCookie, XnCallbackHandle& hCallback);
	virtual void UnregisterFromNewDataAvailable(XnCallbackHandle hCallback);

    //Other methods
    virtual void SetDumpName(const XnChar* strDumpName);
    virtual void SetDumpOn(XnBool bDumpOn);

protected:
	virtual XnStatus StartImpl();
	virtual XnStatus StopImpl();

private:
	LinkLogParser m_logParser;

	static const XnUInt32 CONT_STREAM_PREDEFINED_BUFFER_SIZE;
	mutable XN_CRITICAL_SECTION_HANDLE m_hCriticalSection; //Protects buffers info
	XnBool m_bNewDataAvailable;
    XnBool m_bInitialized;
	XnBool m_bStreaming;
	
	XnUInt32 m_nUserBufferMaxSize;
	XnUInt32 m_nUserBufferCurrentSize;
	XnUInt8* m_pUserBuffer;

	XnUInt32 m_nWorkingBufferCurrentSize;
	XnUInt8* m_pWorkingBuffer;

	XnChar m_strDumpName[XN_FILE_MAX_PATH];
	XnDumpFile* m_pDumpFile;

	xnl::EventNoArgs m_newDataAvailableEvent;
};

}

#endif // __XNLINKCONTINPUTSTREAM_H__