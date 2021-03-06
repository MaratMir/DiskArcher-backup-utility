// DiskArcher.
// CFolder class implementation file.
// (C) Marat Mirgaleev, 2001-2002.
// Modifications:
//  (1) 25.07.02. Analyze() moved into CNewFilesLocator class.
//  (2) 25.08.02. SetType() in constructor.
//                GetItems() implemented.
//==========================================================================

#include "stdafx.h"
#include <ATLComTime.h>
#include "CFolder.h"

#include "CFileOnDisk.h"


//========================================================================
CFolder::CFolder( const CString& strFullName ) : CDiskItem( strFullName )
{
  m_pParentFolder = nullptr;
  setType( DI_FOLDER ); // (2)
    // Probably, we know that this is a Folder, not a File, 
    //  if we call this constructor
}


CFolder::~CFolder()
{
  clear();
}


// (2) Get all subfolders and files which are placed within the folder
//========================================================================
int CFolder::getItems()
{
  clear();

  INT result = IDOK;
  bool firstLoop=true, nextNotFound=true;
  HANDLE hFind=0;

// Find each file or folder in the folder
  while ( result == IDOK )
  {
    WIN32_FIND_DATA FindFileData;
    if ( firstLoop )
    {
      CString mask = getFullName(); // !!!
      if( mask.Right( 1 ) != "\\" )
        mask += "\\";
      mask += "*.*";

      hFind = FindFirstFile( mask, &FindFileData );
      firstLoop    = false;
      nextNotFound = false;
    }
    else
      if ( ! FindNextFile( hFind, &FindFileData ))
        nextNotFound = true;

    if ( ( hFind == INVALID_HANDLE_VALUE ) || nextNotFound )
      if (   GetLastError() == ERROR_FILE_NOT_FOUND 
        || GetLastError() == ERROR_SUCCESS
        || GetLastError() == ERROR_NO_MORE_FILES  )
      {
        result = IDOK; // Ok, files not found
        break;
      }
      else
      {
      // Unable to get directory information - display message
        AfxMessageBox( L"Unable to get directory information:\n" + getFullName() );
        break;
      }
    else // File found
      if (  ( CString( "." ).Compare( FindFileData.cFileName ) != 0 )
          &&  CString( ".." ).Compare( FindFileData.cFileName ) != 0 )
      // If it isn't "current directory" or "parent directory" entry
      {
        // If it is a directory, create a CFolder object
        CDiskItem *pNewItem;
        if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
          pNewItem = new CFolder( AddWithSlash( this->getFullName(), FindFileData.cFileName ));

        // This is just a file - create an CFileOnDisk object
        else
          pNewItem = new CFileOnDisk( AddWithSlash( this->getFullName(), FindFileData.cFileName ));

        pNewItem->m_LastWriteTime = FindFileData.ftLastWriteTime;
        pNewItem->m_pParentFolder = this;

        m_items.push_back( pNewItem );
      }
  } // End of while ( result == IDOK )
  
  if ( hFind != INVALID_HANDLE_VALUE )
    if( ! FindClose(hFind) ) // Free memory used while file searching
      AfxMessageBox( _T("FileFind Close Error") );

  return result;
}


//========================================================================
void CFolder::clear(void)
{
  for( const auto& curItem : m_items )
    delete curItem;
  m_items.clear();
}
