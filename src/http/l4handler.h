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

#ifndef L4TUNNEL_H
#define L4TUNNEL_H

#include <http/hiostream.h>
#include <http/httpreq.h>
#include <extensions/l4conn.h>
#include <util/loopbuf.h>
#include "socket/gsockaddr.h"

class L4Handler : public HioStreamHandler
{

public:
    L4Handler();
    ~L4Handler() {};

    int  init(HttpReq &req, const GSockAddr *pGSockAddr);
    
    LoopBuf*    getBuf()            {   return m_buf;  }
    void        continueRead()      {   getStream()->continueRead();   }
    void        suspendRead()       {   getStream()->suspendRead();   }
    void        suspendWrite()      {   getStream()->suspendWrite();    }
    void        continueWrite()     {   getStream()->continueWrite();    }
    
    void        doWrite();
    void        closeBothConnection();
    
private:
    L4conn  *       m_pL4conn;
    LoopBuf *       m_buf;
    int             m_iState;

    
    void recycle();
    int onTimerEx()         {   return 0;   }
    int onCloseEx()         {   return 0;   }
    int onWriteEx();
    int onReadEx();
    int onInitConnected()   {   return 0;   };
    
public:
    LOG4CXX_NS::Logger* getLogger() const   {   return getStream()->getLogger();   }
    const char * getLogId() {   return getStream()->getLogId();     }
   
};

#endif // L4TUNNEL_H
