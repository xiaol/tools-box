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

#ifndef _GEOMETRYTOOLS_H_INCLUDED_
#define _GEOMETRYTOOLS_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MapShape.h"

#define MIN_DISTANCE 0.000001
#define PI 3.14159265

class CGeometryTools
{
public:

    typedef CPointVector            CMapPolyline, CMapPolygon;
    typedef vector<CMapPolyline*>   CMapPolylineVec, CMapPolygonVec;

    //////////////////////////////////////////////////////////////////////////////////
    //�������ľ���
    static double GetTwoPointDistance( const CMapPoint* pPt1, const CMapPoint* pPt2 );

    //�������ľ���
    static double GetTwoPointDistance( const CMapPoint& pt1, const CMapPoint& pt2 );

    //�ú��׹�ʽ��㵽�߶μ�ľ���
    static double GetDistancePointToLine( const CMapPoint& pt, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, double dMinDistance = MIN_DISTANCE );

    //��㵽���ߵľ���
    static double GetDistancePointToPLine( const CMapPoint& pt, const CMapPolyline* pPolyline );

    //���Ե�ptOriginΪԭ���ˮƽ������������߶εĽ���
    //����ֵ��ʶ�����״��
    // -1: �޽��� 0: �������߶μ�  1: ����Ϊ�߶����  2: ����Ϊ�߶��յ�
    static int GetInterSectionOfHLToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, CMapPoint& ptInterSection );

    //���Ե�ptOriginΪԭ���ˮƽ���ҵ��������߶εĽ���
    //����ֵ��ʶ�����״��
    // -1: �޽��� 0: �������߶μ�  1: ����Ϊ�߶����  2: ����Ϊ�߶��յ�
    static int GetInterSectionOfHRToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, CMapPoint& ptInterSection );

    //���Ե�ptOriginΪԭ��Ĵ�ֱ���ϵ��������߶εĽ���
    //����ֵ��ʶ�����״��
    // -1: �޽��� 0: �������߶μ�  1: ����Ϊ�߶����  2: ����Ϊ�߶��յ�
    static int GetInterSectionOfVTToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart,
        const CMapPoint& ptEnd, CMapPoint& ptInterSection );

    //���Ե�ptOriginΪԭ��Ĵ�ֱ���µ��������߶εĽ���
    //����ֵ��ʶ�����״��
    // -1: �޽��� 0: �������߶μ�  1: ����Ϊ�߶����  2: ����Ϊ�߶��յ�
    static int GetInterSectionOfVBToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, CMapPoint& ptInterSection );

    //��������ε�λ�ù�ϵ,pBound�Ǹ����Ķ���ε���Ӿ���,�������������Ӿ���,
    //���ں����ڲ���ø���Ӿ���.
    // -2: �Ƿ��Ķ���� 
    // -1: ���ڶ�����ⲿ   0: ���ڶ���ζ����� 
    //  1: ���ڶ���α���   2: ���ڶ�����ڲ�
    static int GetPosOfPolygon( const CMapPoint& pt, const CMapPolygon* pPolygon, 
        const CMapRect* pRect = NULL );

    //0: ����ֱ����
    //1: ����ֱ���ӳ�����
    //2: ����ֱ����
    //-1: ����ֵ
    static int RelationOfPointAndLine( const CMapPoint& ptPoint, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, double dMinDistance = MIN_DISTANCE );

    //��(dx1,dy1)Ϊԭ��,������(ˮƽ����)��������(dx1,dy1)->(dx0,dy0)����ʱ��н�
    static float GetLineAngle( const CMapPoint& ptStart, const CMapPoint& ptEnd );

    //�����ߵĳ���
    static double GetLineLength( const CMapPolyline* pPolyline );

    //��(dx0,dy0)Ϊԭ��,������(dx0,dy0)->(dx1,dy1)����������(��ֱ����)����ʱ��н�
    static float CalcAngleToNorth( double dx0, double dy0, double dx1, double dy1 );

    //dXΪ��ptStart,ptEnd�������߶���ֱ��y=dYLine�Ľ���ĺ�����
    static bool LineCrossYLine( const CMapPoint& ptStart, const CMapPoint& ptEnd, 
        double dYLine, double& dX );

    //dYΪ��ptStart,ptEnd�������߶���ֱ��x=dXLine�Ľ����������
    static bool LineCrossXLine( const CMapPoint& ptStart, const CMapPoint& ptEnd, 
        double dXLine, double& dY );
};
#endif  //_GEOMETRYTOOLS_H_