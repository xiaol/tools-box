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
#include "GeometryTools.h"
#include <math.h>
#include "CommonDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//求两点间的距离
double CGeometryTools::GetTwoPointDistance( const CMapPoint* pPt1, const CMapPoint* pPt2 )
{
    if( ( pPt1 == NULL ) || ( pPt2 == NULL ) )
    {
        return 0;
    }
    return _hypot( pPt1->x - pPt2->x, pPt1->y - pPt2->y );
}

//求两点间的距离
double CGeometryTools::GetTwoPointDistance( const CMapPoint& pt1, const CMapPoint& pt2 )
{
    return _hypot( pt1.x - pt2.x, pt1.y - pt2.y );
}

//用海伦公式求点到线段间的距离
double CGeometryTools::GetDistancePointToLine( const CMapPoint& pt, const CMapPoint& ptStart,
                                              const CMapPoint& ptEnd, double dMinDistance )
{
    double dAngle1 = GetLineAngle( pt, ptStart );
    double dAngle2 = GetLineAngle( ptEnd, ptStart );
    double dAngle3 = GetLineAngle( pt, ptEnd );
    double dAngle4 = GetLineAngle( ptStart, ptEnd );

    double dA = GetTwoPointDistance( pt, ptStart );
    double dB = GetTwoPointDistance( pt, ptEnd );
    double dC = GetTwoPointDistance( ptStart, ptEnd );

    double dDistance = 0;
    if ( ( fabs( dAngle1 - dAngle2 ) >= 90.0 ) || ( fabs( dAngle3 - dAngle4 ) >= 90.0 ) )
    {
        dDistance = min( dA, dB );
    }
    else if ( VALUE_EQUAL( dC, 0.0 ) )
    {
        ASSERT( VALUE_EQUAL( dA, dB ) );
        dDistance = dA;
    }
    else if ( ( dA <= dMinDistance ) || ( dB <= dMinDistance ) )
    {
        dDistance = dMinDistance;
    }    
    else if ( fabs( max( dA, dB ) - min( dA, dB ) - dC ) <= dMinDistance )
    {
        dDistance = min( dA, dB );
    }
    else
    {
        double dS = ( dA + dB + dC ) / 2;
        double dAreaABC = sqrt( dS * ( dS - dA ) * ( dS - dB ) * ( dS - dC ) );
        dDistance = 2.0 * dAreaABC / dC;
    }

    return( dDistance );
}

double CGeometryTools::GetDistancePointToPLine( const CMapPoint& pt, const CMapPolyline* pPolyline )
{
    size_t size = pPolyline->size();

    double dMinDistance = 0;
    for ( size_t i = 0; i < size - 1; i++ )
    {
        double d = GetDistancePointToLine( pt, (*pPolyline)[i], (*pPolyline)[i+1] );
        if ( i == 0 )
        {
            dMinDistance = d;
        }
        else
        {
            dMinDistance = min( dMinDistance, d );
        }
    }
    return( dMinDistance );
}

