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


//�������ľ���
double CGeometryTools::GetTwoPointDistance( const CMapPoint* pPt1, const CMapPoint* pPt2 )
{
    if( ( pPt1 == NULL ) || ( pPt2 == NULL ) )
    {
        return 0;
    }
    return _hypot( pPt1->x - pPt2->x, pPt1->y - pPt2->y );
}

//�������ľ���
double CGeometryTools::GetTwoPointDistance( const CMapPoint& pt1, const CMapPoint& pt2 )
{
    return _hypot( pt1.x - pt2.x, pt1.y - pt2.y );
}

//�ú��׹�ʽ��㵽�߶μ�ľ���
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

//���Ե�ptΪԭ���ˮƽ������������߶εĽ���
//����ֵ��ʶ�����״��
// -1: �޽��� 0: �������߶μ�  1: ����Ϊ�߶����  2: ����Ϊ�߶��յ�
int CGeometryTools::GetInterSectionOfHLToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
                                              const CMapPoint& ptEnd, CMapPoint& ptInterSection )
{
    //�����߶ε����Y���ꡢ��СY�����X����,Ԥ���ж��������߶������ཻ������
    if( ( ptOrigin.y > max( ptStart.y, ptEnd.y ) )
        || ( ptOrigin.y < min( ptStart.y, ptEnd.y ) )
        || ( ptOrigin.x < min( ptStart.x, ptEnd.x ) ) )
    {
        return -1;
    }
    
    //����߶�Ϊˮƽ�߶�,�����Ϊ��ˮƽ�����޽���
    if( VALUE_EQUAL( ptEnd.y, ptStart.y ) )
    {
        return -1;
    }
    //����߶ε������������,�򽻵�Ϊ�߶����
    if( VALUE_EQUAL( ptOrigin.y, ptStart.y ) )
    {
        ptInterSection = ptStart;
        return 1;
    }
    
    //����߶ε��յ���������,���յ�Ϊ�߶����
    if( VALUE_EQUAL( ptOrigin.y, ptEnd.y ) )
    {
        ptInterSection = ptEnd;
        return 2;
    }
    
    //����߶�Ϊ��ֱ�߶�
    if( VALUE_EQUAL( ptEnd.x, ptStart.x ) )
    {
        ptInterSection.x = ptStart.x;
        ptInterSection.y = ptOrigin.y;
        return 0;
    }
    
    //�߶ε�б��
    double dSlope = ( ptEnd.y - ptStart.y ) * 1.0 / ( ptEnd.x - ptStart.x );
    ptInterSection.y = ptOrigin.y;
    ptInterSection.x = (long)( ( ptOrigin.y - ptStart.y ) / dSlope + ptStart.x );
    
    //������㲻��������,���������߶��޽���
    if( ptInterSection.x > ptOrigin.x )
    {
        ptInterSection.x = 0;
        ptInterSection.y = 0;
        return -1;
    }
    
    return 0;
}

//���Ե�ptΪԭ���ˮƽ���ҵ��������߶εĽ���
//����ֵ��ʶ�����״��
// -1: �޽��� 0: �������߶μ�  1: ����Ϊ�߶����  2: ����Ϊ�߶��յ�
int CGeometryTools::GetInterSectionOfHRToLine(  const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
                                              const CMapPoint& ptEnd, CMapPoint& ptInterSection )
{
    //�����߶ε����Y���ꡢ��СY�����X����,Ԥ���ж��������߶������ཻ������
    if( ( ptOrigin.y > max( ptStart.y, ptEnd.y ) )
        || ( ptOrigin.y < min( ptStart.y, ptEnd.y ) )
        || ( ptOrigin.x > max( ptStart.x, ptEnd.x ) ) )
    {
        return -1;
    }
    
    //����߶�Ϊˮƽ�߶�,�����Ϊ��ˮƽ�����޽���
    if( VALUE_EQUAL( ptEnd.y, ptStart.y ) )
        return -1;
    
    //����߶ε������������,�򽻵�Ϊ�߶����
    if( VALUE_EQUAL( ptOrigin.y, ptStart.y ) )
    {
        ptInterSection = ptStart;
        return 1;
    }
    
    //����߶ε��յ���������,���յ�Ϊ�߶����
    if( VALUE_EQUAL( ptOrigin.y, ptEnd.y ) )
    {
        ptInterSection = ptEnd;
        return 2;
    }
    
    //����߶�Ϊ��ֱ�߶�
    if( VALUE_EQUAL( ptEnd.x, ptStart.x ) )
    {
        ptInterSection.x = ptStart.x;
        ptInterSection.y = ptOrigin.y;
        return 0;
    }
    
    //�߶ε�б��
    double dSlope = ( ptEnd.y - ptStart.y ) * 1.0 / ( ptEnd.x - ptStart.x );
    ptInterSection.y = ptOrigin.y;
    ptInterSection.x = (long)( ( ptOrigin.y - ptStart.y ) / dSlope + ptStart.x );
    
    //������㲻��������,���������߶��޽���
    if( ptInterSection.x < ptOrigin.x )
    {
        ptInterSection.x = 0;
        ptInterSection.y = 0;
        return -1;
    }
    return 0;
}

