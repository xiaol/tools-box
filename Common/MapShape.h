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

    enum PtToRectFlag//����RECT��λ�ù�ϵ���
    {
        PRF_OUTRECT = -1,	//��Ŀ��RECT��
        PRF_INRECT = 0,		//��Ŀ��RECT��
        PRF_ONBORDER = 1	//��Ŀ��RECT�߽�
    };

    enum RectToRectFlag//RECT��RECT��λ�ù�ϵ���
    {
        RRF_INSRC = 0,	    //Ŀ��RECT�ڵ�ǰRECT�ڲ�
        RRF_INDEST = 1,		//��ǰRECT��Ŀ��RECT�ڲ�
        RRF_INTERSECT = 2,	//�ཻ
        RRF_EQUAL = 3,      //���
        RRF_DISCRETE = 4	//����	
    };

    CMapRect(void);
    CMapRect( long maxx, long minx, long maxy, long miny );
    CMapRect( const CMapRect& rc );
    bool operator==( const CMapRect& rc ) const;
    bool operator!=( const CMapRect& rc ) const;

    //�õ��ʼ��Rect
    const CMapRect& operator=( const MAPPOINT& pt );

    //������Rect�Ĳ���        
    const CMapRect& operator+=( const CMapRect& rc );

    //�õ�����Rect
    const CMapRect& operator+=( const MAPPOINT& pt );

    //������Rect�Ĳ���
    CMapRect operator+( const CMapRect& rc1 );

    //��Rect�Ŀ��
    const long GetWidth(void) const;

    //��Rect�ĸ߶�
    const long GetHeight(void) const;

    //��this��rc�Ĳ�
    void OR( const CMapRect& rc );

    //��this��rc�Ľ�
    void AND( const CMapRect& rc );

    //�ж�Rect�ĺϷ���.
    bool checkRectValid();

    //�жϵ㵱ǰ���������λ�ù�ϵ.
    const PtToRectFlag IsPointInRect( const CMapPoint& pt ) const;

    //�ж����������Ƿ�����
    bool IsRectJoined( const CMapRect& rc ) const;

    //�ж�ָ�����������Ƿ��ڵ�ǰ������
    bool RectInRect( const CMapRect& rc ) const;

    //�ж����������λ�ù�ϵ
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

    //��������
    const long AddPoint( const MAPPOINT& pPoint );

    //���������
    void InsertPoint( long nIndex, const MAPPOINT& pPoint );

    //�����״����
    void Release(void);

    //���ƶ���
    CPoly* Clone(void);
};

typedef CPoly CPolygon, CPolyline;
typedef vector< CPoly* > CPolyVector, CPolygonVector, CPolylineVector;   
#endif