//求以点pt为原点的水平向左的射线与线段的交点
//返回值标识交点的状况
// -1: 无交点 0: 交点在线段间  1: 交点为线段起点  2: 交点为线段终点
int CGeometryTools::GetInterSectionOfHLToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
                                              const CMapPoint& ptEnd, CMapPoint& ptInterSection )
{
    //根据线段的最大Y坐标、最小Y坐标和X坐标,预先判断射线与线段有无相交可能性
    if( ( ptOrigin.y > max( ptStart.y, ptEnd.y ) )
        || ( ptOrigin.y < min( ptStart.y, ptEnd.y ) )
        || ( ptOrigin.x < min( ptStart.x, ptEnd.x ) ) )
    {
        return -1;
    }
    
    //如果线段为水平线段,则可认为与水平射线无交点
    if( VALUE_EQUAL( ptEnd.y, ptStart.y ) )
    {
        return -1;
    }
    //如果线段的起点在射线上,则交点为线段起点
    if( VALUE_EQUAL( ptOrigin.y, ptStart.y ) )
    {
        ptInterSection = ptStart;
        return 1;
    }
    
    //如果线段的终点在射线上,则终点为线段起点
    if( VALUE_EQUAL( ptOrigin.y, ptEnd.y ) )
    {
        ptInterSection = ptEnd;
        return 2;
    }
    
    //如果线段为垂直线段
    if( VALUE_EQUAL( ptEnd.x, ptStart.x ) )
    {
        ptInterSection.x = ptStart.x;
        ptInterSection.y = ptOrigin.y;
        return 0;
    }
    
    //线段的斜率
    double dSlope = ( ptEnd.y - ptStart.y ) * 1.0 / ( ptEnd.x - ptStart.x );
    ptInterSection.y = ptOrigin.y;
    ptInterSection.x = (long)( ( ptOrigin.y - ptStart.y ) / dSlope + ptStart.x );
    
    //如果交点不在射线上,则射线与线段无交点
    if( ptInterSection.x > ptOrigin.x )
    {
        ptInterSection.x = 0;
        ptInterSection.y = 0;
        return -1;
    }
    
    return 0;
}

//求以点pt为原点的水平向右的射线与线段的交点
//返回值标识交点的状况
// -1: 无交点 0: 交点在线段间  1: 交点为线段起点  2: 交点为线段终点
int CGeometryTools::GetInterSectionOfHRToLine(  const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
                                              const CMapPoint& ptEnd, CMapPoint& ptInterSection )
{
    //根据线段的最大Y坐标、最小Y坐标和X坐标,预先判断射线与线段有无相交可能性
    if( ( ptOrigin.y > max( ptStart.y, ptEnd.y ) )
        || ( ptOrigin.y < min( ptStart.y, ptEnd.y ) )
        || ( ptOrigin.x > max( ptStart.x, ptEnd.x ) ) )
    {
        return -1;
    }
    
    //如果线段为水平线段,则可认为与水平射线无交点
    if( VALUE_EQUAL( ptEnd.y, ptStart.y ) )
        return -1;
    
    //如果线段的起点在射线上,则交点为线段起点
    if( VALUE_EQUAL( ptOrigin.y, ptStart.y ) )
    {
        ptInterSection = ptStart;
        return 1;
    }
    
    //如果线段的终点在射线上,则终点为线段起点
    if( VALUE_EQUAL( ptOrigin.y, ptEnd.y ) )
    {
        ptInterSection = ptEnd;
        return 2;
    }
    
    //如果线段为垂直线段
    if( VALUE_EQUAL( ptEnd.x, ptStart.x ) )
    {
        ptInterSection.x = ptStart.x;
        ptInterSection.y = ptOrigin.y;
        return 0;
    }
    
    //线段的斜率
    double dSlope = ( ptEnd.y - ptStart.y ) * 1.0 / ( ptEnd.x - ptStart.x );
    ptInterSection.y = ptOrigin.y;
    ptInterSection.x = (long)( ( ptOrigin.y - ptStart.y ) / dSlope + ptStart.x );
    
    //如果交点不在射线上,则射线与线段无交点
    if( ptInterSection.x < ptOrigin.x )
    {
        ptInterSection.x = 0;
        ptInterSection.y = 0;
        return -1;
    }
    return 0;
}

