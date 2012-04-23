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

#ifndef _MAPSHAPE_H_INCLUDED_
#define _MAPSHAPE_H_INCLUDED_

#pragma once

#include <vector>
using namespace std;


struct CMapPoint 
{
    long x;
    long y;

    CMapPoint(void);
    CMapPoint( long x, long y );
    CMapPoint( const CMapPoint& pt );
    bool operator==( const CMapPoint& pt ) const;
    bool operator!=( const CMapPoint& pt ) const;
};

typedef CMapPoint MAPPOINT, *LPMAPPOINT;
typedef vector<CMapPoint> CPointVector;

///////////////////////////////////////////////////////////////////////////////////////////
struct CMapRect 
{
    long minx;
    long miny;
    long maxx;
    long maxy;

    enum PtToRectFlag//点与RECT的位置关系标记
    {
        PRF_OUTRECT = -1,	//在目标RECT外
        PRF_INRECT = 0,		//在目标RECT内
        PRF_ONBORDER = 1	//在目标RECT边界
    };

    enum RectToRectFlag//RECT与RECT的位置关系标记
    {
        RRF_INSRC = 0,	    //目标RECT在当前RECT内部
        RRF_INDEST = 1,		//当前RECT在目标RECT内部
        RRF_INTERSECT = 2,	//相交
        RRF_EQUAL = 3,      //相等
        RRF_DISCRETE = 4	//相离	
    };

    CMapRect(void);
    CMapRect( long maxx, long minx, long maxy, long miny );
    CMapRect( const CMapRect& rc );
    bool operator==( const CMapRect& rc ) const;
    bool operator!=( const CMapRect& rc ) const;

    //用点初始化Rect
    const CMapRect& operator=( const MAPPOINT& pt );

    //求两个Rect的并集        
    const CMapRect& operator+=( const CMapRect& rc );

    //用点扩充Rect
    const CMapRect& operator+=( const MAPPOINT& pt );

    //求两个Rect的并集
    CMapRect operator+( const CMapRect& rc1 );

    //求Rect的宽度
    const long GetWidth(void) const;

    //求Rect的高度
    const long GetHeight(void) const;

    //求this与rc的并
    void OR( const CMapRect& rc );

    //求this与rc的交
    void AND( const CMapRect& rc );

    //判断Rect的合法性.
    bool checkRectValid();

    //判断点当前矩形区域的位置关系.
    const PtToRectFlag IsPointInRect( const CMapPoint& pt ) const;

    //判断两个区域是否相连
    bool IsRectJoined( const CMapRect& rc ) const;

    //判断指定矩形区域是否在当前区域内
    bool RectInRect( const CMapRect& rc ) const;

    //判断两个区域的位置关系
    const RectToRectFlag RelationOfTwoRect( const CMapRect& rc ) const;
};

typedef CMapRect MAPRECT, *LPMAPRECT;
///////////////////////////////////////////////////////////////////////////////////////////
struct CPoly
{
    CMapRect rect;
    CPointVector shape;

    CPoly();
    ~CPoly();
    CPoly( const CPoly* CPoly );
    CPoly( const CPointVector* pPointVec );

    CPoly( const CPointVector* pPointVec, const CMapRect& rc );

    //添加坐标点
    const long AddPoint( const MAPPOINT& pPoint );

    //插入坐标点
    void InsertPoint( long nIndex, const MAPPOINT& pPoint );

    //清空形状内容
    void Release(void);

    //复制对象
    CPoly* Clone(void);
};

typedef CPoly CPolygon, CPolyline;
typedef vector< CPoly* > CPolyVector, CPolygonVector, CPolylineVector;   
#endif