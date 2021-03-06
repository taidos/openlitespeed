/*****************************************************************************
*    Open LiteSpeed is an open source HTTP server.                           *
*    Copyright (C) 2013  LiteSpeed Technologies, Inc.                        *
*                                                                            *
*    This program is free software: you can redistribute it and/or modify    *
*    it under the terms of the GNU General Public License as published by    *
*    the Free Software Foundation, either version 3 of the License, or       *
*    (at your option) any later version.                                     *
*                                                                            *
*    This program is distributed in the hope that it will be useful,         *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of          *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*    GNU General Public License for more details.                            *
*                                                                            *
*    You should have received a copy of the GNU General Public License       *
*    along with this program. If not, see http://www.gnu.org/licenses/.      *
*****************************************************************************/
#ifndef HTTPEXTCONNECTOR_H
#define HTTPEXTCONNECTOR_H



#include <extensions/extrequest.h>
#include <http/reqhandler.h>
#include <util/autobuf.h>
#include <log4cxx/ilog.h>


class HttpConnection;
class HttpExtProcessor;
class ExtWorker;
class ExtRequest;
class GzipBuf;
class VMemBuf;

#define HEC_BEGIN_REQUEST       0
#define HEC_FWD_REQ_HEADER      1
#define HEC_FWD_REQ_BODY        2
#define HEC_ABORT_REQUEST       4
#define HEC_FWD_RESP_HEADER     8
#define HEC_REQ_HEADER_DONE     (HEC_FWD_REQ_BODY | HEC_FWD_RESP_HEADER)
#define HEC_FWD_RESP_BODY       16
#define HEC_COMPLETE            32
#define HEC_ERROR               64
#define HEC_REDIRECT            128


class HttpExtConnector : public ReqHandler, public ExtRequest
{
protected:
    HttpConnection      * m_pHttpConn;
    HttpExtProcessor    * m_pProcessor;
    ExtWorker           * m_pWorker;
    AutoBuf               m_respHeaderBuf;
    int                   m_iState;
    int                   m_iRespState;
    int                   m_iReqBodySent;
    int                   m_iRespHeaderSize;
    int                   m_iRespBodyLen;
    int                   m_iRespBodySent;


    int sendReqBody();
    int sendReqHeader();
    void extProcessorError(int error);
    int releaseProcessor();

    int prepareRespHeader();
    int addToWriteBuffer( const char * pBuf, int len );
    int setupGzipBuf();
    
    int checkState();

    int sendRespBody( const char * pBuf, int size );
    int sendResp( );
    void testEndOfReqBody();
    int checkRespSize();
    void setHttpError( int error );
        
public:
    HttpExtConnector();
    virtual ~HttpExtConnector();

    void setState( int state ) { m_iState = state; }
    int getState() const       { return m_iState;  }
    void setProcessor( HttpExtProcessor * pProcessor )
    {   m_pProcessor = pProcessor; }
    void setHttpConn( HttpConnection  * pConn )
    {   m_pHttpConn = pConn;    }
    
    HttpExtProcessor * getProcessor() const
    {   return m_pProcessor;    }
    HttpConnection* getHttpConn() const
    {   return m_pHttpConn;     }

    ExtWorker* getWorker() const    {   return m_pWorker;   }
    void setWorker( ExtWorker * pWorker )
    {   m_pWorker = pWorker;    }
    
    void resetConnector();

    int& getRespState()             {   return m_iRespState;    }
    void setRespState( int state)   {   m_iRespState = state;   }
    
    //functions called by the HttpConnection
    void abortReq();
    //defined in ReqHandler
    virtual int process( HttpConnection* pConn, const HttpHandler * pHandler );
    virtual int onWrite( HttpConnection* pConn, int aioSent );
    virtual int cleanUp( HttpConnection* pConn );
    virtual int onRead( HttpConnection* pConn );
    virtual void onTimer();

    //functions called by the HttpExtProcessor
    virtual void extProcessorReady();
    virtual int processRespData( const char * pBuf, int len );
    virtual int processErrData( const char * pBuf, int len );
    virtual int endResponse( int endCode, int protocolStatus );
    virtual int errResponse( int code, const char * pErr );
    virtual int extOutputReady();
    virtual int extInputReady();
    virtual int processRespHeaderData( const char * pBuf, int len ) {return len;}
    virtual void dump();
    virtual int  dumpAborted();

    int  parseHeader( const char * &pBuf, int &len, int httpResp=0 );
    int  processRespBodyData( int inplace, const char * pBuf, int len );
    int  respHeaderDone(int len);
    bool isRecoverable();
    int  tryRecover();
    int  isAlive();
    void suspend();
        
    char * getRespBuf( size_t& len );
    int  respBodyRecv( const char * pBuf, size_t len )
    {   return processRespBodyData( 1, pBuf, len );     }
    
    int reqHeaderDone();
    int reqBodyDone();
    const char *  getLogId();  
    LOG4CXX_NS::Logger* getLogger() const;  
    bool wantWrite() const
    {   return (m_iState & (HEC_FWD_REQ_HEADER|HEC_FWD_REQ_BODY));  }
    int getRespBodyLen() const {   return m_iRespBodyLen;   }
    int flushResp();
};

#endif
