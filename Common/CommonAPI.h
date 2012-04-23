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

#ifndef _COMMONAPI_H_INCLUDE_
#define _COMMONAPI_H_INCLUDE_

#include <vector>
#include <assert.h>
using std::vector;

//用于释放存储指针的向量，ptr必须为对象指定
template<class ptr>
void ClearVector( vector<ptr>* pVector )
{
	if( pVector == NULL )
		return;
	
    vector<ptr>::iterator itr = pVector->begin();
    while( itr != pVector->end() )
    {
        delete *itr;
        itr++;
    }
	pVector->clear();
}

static int strReplace( char* psz, char* pOldChar, char* pNewChar, size_t maxLen )
{
    assert( psz != NULL && pOldChar != NULL && pNewChar != NULL );

    // Set buffer & maximum length.
    const int MAX_LEN = 65535;
    char szBuf[MAX_LEN + 1];

    int count = 0;
    char s1, s2, d1, d2;
    char c, c2, *src = psz, *des = szBuf;

    // Save old char & new char.
    if( * pOldChar >= 0 )
    {  
        s1 = 0;  
        s2 = *pOldChar; 
    }
    else
    {  
        s1 = *pOldChar;  
        s2 = *( pOldChar + 1 ); 
    }

    if( * pNewChar >= 0 )
    {  
        d1 = 0;  
        d2 = *pNewChar;
    }
    else
    {  
        d1 = *pNewChar;  
        d2 = *( pNewChar + 1 ); 
    }

    if( s1 == 0 && d1 == 0 )      // ( ASCII, ASCII )
    {
        while( ( c = * src++ ) != '\0' )
        {
            if( c < 0 )
            {
                c2 = * src++;
                *des++ = c;  
                *des++ = c2;
            }
            else
            {
                if( c == s2 )
                {
                    if ( d2 != 0 ) 
                    { 
                        * des++ = d2;
                    }
                    count++;
                }
                else
                {  
                    * des++ = c; 
                }
            }
        }
    }
    else if( s1 == 0 && d1 < 0 )  // ( ASCII, MBCS )
    {
        while( ( c = *src++ ) != '\0' )
        {
            if( c < 0 )
            {
                c2 = * src++;
                * des++ = c;  
                * des++ = c2;
            }
            else
            {
                if( c == s2 )
                {  
                    *des++ = d1;  
                    *des++ = d2;  
                    count++; 
                }
                else
                {  
                    *des++ = c;
                }
            }
        }
    }
    else if( s1 < 0 && d1 == 0 )  // ( MBCS, ASCII )
    {
        while( ( c = *src++ ) != '\0' )
        {
            if( c < 0 )
            {
                c2 = *src++;
                if( c == s1 && c2 == s2 )
                {  
                    if ( d2 != 0 ) 
                    { 
                        *des++ = d2; 
                    }
                    count++;
                }
                else
                {  
                    *des++ = c;  
                    *des++ = c2; 
                }
            }
            else
            {  *des++ = c; 
            }
        }
    }
    else if( s1 < 0 && d1 < 0 )   // ( MBCS, MBCS )
    {
        while( ( c = *src++ ) != '\0' )
        {
            if ( c < 0 )
            {
                c2 = *src++;
                if ( c == s1 && c2 == s2 )
                {  
                    *des++ = d1;  
                    *des++ = d2;  
                    count++; 
                }
                else
                {  
                    *des++ = c;  
                    *des++ = c2; 
                }
            }
            else
            {  *des++ = c; 
            }
        }
    }

    *des = '\0';  // Add termination symbol.

    // Verify buffer length supported by user.
    size_t len = strlen( szBuf );
    if ( len > maxLen )
        return -1;

    if ( count > 0 )
    {

        strcpy_s( psz, maxLen, szBuf );
    }

    return count;
}

static void TrimAll( char* psz, char* pChar )
{
    assert( psz != NULL && pChar != NULL );

    size_t maxLen = strlen( psz );
    strReplace( psz, pChar, "", maxLen );
}

static void TrimAll( char* psz )
{
    assert( psz != NULL );

    size_t maxLen = strlen( psz );
    strReplace( psz, " ", "", maxLen );
}
#endif