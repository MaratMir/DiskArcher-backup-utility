// Miscelaneous functions.
// (C) Marat Mirgaleev, 2001-2014.
//=================================================================

#if !defined(MISC_H__INCLUDED_)
#define MISC_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

CString GetErrDescription( bool bWithoutLF=false );

CString RemoveRightSlash( const CString& str );

#endif // !defined(MISC_H__INCLUDED_)
