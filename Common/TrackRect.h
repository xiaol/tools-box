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

#ifndef _TRACKRECT_H_
#define _TRACKRECT_H_

//////////////////////////////////////////////////////////////////////////////
// CTrackRect - Declaration

class CTrackRect
{
// Construction / Destruction
public:
    CTrackRect();
    ~CTrackRect();

public:
    // Initialize object
    void Initialize( int nPenStyle, int nPenWidth, COLORREF crPenColor );

    // Set begin point of track rectangle.
    void SetBgnPoint( CPoint& bgnPoint );

    // Set end point of track rectangle.
    void SetEndPoint( CPoint& endPoint );

    // Draw track rectangle.
    void Draw( CDC* pDC );

    // Erase track rectangle.
    void Erase( CDC* pDC );

// Interface
public:

// Method
private:


// Attribute
private:
    CPen m_pen;  // A pen for draw track rectangle.
public:
    CPoint m_BgnPoint;  // Begin point of track rectangle.
    CPoint m_EndPoint;  // End point of track rectangle.

};

//////////////////////////////////////////////////////////////////////////////
#endif  // _TRACKRECT_H_