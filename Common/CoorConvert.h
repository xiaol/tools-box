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


#ifndef _COORCONVERT_H_
#define _COORCONVERT_H_

#include "wtypes.h"
#include "MapShape.h"
#include "CommonDef.h"

#define MOVE_STEP				5


enum MoveFlag
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};

//////////////////////////////////////////////////////////////////
class CCoorConvert
{
public:

	CCoorConvert();

//对象初始化方法
public:

    void SetMapRect( const MAPRECT& rc ) { m_MapRect = rc; }
    void SetViewRect( const RECT& rc ) { m_ViewRect = rc; }
    void SetMapCenter( const MAPPOINT& pt ) { m_ptMapCenter = pt; }
    void SetCurScale( double dScale ) { m_dScale = dScale; }
    void UpdateWindow();

    void ZoomIn( const POINT& point );
    void ZoomOut( const POINT& point );
    void ZoomByRect( const RECT& rc );

    void Reset();

//获取相关信息方法
public:

	const RECT& GetViewRect() const { return m_ViewRect; }
	const MAPRECT& GetMapRect() const { return m_MapRect; }
	const MAPRECT& GetClipRect() const { return m_ClipRect; }
    const MAPPOINT& GetMapCenterPoint() const { return m_ptMapCenter; }
	DOUBLE GetCurScale() const { return m_dScale; }
    LONG GetScaleValue() { return m_nScaleValue; }

//坐标转换方法
public:

	void LPToDP( LONG dLPX, LONG dLPY, LONG& nDPX, LONG& nDPY );
	void DPToLP( LONG nDPX, LONG nDPY, LONG& dLPX, LONG& dLPY );

	void DPToLP( const RECT& rcDP, CMapRect& rcLP );
	void LPToDP( const CMapRect& rcLP, RECT& rcDP );

	void LPToDP( const MAPPOINT& ptLP, POINT& ptDP )
	{
		LPToDP( ptLP.x, ptLP.y, ptDP.x, ptDP.y );
	}
	void DPToLP( const POINT& ptDP, MAPPOINT& ptLP )
	{
		DPToLP( ptDP.x, ptDP.y, ptLP.x, ptLP.y );
	}
	void LPToDP( const LPMAPPOINT lpLP, LPPOINT lpDP )
	{
		if( lpLP == NULL || lpDP == NULL )
			return;
		LPToDP( lpLP->x, lpLP->y, lpDP->x, lpDP->y );
	}
	void DPToLP( const LPPOINT lpDP, LPMAPPOINT lpLP )
	{
		if( lpLP == NULL || lpDP == NULL )
			return;
		DPToLP( lpDP->x, lpDP->y, lpLP->x, lpLP->y );
	}

private:

	void UpdateClipRect();

private:

	MAPRECT m_MapRect;		//地图外接矩形
	MAPRECT m_ClipRect;		//当前裁剪矩形

	RECT m_ViewRect;        //当前视口范围
    LONG m_nOffsetX;        //当前X坐标平移
    LONG m_nOffsetY;		//当前Y坐标平移

    MAPPOINT m_ptMapCenter;
	DOUBLE m_dScale;        //缩放比例
    LONG m_nScaleValue;
};

#endif