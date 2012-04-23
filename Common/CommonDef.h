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

#ifndef _COMMONDEF_H_INCLUDED_
#define _COMMONDEF_H_INCLUDED_

#define INVALID_VALUE_32    -1
#define INVALID_VALUE_16    -1
#define INVALID_VALUE_8     -1  

#define SHP_POINT           0x00
#define SHP_LINE            0x01
#define SHP_AREA            0x02

#define BUFFER_SIZE         256
#define COOR_RATE           10000000.0//坐标单位系数
#define MAX_LATITUDE        90.0
#define PARCEL_BASE_WIDTH   ( COOR_RATE / 32 )
#define PARCEL_BASE_HEIGHT  ( COOR_RATE / 32 )

#define VALUE_EQUAL( x1, x2 )     ( ( x1 > x2 || x1 < x2  ) ? false : true )
#define VALUE_NOTEQUAL( x1, x2 )  ( ( x1 > x2 || x1 < x2  ) ? true : false )

#define FLOAT_TO_LONG( dx ) ( long( dx + 0.5 ) )
#define FLOAT_TO_ULONG( dx ) ( ULONG( dx + 0.5 ) )

//进度信息回调函数
typedef void(*LPCALLBACKFUNC)( const char* pszInfo, ULONG type );

#define COLUMN_COUNT        2
static  int nColumnWidth[COLUMN_COUNT] = { 120, 640 };
static LPTSTR lpstrColumnName[COLUMN_COUNT] = { TEXT("属性名称"), TEXT("属性值") };

#define WM_SELELEMCHANGED       WM_USER + 101
#define WM_SELELEMCHANGE        WM_USER + 102
#define WM_QUERY                WM_USER + 103 
////////////////////////////////////////////////////////////////////
#endif