//���Ե�ptΪԭ��Ĵ�ֱ���ϵ��������߶εĽ���
//����ֵ��ʶ�����״��
// -1: �޽��� 0: �������߶μ�  1: ����Ϊ�߶����  2: ����Ϊ�߶��յ�
int CGeometryTools::GetInterSectionOfVTToLine(  const CMapPoint& ptOrigin, 
                                              const CMapPoint& ptStart, 
                                              const CMapPoint& ptEnd, 
                                              CMapPoint& ptInterSection )
{
    //�����߶ε����Y���ꡢ��СY�����X����,Ԥ���ж��������߶������ཻ������
    if( ( ptOrigin.x > max( ptStart.x, ptEnd.x ) )
        || ( ptOrigin.x < min( ptStart.x, ptEnd.x ) )
        || ( ptOrigin.y > max( ptStart.y, ptEnd.y ) ) )
    {
        return -1;
    }
    
    //����߶�Ϊ��ֱ�߶�,�����Ϊ��ˮƽ�����޽���
    if( VALUE_EQUAL( ptEnd.x, ptStart.x ) )
        return -1;
    
    //����߶ε������������,�򽻵�Ϊ�߶����
    if( VALUE_EQUAL( ptOrigin.x, ptStart.x ) )
    {
        ptInterSection = ptStart;
        return 1;
    }
    
    //����߶ε��յ���������,���յ�Ϊ�߶����
    if( VALUE_EQUAL( ptOrigin.x, ptEnd.x ) )
    {
        ptInterSection = ptEnd;
        return 2;
    }
    
    //����߶�Ϊˮƽ�߶�
    if( VALUE_EQUAL( ptEnd.y, ptStart.y ) )
    {
        ptInterSection.x = ptOrigin.x;
        ptInterSection.y = ptStart.y;
        return 0;
    }
    
    //�߶ε�б��
    double dSlope = ( ptEnd.y - ptStart.y ) * 1.0 / ( ptEnd.x - ptStart.x );
    ptInterSection.x = ptOrigin.x;
    ptInterSection.y = (long)( ( ptOrigin.x - ptStart.x ) * dSlope + ptStart.y );
    
    //������㲻��������,���������߶��޽���
    if( ptInterSection.y < ptOrigin.y )
    {
        ptInterSection.x = 0;
        ptInterSection.y = 0;
        return -1;
    }
    return 0;
}

