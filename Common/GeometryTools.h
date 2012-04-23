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
    //求两点间的距离
    static double GetTwoPointDistance( const CMapPoint* pPt1, const CMapPoint* pPt2 );

    //求两点间的距离
    static double GetTwoPointDistance( const CMapPoint& pt1, const CMapPoint& pt2 );

    //用海伦公式求点到线段间的距离
    static double GetDistancePointToLine( const CMapPoint& pt, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, double dMinDistance = MIN_DISTANCE );

    //求点到折线的距离
    static double GetDistancePointToPLine( const CMapPoint& pt, const CMapPolyline* pPolyline );

    //求以点ptOrigin为原点的水平向左的射线与线段的交点
    //返回值标识交点的状况
    // -1: 无交点 0: 交点在线段间  1: 交点为线段起点  2: 交点为线段终点
    static int GetInterSectionOfHLToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, CMapPoint& ptInterSection );

    //求以点ptOrigin为原点的水平向右的射线与线段的交点
    //返回值标识交点的状况
    // -1: 无交点 0: 交点在线段间  1: 交点为线段起点  2: 交点为线段终点
    static int GetInterSectionOfHRToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, CMapPoint& ptInterSection );

    //求以点ptOrigin为原点的垂直向上的射线与线段的交点
    //返回值标识交点的状况
    // -1: 无交点 0: 交点在线段间  1: 交点为线段起点  2: 交点为线段终点
    static int GetInterSectionOfVTToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart,
        const CMapPoint& ptEnd, CMapPoint& ptInterSection );

    //求以点ptOrigin为原点的垂直向下的射线与线段的交点
    //返回值标识交点的状况
    // -1: 无交点 0: 交点在线段间  1: 交点为线段起点  2: 交点为线段终点
    static int GetInterSectionOfVBToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, CMapPoint& ptInterSection );

    //求点与多边形的位置关系,pBound是给定的多边形的外接矩形,如果不给定该外接矩形,
    //则在函数内部求得该外接矩形.
    // -2: 非法的多边形 
    // -1: 点在多边形外部   0: 点在多边形顶点上 
    //  1: 点在多边形边上   2: 点在多边形内部
    static int GetPosOfPolygon( const CMapPoint& pt, const CMapPolygon* pPolygon, 
        const CMapRect* pRect = NULL );

    //0: 点在直线上
    //1: 点在直线延长线上
    //2: 点在直线外
    //-1: 错误值
    static int RelationOfPointAndLine( const CMapPoint& ptPoint, const CMapPoint& ptStart, 
        const CMapPoint& ptEnd, double dMinDistance = MIN_DISTANCE );

    //以(dx1,dy1)为原点,求正东(水平向右)方向到射线(dx1,dy1)->(dx0,dy0)的逆时针夹角
    static float GetLineAngle( const CMapPoint& ptStart, const CMapPoint& ptEnd );

    //求折线的长度
    static double GetLineLength( const CMapPolyline* pPolyline );

    //以(dx0,dy0)为原点,求射线(dx0,dy0)->(dx1,dy1)到正北方向(垂直向上)的逆时针夹角
    static float CalcAngleToNorth( double dx0, double dy0, double dx1, double dy1 );

    //dX为点ptStart,ptEnd的连接线段与直线y=dYLine的交点的横坐标
    static bool LineCrossYLine( const CMapPoint& ptStart, const CMapPoint& ptEnd, 
        double dYLine, double& dX );

    //dY为点ptStart,ptEnd的连接线段与直线x=dXLine的交点的纵坐标
    static bool LineCrossXLine( const CMapPoint& ptStart, const CMapPoint& ptEnd, 
        double dXLine, double& dY );
};
#endif  //_GEOMETRYTOOLS_H_