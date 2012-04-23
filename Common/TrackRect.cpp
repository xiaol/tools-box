// Copyright 2008 The Navi Library Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS-IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "stdafx.h"

#include "TrackRect.h"

//////////////////////////////////////////////////////////////////////////////
// CTrackRect - Implementation

CTrackRect::CTrackRect()
{
    // Set default value.
    m_BgnPoint.x = m_BgnPoint.y = 0;
    m_EndPoint.x = m_EndPoint.y = 0;
}

CTrackRect::~CTrackRect()
{
    m_pen.DeleteObject();
}

// Initialize object
void CTrackRect::Initialize( int nPenStyle, int nPenWidth, COLORREF crPenColor )
{
    m_pen.CreatePen( nPenStyle, nPenWidth, crPenColor );
}

// Set begin point of track rectangle.
void CTrackRect::SetBgnPoint( CPoint& bgnPoint )
{
    m_BgnPoint = bgnPoint;
    m_EndPoint = m_BgnPoint;
}

// Set end point of track rectangle.
void CTrackRect::SetEndPoint( CPoint& endPoint )
{
    m_EndPoint = endPoint;
}

// Draw track rectangle.
void CTrackRect::Draw( CDC* pDC )
{
    if ( pDC == NULL )
        return;

    // Set ROP mode and select new pen.
    int iOldMode = pDC->SetROP2( R2_NOTXORPEN );
	CPen* pOldPen = (CPen*)pDC->SelectObject( &m_pen ) ;
	pDC->Rectangle( m_BgnPoint.x, m_BgnPoint.y, m_EndPoint.x, m_EndPoint.y );

    // Revert old attributes for CDC.
    pDC->SelectObject( pOldPen );
    pDC->SetROP2( iOldMode );
}

// Erase track rectangle.
void CTrackRect::Erase( CDC* pDC )
{
    if ( pDC == NULL )
        return;

    Draw( pDC );
}