//���Ե�ptΪԭ��Ĵ�ֱ���µ��������߶εĽ���
//����ֵ��ʶ�����״��
// -1: �޽��� 0: �������߶μ�  1: ����Ϊ�߶����  2: ����Ϊ�߶��յ�
int CGeometryTools::GetInterSectionOfVBToLine( const CMapPoint& ptOrigin, const CMapPoint& ptStart, 
                                              const CMapPoint& ptEnd, CMapPoint& ptInterSection )
{
    //�����߶ε����Y���ꡢ��СY�����X����,Ԥ���ж��������߶������ཻ������
    if( ( ptOrigin.x > max( ptStart.x, ptEnd.x ) )
        || ( ptOrigin.x < min( ptStart.x, ptEnd.x ) )
        || ( ptOrigin.y < min( ptStart.y, ptEnd.y ) ) )
    {
        return -1;
    }
    
    //����߶�Ϊ��ֱ�߶�,�����Ϊ��ˮƽ�����޽���
    if( VALUE_EQUAL( ptEnd.x, ptStart.x ) )
        return -1;
    
    //����߶ε������������,�򽻵�Ϊ�߶����
    if( VALUE_EQUAL( ptOrigin.x, ptStart.x ) )
    {
        ptInterSection = ptStart;
        return 1;
    }
    
    //����߶ε��յ���������,���յ�Ϊ�߶����
    if( VALUE_EQUAL( ptOrigin.x, ptEnd.x ) )
    {
        ptInterSection = ptEnd;
        return 2;
    }
    
    //����߶�Ϊˮƽ�߶�
    if( VALUE_EQUAL( ptEnd.y, ptStart.y ) )
    {
        ptInterSection.x = ptOrigin.x;
        ptInterSection.y = ptStart.y;
        return 0;
    }
    
    //�߶ε�б��
    double dSlope = ( ptEnd.y - ptStart.y ) * 1.0 / ( ptEnd.x - ptStart.x );
    ptInterSection.x = ptOrigin.x;
    ptInterSection.y = (long)(( ptOrigin.x - ptStart.x ) * dSlope 
        + ptStart.y);
    
    //������㲻��������,���������߶��޽���
    if( ptInterSection.y > ptOrigin.y )
    {
        ptInterSection.x = 0;
        ptInterSection.y = 0;
        return -1;
    }
    return 0;
}