//求以点pt为原点的垂直向上的射线与线段的交点
//返回值标识交点的状况
// -1: 无交点 0: 交点在线段间  1: 交点为线段起点  2: 交点为线段终点
int CGeometryTools::GetInterSectionOfVTToLine(  const CMapPoint& ptOrigin, 
                                              const CMapPoint& ptStart, 
                                              const CMapPoint& ptEnd, 
                                              CMapPoint& ptInterSection )
{
    //根据线段的最大Y坐标、最小Y坐标和X坐标,预先判断射线与线段有无相交可能性
    if( ( ptOrigin.x > max( ptStart.x, ptEnd.x ) )
        || ( ptOrigin.x < min( ptStart.x, ptEnd.x ) )
        || ( ptOrigin.y > max( ptStart.y, ptEnd.y ) ) )
    {
        return -1;
    }
    
    //如果线段为垂直线段,则可认为与水平射线无交点
    if( VALUE_EQUAL( ptEnd.x, ptStart.x ) )
        return -1;
    
    //如果线段的起点在射线上,则交点为线段起点
    if( VALUE_EQUAL( ptOrigin.x, ptStart.x ) )
    {
        ptInterSection = ptStart;
        return 1;
    }
    
    //如果线段的终点在射线上,则终点为线段起点
    if( VALUE_EQUAL( ptOrigin.x, ptEnd.x ) )
    {
        ptInterSection = ptEnd;
        return 2;
    }
    
    //如果线段为水平线段
    if( VALUE_EQUAL( ptEnd.y, ptStart.y ) )
    {
        ptInterSection.x = ptOrigin.x;
        ptInterSection.y = ptStart.y;
        return 0;
    }
    
    //线段的斜率
    double dSlope = ( ptEnd.y - ptStart.y ) * 1.0 / ( ptEnd.x - ptStart.x );
    ptInterSection.x = ptOrigin.x;
    ptInterSection.y = (long)( ( ptOrigin.x - ptStart.x ) * dSlope + ptStart.y );
    
    //如果交点不在射线上,则射线与线段无交点
    if( ptInterSection.y < ptOrigin.y )
    {
        ptInterSection.x = 0;
        ptInterSection.y = 0;
        return -1;
    }
    return 0;
}

//求以点pt为原点的垂直向下的射线与线段的交点
//返回值标识交点的状况
// -1: 无交点 0: 交点在线段间  1: 交点为线段起点  2: 交点为线段终点
int CGeometryTools::GetInterSectionOfVBToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
                                              const CMapPoint& ptEnd, CMapPoint& ptInterSection )
{
    //根据线段的最大Y坐标、最小Y坐标和X坐标,预先判断射线与线段有无相交可能性
    if( ( ptOrigin.x > max( ptStart.x, ptEnd.x ) )
        || ( ptOrigin.x < min( ptStart.x, ptEnd.x ) )
        || ( ptOrigin.y < min( ptStart.y, ptEnd.y ) ) )
    {
        return -1;
    }
    
    //如果线段为垂直线段,则可认为与水平射线无交点
    if( VALUE_EQUAL( ptEnd.x, ptStart.x ) )
        return -1;
    
    //如果线段的起点在射线上,则交点为线段起点
    if( VALUE_EQUAL( ptOrigin.x, ptStart.x ) )
    {
        ptInterSection = ptStart;
        return 1;
    }
    
    //如果线段的终点在射线上,则终点为线段起点
    if( VALUE_EQUAL( ptOrigin.x, ptEnd.x ) )
    {
        ptInterSection = ptEnd;
        return 2;
    }
    
    //如果线段为水平线段
    if( VALUE_EQUAL( ptEnd.y, ptStart.y ) )
    {
        ptInterSection.x = ptOrigin.x;
        ptInterSection.y = ptStart.y;
        return 0;
    }
    
    //线段的斜率
    double dSlope = ( ptEnd.y - ptStart.y ) * 1.0 / ( ptEnd.x - ptStart.x );
    ptInterSection.x = ptOrigin.x;
    ptInterSection.y = (long)(( ptOrigin.x - ptStart.x ) * dSlope 
        + ptStart.y);
    
    //如果交点不在射线上,则射线与线段无交点
    if( ptInterSection.y > ptOrigin.y )
    {
        ptInterSection.x = 0;
        ptInterSection.y = 0;
        return -1;
    }
    return 0;
}

