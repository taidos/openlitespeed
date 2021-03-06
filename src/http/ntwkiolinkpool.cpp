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

#include "ntwkiolinkpool.h"
#include "ntwkiolink.h"

NtwkIoLinkPool::NtwkIoLinkPool()
{

}

NtwkIoLinkPool::NtwkIoLinkPool(const NtwkIoLinkPool& other)
{

}

NtwkIoLinkPool::~NtwkIoLinkPool()
{

}

void NtwkIoLinkPool::recycle( NtwkIOLink* pConn )
{   s_pool.recycle( pConn );    }

NtwkIOLink* NtwkIoLinkPool::get()
{
    return s_pool.get();
}

void NtwkIoLinkPool::recycle( NtwkIOLink** pConn, int n )
{   s_pool.recycle( (void **)pConn, n );    }

int NtwkIoLinkPool::get( NtwkIOLink** pConn, int n)
{
    return s_pool.get( pConn, n);
}
