// DiskArcher.
// A special file for enumerations.
// (C) Marat Mirgaleev, 2004.
// Created 28.07.2004.
// Modifications:
//   (1) 07.05.2006. OpResult added.
//==============================================================================

#if !defined(_DiskArcher_Enums_H__INCLUDED_)
#define _DiskArcher_Enums_H__INCLUDED_

enum LocatorWhatToExclude
{ 
  LocExclFileTypes,
  LocExclFolders
};

enum OpResult   // (1) Resulting code of operation
                // They are in order of severity increasing
{
  OPR_SUCCESSFUL=0,
  OPR_WARNINGS,
  OPR_NONFATAL_ERRORS /* There are errors, but we can continue */,
  OPR_FATAL_STOP,
  OPR_USER_STOP
};

#endif // !defined(_DiskArcher_Enums_H__INCLUDED_)