//��������ε�λ�ù�ϵ
// -2: �Ƿ��Ķ���� 
// -1: ���ڶ�����ⲿ   0: ���ڶ���ζ����� 
//  1: ���ڶ���α���   2: ���ڶ�����ڲ�
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
    
    //�ĸ���������������αߵĽ��������,
    //�������һ��������Ϊ����,����ڶ�����ڲ� 
    int nCntHL = 0;//ˮƽ��������������ߵıߵĽ��������
    int nCntHR = 0;//ˮƽ���ҵ����������ߵıߵĽ��������
    int nCntVT = 0;//��ֱ���ϵ����������ߵıߵĽ��������
    int nCntVB = 0;//��ֱ���µ����������ߵıߵĽ��������
    
    int nReturn = 0;//���������ε�ĳһ���ߵĽ����������ֵ
    
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
        
        //����mapptΪԭ��,ˮƽ����������뵱ǰ�ߵĽ������
        nReturn = GetInterSectionOfHLToLine( ptOrigin, ptStart, ptEnd, pt );
        switch( nReturn )
        {
        case 0:
            {
                nCntHL++;
            }
            break;
        case 2://�������Ϊ��ǰ�߶ε��յ�,���ж϶���ε���һ�����������ߵ�λ�����
            //����߶ε�������һ������ֲ������ߵ�����,�򽻵��������1
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
        
        //����mapptΪԭ��,ˮƽ���ҵ������뵱ǰ�ߵĽ������
        nReturn = GetInterSectionOfHRToLine( ptOrigin, ptStart, ptEnd, pt );
        switch( nReturn )
        {
        case 0:
            {
                nCntHR++;
            }
            break;
        case 2://�������Ϊ��ǰ�߶ε��յ�,���ж϶���ε���һ�����������ߵ�λ�����
            //����߶ε�������һ������ֲ������ߵ�����,�򽻵��������1
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
        
        //����mapptΪԭ��,��ֱ���ϵ������뵱ǰ�ߵĽ������
        nReturn = GetInterSectionOfVTToLine( ptOrigin, ptStart, ptEnd, pt );
        switch( nReturn )
        {
        case 0:
            {
                nCntVT++;
            }
            break;
        case 2://�������Ϊ��ǰ�߶ε��յ�,���ж϶���ε���һ�����������ߵ�λ�����
            //����߶ε�������һ������ֲ������ߵ�����,�򽻵��������1
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
        
        //����mapptΪԭ��,��ֱ���µ������뵱ǰ�ߵĽ������
        nReturn = GetInterSectionOfVBToLine( ptOrigin, ptStart, ptEnd, pt );
        switch( nReturn )
        {
        case 0:
            {
                nCntVB++;
            }
            break;
        case 2://�������Ϊ��ǰ�߶ε��յ�,���ж϶���ε���һ�����������ߵ�λ�����
            //����߶ε�������һ������ֲ������ߵ�����,�򽻵��������1
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
    
    //�жϽ������������ż��
    if( ( ( nCntHL % 2 ) == 1 ) || ( ( nCntHR % 2 ) == 1 )  
        || ( ( nCntVT % 2 ) == 1 ) || ( ( nCntVB % 2 ) == 1 ) )
    {
        return 2;
    }
    return -1;
}

//0: ����ֱ����
//1: ����ֱ���ӳ�����
//2: ����ֱ����
//-1: ����ֵ
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

//��(dx1,dy1)Ϊԭ��,������(ˮƽ����)��������(dx1,dy1)->(dx0,dy0)����ʱ��н�
float CGeometryTools::GetLineAngle( const CMapPoint& ptStart, const CMapPoint& ptEnd )
{
    float fAngle = 0;
    if( VALUE_EQUAL( ptStart.x, ptEnd.x ) )  //��ֱ����
    {
        if( ptStart.y > ptEnd.y )//����
        {
            return 90;
        }
        else//����
        {
            return 270;
        }
    }
    else if( VALUE_EQUAL( ptStart.y, ptEnd.y ) )//ˮƽ����
    {
        if( ptEnd.x > ptStart.x )//����
        {
            return 0;
        }
        else//����
        {
            return 180;
        }
    }
    
    fAngle = (float)( ( atan2( ( ptStart.y - ptEnd.y ) * 1.0, ptStart.x - ptEnd.x ) * 180 ) / PI );
    
    if( ptStart.y < ptEnd.y )//����������������
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

//��(dx0,dy0)Ϊԭ��,������(dx0,dy0)->(dx1,dy1)����������(��ֱ����)����ʱ��н�
float CGeometryTools::CalcAngleToNorth( double dx0, double dy0, double dx1, double dy1 )
{
    float fAngle = 0;
    if( dx0 == dx1 )  //��ֱ����
    {
        if( dy1 > dy0 )//����
        {
            return 0;
        }
        else//����
        {
            return 180;
        }
    }
    else if( dy0 == dy1 )//ˮƽ����
    {
        if( dx0 < dx1 )//����
        {
            return 270;
        }
        else//����
        {
            return 90;
        }
    }
    
    fAngle = (float)( ( atan2( dy1 - dy0, dx1 - dx0 ) * 180.0 ) / PI );
    fAngle -= 90;
    if( fAngle < 0 )//����������������
    {
        fAngle += 360;
    }
    
    return fAngle;
}

//dXΪ��ptStart,ptEnd�������߶���ֱ��y=dYLine�Ľ���ĺ�����
bool CGeometryTools::LineCrossYLine( const CMapPoint& ptStart, const CMapPoint& ptEnd, 
                                    double dYLine, double& dX )
{
    if( VALUE_EQUAL( dYLine, ptStart.y ) )
    {
        return false;
    }
    if( VALUE_EQUAL( ptStart.y, ptEnd.y ) && VALUE_NOTEQUAL( ptStart.y, dYLine ) )// ƽ��
    {
        return false;			
    }    
    if( ( ptStart.y < dYLine ) && ( ptEnd.y < dYLine ) )// λ��ֱ���²�
    {
        return false;	
    }    
    if( ( ptStart.y > dYLine ) && ( ptEnd.y > dYLine ) )// λ��ֱ���ϲ�
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

//dYΪ��ptStart,ptEnd�������߶���ֱ��x=dXLine�Ľ����������
bool CGeometryTools::LineCrossXLine( const CMapPoint& ptStart, const CMapPoint& ptEnd, 
                                    double dXLine, double& dY )
{
    if( VALUE_EQUAL( dXLine, ptStart.x ) )
    {
        return false;
    }
    if( VALUE_EQUAL( ptStart.x, ptEnd.x ) && VALUE_NOTEQUAL( ptStart.x, dXLine ) )// ƽ��
    {
        return false;
    }    
    if( ( ptStart.x < dXLine ) && ( ptEnd.x < dXLine ) )// λ��ֱ�����
    {
        return false;	
    }    
    if( ( ptStart.x > dXLine ) && ( ptEnd.x > dXLine ) )// λ��ֱ���Ҳ�
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

