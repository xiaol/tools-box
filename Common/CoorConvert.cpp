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
#include <math.h>
#include "CoorConvert.h"

CCoorConvert g_Convert;

#define DEVICE_PIXEL_SIZE   ( 2.54 / 100 / 96 )         //设备象素大小,单位:m
#define DPIXELS_IN_METER    ( 1.0 / DEVICE_PIXEL_SIZE ) //1米宽度的设备象素点数量
#define LOGIC_PIXEL_SIZE    ( 3600 * 24.0 / COOR_RATE ) //逻辑象素大小,单位:m, 1/24秒大约相对于1米
#define LPIXELS_IN_METER    ( 1.0 / LOGIC_PIXEL_SIZE )  //1米宽度的逻辑象素点数量
#define DEFAULT_SCALEVALUE  20000.0                       //默认比例尺系数( 1:200米)
////////////////////////////////////////////////////////////////////
CCoorConvert::CCoorConvert()
{
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	m_ViewRect.bottom = 0;
	m_ViewRect.left = 0;
	m_ViewRect.right = 0;
	m_ViewRect.top = 0;

    m_nScaleValue = LONG( DEFAULT_SCALEVALUE );
    m_dScale = DPIXELS_IN_METER / ( LPIXELS_IN_METER * m_nScaleValue );
}

void CCoorConvert::LPToDP( LONG nLPX, LONG nLPY, LONG& nDPX, LONG& nDPY )
{
    nDPX = FLOAT_TO_LONG( ( nLPX - m_MapRect.minx ) * m_dScale ) + m_nOffsetX;
    nDPY = FLOAT_TO_LONG( ( nLPY - m_MapRect.miny ) * m_dScale ) - m_nOffsetY;
    nDPY = m_ViewRect.bottom - nDPY;
}

void CCoorConvert::DPToLP( LONG nDPX, LONG nDPY, LONG& nLPX, LONG& nLPY )
{
    nLPX = FLOAT_TO_LONG( m_MapRect.minx + ( nDPX - m_nOffsetX ) / m_dScale );
    nLPY = FLOAT_TO_LONG( m_MapRect.miny + ( m_ViewRect.bottom - nDPY + m_nOffsetY ) / m_dScale );
}

void CCoorConvert::DPToLP(const RECT& rcDP, CMapRect& rcLP )
{
	rcLP.minx = m_MapRect.minx; 
	rcLP.minx += FLOAT_TO_LONG( ( rcDP.left - m_nOffsetX ) / m_dScale );
	rcLP.maxx = m_MapRect.minx; 
	rcLP.maxx += FLOAT_TO_LONG( ( rcDP.right - m_nOffsetX ) / m_dScale );
	rcLP.miny = m_MapRect.miny;
	rcLP.miny += FLOAT_TO_LONG( ( m_ViewRect.bottom - rcDP.bottom + m_nOffsetY ) / m_dScale );
	rcLP.maxy = m_MapRect.miny; 
	rcLP.maxy += FLOAT_TO_LONG( ( m_ViewRect.bottom - rcDP.top + m_nOffsetY ) / m_dScale );
}

void CCoorConvert::LPToDP( const CMapRect& rcLP, RECT& rcDP )
{
	rcDP.left = FLOAT_TO_LONG( ( rcLP.minx - m_MapRect.minx ) * m_dScale ) + m_nOffsetX;
	rcDP.right = FLOAT_TO_LONG( ( rcLP.maxx - m_MapRect.minx ) * m_dScale ) + m_nOffsetX;
	rcDP.top = FLOAT_TO_LONG( ( rcLP.maxy - m_MapRect.miny ) * m_dScale );
	rcDP.top = m_ViewRect.bottom - rcDP.top	+ m_nOffsetY;
	rcDP.bottom = FLOAT_TO_LONG( ( rcLP.miny - m_MapRect.miny ) * m_dScale );
	rcDP.bottom = m_ViewRect.bottom - rcDP.bottom + m_nOffsetY;
}

void CCoorConvert::UpdateClipRect()
{
	DPToLP( m_ViewRect, m_ClipRect );
}

void CCoorConvert::UpdateWindow()
{
    POINT ptDP1 = { m_ViewRect.right / 2, m_ViewRect.bottom / 2 };
    
    POINT ptDP2;
    LPToDP( m_ptMapCenter, ptDP2 );
    
    m_nOffsetX += ptDP1.x - ptDP2.x;
    m_nOffsetY += ptDP1.y - ptDP2.y;
    
    UpdateClipRect();
    m_nScaleValue = FLOAT_TO_LONG( DPIXELS_IN_METER / ( m_dScale * LPIXELS_IN_METER ) );
}

void CCoorConvert::ZoomIn( const POINT& point )
{
    DPToLP( point, m_ptMapCenter );
    if( m_dScale > 1 )
    {
        m_dScale *= 1.5;
    }
    else
    {
        DOUBLE dScale = m_dScale;
        double dStep = 0;
        for( int i = 0; i < 16; i++ )
        {
            dScale *= 10;
            if( dScale > 1 )
            {
                dStep = LONG( dScale ) / pow( 10.0, i + 1 ) / 4;
                break;
            }
        }
        m_dScale += dStep;
    }
    
    UpdateWindow();
}

void CCoorConvert::ZoomOut( const POINT& point )
{
    DPToLP( point, m_ptMapCenter );
    if( m_dScale >= 1 )
    {
        m_dScale /= 1.5;
    }
    else
    {
        DOUBLE dScale = m_dScale;
        DOUBLE dStep = 0;
        for( int i = 0; i < 16; i++ )
        {
            dScale *= 10;
            if( dScale > 1 )
            {
                dStep = LONG( dScale ) / pow( 10.0, i + 1 ) / 4;
                break;
            }
        }
        m_dScale -= dStep;
    }

    UpdateWindow();
}

void CCoorConvert::ZoomByRect( const RECT& rc )
{
    MAPRECT ClipRect;
    DPToLP( rc, ClipRect );
    m_ptMapCenter.x = ULONG( ClipRect.maxx + ClipRect.minx ) / 2; 
    m_ptMapCenter.y = ULONG( ClipRect.maxy + ClipRect.miny ) / 2;
    
    LONG nLPWidth = FLOAT_TO_LONG( ClipRect.GetWidth() );
    LONG nDPWidth = m_ViewRect.right - m_ViewRect.left;
    POINT ptCenter = { ( rc.right + rc.left ) / 2, ( rc.top + rc.bottom ) / 2 };
    
    DOUBLE dLPWidth = nLPWidth * LOGIC_PIXEL_SIZE;
    DOUBLE dDPWidth = nDPWidth * DEVICE_PIXEL_SIZE;

    DOUBLE dOldScale = m_dScale;
    LONG nScaleValue = LONG( dLPWidth / dDPWidth );
    if( nScaleValue < 1 )
    {
        m_dScale = DPIXELS_IN_METER / LPIXELS_IN_METER;
    }
    else
    {
        m_dScale = DPIXELS_IN_METER / ( LPIXELS_IN_METER * nScaleValue );
    }
    
    if( m_dScale / dOldScale > 32 )
        m_dScale /= 4;
    UpdateWindow();
}

void CCoorConvert::Reset()
{
    m_dScale = DPIXELS_IN_METER / ( LPIXELS_IN_METER * DEFAULT_SCALEVALUE );
    m_nOffsetX = 0;
    m_nOffsetY = 0;
    UpdateWindow();
}
