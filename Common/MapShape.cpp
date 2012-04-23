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
#include "MapShape.h"
#include "CommonAPI.h"
#include "CommonDef.h"


CMapPoint::CMapPoint(void)
{
    x = 0;
    y = 0;
}

CMapPoint::CMapPoint( long x, long y )
{
    this->x = x;
    this->y = y;
}

CMapPoint::CMapPoint( const CMapPoint& pt )
{
    x = pt.x;
    y = pt.y;
}

bool CMapPoint::operator==( const CMapPoint& pt ) const
{
    if( VALUE_NOTEQUAL( x, pt.x ) || VALUE_NOTEQUAL( y, pt.y ) )
    {
        return false;
    }
    return true;
}

bool CMapPoint::operator!=( const CMapPoint& pt ) const
{
    if( VALUE_NOTEQUAL( x, pt.x ) || VALUE_NOTEQUAL( y, pt.y ) )
    {
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
CMapRect::CMapRect(void)
{
    maxx = 0;
    maxy = 0;
    minx = 0;
    miny = 0;
}

CMapRect::CMapRect( long maxx, long minx, long maxy, long miny )
{
    this->maxx = maxx;
    this->maxy = maxy;
    this->minx = minx;
    this->miny = miny;
}

CMapRect::CMapRect( const CMapRect& rc )
{
    maxx = rc.maxx;
    maxy = rc.maxy;
    minx = rc.minx;
    miny = rc.miny;
}

bool CMapRect::operator==( const CMapRect& rc ) const
{
    if( VALUE_NOTEQUAL( maxx, rc.maxx ) || VALUE_NOTEQUAL( minx, rc.minx )
        || VALUE_NOTEQUAL( maxy, rc.maxy ) || VALUE_NOTEQUAL( miny, rc.miny ) )
    {
        return false;
    }
    return true;
}

bool CMapRect::operator!=( const CMapRect& rc ) const
{
    if( VALUE_NOTEQUAL( maxx, rc.maxx ) || VALUE_NOTEQUAL( minx, rc.minx )
        || VALUE_NOTEQUAL( maxy, rc.maxy ) || VALUE_NOTEQUAL( miny, rc.miny ) )
    {
        return true;
    }
    return false;
}

const CMapRect& CMapRect::operator=( const CMapPoint& pt )
{
    minx = pt.x;
    maxx = pt.x;
    miny = pt.y;
    maxy = pt.y;
    return *this; 
}

const CMapRect& CMapRect::operator+=( const CMapRect& rc )
{
    minx = min( minx, rc.minx );
    maxx = max( maxx, rc.maxx );
    miny = min( miny, rc.miny );
    maxy = max( maxy, rc.maxy );
    return *this; 
}

const CMapRect& CMapRect::operator+=( const CMapPoint& pt )
{
    minx = min( minx, pt.x );
    maxx = max( maxx, pt.x );
    miny = min( miny, pt.y );
    maxy = max( maxy, pt.y );
    return *this; 
}

CMapRect CMapRect::operator+( const CMapRect& rc )
{
    CMapRect rect = *this;
    rect += rc;
    return rect;
}

const long CMapRect::GetWidth(void) const 
{
    return maxx - minx;
}

const long CMapRect::GetHeight(void) const
{
    return maxy - miny;
}

void CMapRect::OR( const CMapRect& rc )
{
    *this += rc;
}

void CMapRect::AND( const CMapRect& rc )
{
    minx = max( minx, rc.minx );
    miny = max( miny, rc.miny );
    maxx = min( maxx, rc.maxx );
    maxy = min( maxy, rc.maxy );

    if( !checkRectValid() )
    {
        maxx = 0;
        maxy = 0;
        minx = 0;
        miny = 0;
    }
}

//判断Rect的合法性。
bool CMapRect::checkRectValid(void)
{
    if( maxx <= minx || maxy <= miny )
    {
        return false;
    }
    return true;
}

//判断点给定矩形区域的位置关系。
const CMapRect::PtToRectFlag CMapRect::IsPointInRect( const CMapPoint& pt ) const
{
    CMapRect::PtToRectFlag flag = PRF_OUTRECT;
    if( pt.x >= minx && pt.x <= maxx 
        && pt.y >= miny && pt.y <= maxy )
    {
        if( VALUE_EQUAL( pt.x, minx ) || VALUE_EQUAL( pt.x, maxx ) 
            ||	pt.y >= miny || pt.y <= maxy )
        {
            flag = PRF_ONBORDER;//点在矩形区域边界。
        }
        else
        {
            flag = PRF_INRECT;//点在矩形区域内。
        }
    }
    return flag;//点在矩形区域外部。
}

//判断两个区域是否相连
bool CMapRect::IsRectJoined( const CMapRect& rc ) const
{
    if( maxx <= rc.minx || minx >= rc.maxx )
    {
        return false;
    }
    else
    {
        if( maxy <= rc.miny || miny >= rc.maxy )
        {
            return false;
        }
    }
    return true;
}

//判断两个区域的位置关系
const CMapRect::RectToRectFlag CMapRect::RelationOfTwoRect( const CMapRect& rc ) const
{
    CMapRect::RectToRectFlag flag = RRF_EQUAL;
    if( VALUE_EQUAL( maxx, rc.maxx ) && VALUE_EQUAL( minx, rc.minx ) 
        && VALUE_EQUAL( maxy, rc.maxy ) && VALUE_EQUAL( miny, rc.miny ) )
    {
        flag = RRF_EQUAL;
    }
    else if( IsRectJoined( rc ) )
    {
        if( maxx <= rc.maxx && minx >= rc.minx 
            && maxy <= rc.maxy && miny >= rc.miny ) 
        {
            flag = RRF_INDEST;//this包含于Rect
        }
        else if( rc.maxx <= maxx && rc.minx >= minx 
            && rc.maxy <= maxy && rc.miny >= miny )
        {
            flag = RRF_INSRC;//Rect包含于this
        }
        else
        {
            flag = RRF_INTERSECT;//this与Rect相交
        }
    }
    else
    {
        flag = RRF_DISCRETE;//this与Rect相离
    }
    return flag;
}

bool CMapRect::RectInRect( const CMapRect& rc ) const
{
    if( rc.maxx <= maxx && rc.minx >= minx 
        && rc.maxy <= maxy && rc.miny >= miny )
    {
        return true;
    }
    return false;
}

CPoly::CPoly()
{
}

CPoly::CPoly( const CPoly* pPoly )
{
    if( pPoly != NULL )
    {
        shape.assign( pPoly->shape.begin(), pPoly->shape.end() );
        rect = pPoly->rect;
    }
}

CPoly::CPoly( const CPointVector* pPointVec )
{
    if( pPointVec != NULL )
    {
        CPointVector::const_iterator itr;
        for( itr = pPointVec->begin(); itr != pPointVec->end(); itr++ )
        {
            AddPoint( *itr );
        }
    }
}

CPoly::CPoly( const CPointVector* pPointVec, const CMapRect& rc )
{
    if( pPointVec != NULL )
    {
        shape.assign( pPointVec->begin(), pPointVec->end() );
    }
    rect = rc;
}

CPoly::~CPoly()
{
}

const long CPoly::AddPoint( const MAPPOINT& point )
{
    if( shape.size() == 0 )
    {
        rect = point;
    }
    else
    {
        rect += point;
    }
    shape.push_back( point );
    return (long)shape.size();
}

void CPoly::InsertPoint( long lIndex, const MAPPOINT& point )
{
    size_t size = shape.size();
    if( size == 0 )
    {
        rect = point;
    }
    else
    {
        rect += point;
    }

    if( lIndex < 0 || lIndex >= (long)size )
    {
        shape.push_back( point );
    }
    else
    {
        CPointVector::iterator itr = shape.begin();
        for( long i = 0; i < lIndex; i++ )
        {
            itr++;
        }
        shape.insert( itr, point );
    }
}

CPoly* CPoly::Clone()
{
    CPoly* pNew = new CPoly( this );
    return pNew;
}

void CPoly::Release()
{
    rect = CMapRect();
    shape.clear();
}