//求点与多边形的位置关系
// -2: 非法的多边形 
// -1: 点在多边形外部   0: 点在多边形顶点上 
//  1: 点在多边形边上   2: 点在多边形内部
int CGeometryTools::GetPosOfPolygon( const CMapPoint& ptOrigin, const CMapPolygon* pPolygon, 
                                    const CMapRect* pRect )
{
    size_t nCount = pPolygon->size();
    if( nCount < 3 )
    {
        return -2;
    }
    CMapRect bound;
    if( pRect != NULL )
    {
        bound = *pRect;
    }
    else
    {
        for( size_t i = 0; i < nCount; i++ )
        {
            const CMapPoint& pt = (*pPolygon)[i];
            if( i == 0 )
            {
                bound = pt;
            }
            else
            {
                bound += pt;
            }
        }
    }
    
    if( bound.IsPointInRect( ptOrigin ) == CMapRect::PRF_OUTRECT )
    {
        return -1;
    }
    
    size_t nStart = 0;
    size_t nEnd = 0;
    
    CMapPoint ptStart, ptEnd;
    CMapPoint pt;
    
    //四个方向的射线与多边形边的交点计数器,
    //如果其中一个计数器为奇数,则点在多边形内部 
    int nCntHL = 0;//水平向左的射线与多边线的边的交点计数器
    int nCntHR = 0;//水平向右的射线与多边线的边的交点计数器
    int nCntVT = 0;//垂直向上的射线与多边线的边的交点计数器
    int nCntVB = 0;//垂直向下的射线与多边线的边的交点计数器
    
    int nReturn = 0;//射线与多边形的某一条边的交点情况返回值
    
    for( size_t i = 0; i < nCount; i++ )
    {
        nStart = i;
        nEnd = i + 1;
        if( nEnd == nCount )
        {
            nEnd = 0;
        }
        ptStart = (*pPolygon)[nStart];
        ptEnd = (*pPolygon)[nEnd];
        
        double dDistance = GetDistancePointToLine( ptOrigin, ptStart, ptEnd );
        if( ( dDistance <= MIN_DISTANCE ) && ( dDistance >= 0 ) )
        {
            dDistance = min( GetTwoPointDistance( ptStart, ptOrigin ),
                GetTwoPointDistance( ptEnd, ptOrigin ) );
            if( dDistance <= MIN_DISTANCE )
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        
        //求以mappt为原点,水平向左的射线与当前边的交点情况
        nReturn = GetInterSectionOfHLToLine( ptOrigin, ptStart, ptEnd, pt );
        switch( nReturn )
        {
        case 0:
            {
                nCntHL++;
            }
            break;
        case 2://如果交点为当前线段的终点,则判断多边形的下一个顶点与射线的位置情况
            //如果线段的起点和下一个顶点分布在射线的两侧,则交点计数器加1
            {
                size_t nNext = nEnd + 1;
                if( nNext == nCount )
                {
                    nNext = 0;
                }
                pt = (*pPolygon)[nNext];
                if( ( ptOrigin.y > min( pt.y, ptStart.y ) )
                    && ( ptOrigin.y <  max( pt.y, ptStart.y ) ) )
                {
                    nCntHL++;
                }
            }
            break;
        }
        
        //求以mappt为原点,水平向右的射线与当前边的交点情况
        nReturn = GetInterSectionOfHRToLine( ptOrigin, ptStart, ptEnd, pt );
        switch( nReturn )
        {
        case 0:
            {
                nCntHR++;
            }
            break;
        case 2://如果交点为当前线段的终点,则判断多边形的下一个顶点与射线的位置情况
            //如果线段的起点和下一个顶点分布在射线的两侧,则交点计数器加1
            {
                size_t nNext = nEnd + 1;
                if( nNext == nCount )
                {
                    nNext = 0;
                }
                pt = (*pPolygon)[nNext];
                if( ( ptOrigin.y > min( pt.y, ptStart.y ) )
                    && ( ptOrigin.y < max( pt.y, ptStart.y ) ) )
                {
                    nCntHR++;
                }
            }
            break;
        }
        
        //求以mappt为原点,垂直向上的射线与当前边的交点情况
        nReturn = GetInterSectionOfVTToLine( ptOrigin, ptStart, ptEnd, pt );
        switch( nReturn )
        {
        case 0:
            {
                nCntVT++;
            }
            break;
        case 2://如果交点为当前线段的终点,则判断多边形的下一个顶点与射线的位置情况
            //如果线段的起点和下一个顶点分布在射线的两侧,则交点计数器加1
            {
                size_t nNext = nEnd + 1;
                if( nNext == nCount )
                {
                    nNext = 0;
                }
                pt = (*pPolygon)[nNext];
                if( ( ptOrigin.x > min( pt.x, ptStart.x ) )
                    && ( ptOrigin.x < max( pt.x, ptStart.x ) ) )
                {
                    nCntVT++;
                }
            }
            break;
        }
        
        //求以mappt为原点,垂直向下的射线与当前边的交点情况
        nReturn = GetInterSectionOfVBToLine( ptOrigin, ptStart, ptEnd, pt );
        switch( nReturn )
        {
        case 0:
            {
                nCntVB++;
            }
            break;
        case 2://如果交点为当前线段的终点,则判断多边形的下一个顶点与射线的位置情况
            //如果线段的起点和下一个顶点分布在射线的两侧,则交点计数器加1
            {
                size_t nNext = nEnd + 1;
                if( nNext == nCount )
                {
                    nNext = 0;
                }
                pt = (*pPolygon)[nNext];
                if( ( ptOrigin.x > min( pt.x, ptStart.x ) )
                    && ( ptOrigin.x < max( pt.x, ptStart.x ) ) )
                {
                    nCntVB++;
                }
            }
            break;
        }
    }
    
    //判断交点计数器的奇偶性
    if( ( ( nCntHL % 2 ) == 1 ) || ( ( nCntHR % 2 ) == 1 )  
        || ( ( nCntVT % 2 ) == 1 ) || ( ( nCntVB % 2 ) == 1 ) )
    {
        return 2;
    }
    return -1;
}

//0: 点在直线上
//1: 点在直线延长线上
//2: 点在直线外
//-1: 错误值
int CGeometryTools::RelationOfPointAndLine( const CMapPoint& ptPoint, const CMapPoint& ptStart, 
                                           const CMapPoint& ptEnd, double dMinDistance )
{
    double dDistance = GetDistancePointToLine( ptPoint, ptStart, ptEnd );
    if( VALUE_EQUAL( dDistance, - 1.0 ) )
    {
        return -1;
    }
    else if( VALUE_EQUAL( dDistance, -2.0 ) )
    {
        double dA = GetTwoPointDistance( ptPoint, ptStart );
        double dB = GetTwoPointDistance( ptPoint, ptEnd );
        if( max( dA, dB ) <= dMinDistance )
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else if( dDistance <= dMinDistance )
    {
        return 0;
    }
    else
    {
        return 2;
    }
    return -1;
}

//以(dx1,dy1)为原点,求正东(水平向右)方向到射线(dx1,dy1)->(dx0,dy0)的逆时针夹角
float CGeometryTools::GetLineAngle( const CMapPoint& ptStart, const CMapPoint& ptEnd )
{
    float fAngle = 0;
    if( VALUE_EQUAL( ptStart.x, ptEnd.x ) )  //垂直射线
    {
        if( ptStart.y > ptEnd.y )//向上
        {
            return 90;
        }
        else//向下
        {
            return 270;
        }
    }
    else if( VALUE_EQUAL( ptStart.y, ptEnd.y ) )//水平射线
    {
        if( ptEnd.x > ptStart.x )//向右
        {
            return 0;
        }
        else//向左
        {
            return 180;
        }
    }
    
    fAngle = (float)( ( atan2( ( ptStart.y - ptEnd.y ) * 1.0, ptStart.x - ptEnd.x ) * 180 ) / PI );
    
    if( ptStart.y < ptEnd.y )//第三、四区间射线
    {
        fAngle += 360;
    }
    
    return fAngle;
}

double CGeometryTools::GetLineLength( const CMapPolyline* pPolyline )
{
    if( pPolyline == NULL )
    {
        return 0.0;
    }
    size_t size = pPolyline->size();
    if( size < 2 )
        return 0.0;
    
    double dLen = 0;
    for( size_t i = 0; i < size - 1; i++ )
    {
        dLen += GetTwoPointDistance( (*pPolyline)[i], (*pPolyline)[i+1] );
    }
    
    return dLen;
}

//以(dx0,dy0)为原点,求射线(dx0,dy0)->(dx1,dy1)到正北方向(垂直向上)的逆时针夹角
float CGeometryTools::CalcAngleToNorth( double dx0, double dy0, double dx1, double dy1 )
{
    float fAngle = 0;
    if( dx0 == dx1 )  //垂直射线
    {
        if( dy1 > dy0 )//向上
        {
            return 0;
        }
        else//向下
        {
            return 180;
        }
    }
    else if( dy0 == dy1 )//水平射线
    {
        if( dx0 < dx1 )//向右
        {
            return 270;
        }
        else//向左
        {
            return 90;
        }
    }
    
    fAngle = (float)( ( atan2( dy1 - dy0, dx1 - dx0 ) * 180.0 ) / PI );
    fAngle -= 90;
    if( fAngle < 0 )//第三、四区间射线
    {
        fAngle += 360;
    }
    
    return fAngle;
}

//dX为点ptStart,ptEnd的连接线段与直线y=dYLine的交点的横坐标
bool CGeometryTools::LineCrossYLine( const CMapPoint& ptStart, const CMapPoint& ptEnd, 
                                    double dYLine, double& dX )
{
    if( VALUE_EQUAL( dYLine, ptStart.y ) )
    {
        return false;
    }
    if( VALUE_EQUAL( ptStart.y, ptEnd.y ) && VALUE_NOTEQUAL( ptStart.y, dYLine ) )// 平行
    {
        return false;			
    }    
    if( ( ptStart.y < dYLine ) && ( ptEnd.y < dYLine ) )// 位于直线下侧
    {
        return false;	
    }    
    if( ( ptStart.y > dYLine ) && ( ptEnd.y > dYLine ) )// 位于直线上侧
    {
        return false;	
    }
    if( VALUE_EQUAL( ptEnd.y, dYLine ) )
    {
        dX = ptEnd.x;
        return true;
    }

    dX = ( ptEnd.x - ptStart.x ) * ( dYLine - ptStart.y ) * 1.0
        / ( ptEnd.y - ptStart.y ) + ptStart.x;
    return true;
}

//dY为点ptStart,ptEnd的连接线段与直线x=dXLine的交点的纵坐标
bool CGeometryTools::LineCrossXLine( const CMapPoint& ptStart, const CMapPoint& ptEnd, 
                                    double dXLine, double& dY )
{
    if( VALUE_EQUAL( dXLine, ptStart.x ) )
    {
        return false;
    }
    if( VALUE_EQUAL( ptStart.x, ptEnd.x ) && VALUE_NOTEQUAL( ptStart.x, dXLine ) )// 平行
    {
        return false;
    }    
    if( ( ptStart.x < dXLine ) && ( ptEnd.x < dXLine ) )// 位于直线左侧
    {
        return false;	
    }    
    if( ( ptStart.x > dXLine ) && ( ptEnd.x > dXLine ) )// 位于直线右侧
    {
        return false;	
    }    
    if( VALUE_EQUAL( ptEnd.x, dXLine ) )
    {
        dY = ptEnd.y;
        return true;
    }

    dY = ( ptEnd.y - ptStart.y ) * ( dXLine - ptStart.x ) * 1.0
        / ( ptEnd.x - ptStart.x ) + ptStart.y;
    
    return true;
}

