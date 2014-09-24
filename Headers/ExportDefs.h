/*
 *  ExportDefs.h
 *
 *  Created by Hubert Affolter on 17-Mar-2008.
 *  Copyright 2005-2011 HEKA Electronik GmbH
 *
 */

// ITC_Export is a manifest constant used as a prefix to all function
// prototypes. If a particular prefix is needed (e.g. "PASCAL"), define
// ITC_Export appropriately.
// For Windows DLL builds we need to mark the exported functions as well.
// For MacOS frameworks, define exported symbols.
// For MacOS frameworks, define weak linking.

//#define ITC_Export_Symbols

#ifndef ITC_Export
   #ifdef _WINDOWS
      #ifdef ITC_Suppress_Symbols
         #define ITC_Export  
         #define ITC_Import  
      #else
         #define ITC_Export __declspec(dllexport) 
         #define ITC_Import  
      #endif
   #else
      #ifdef ITC_Suppress_Symbols
         #define ITC_Export  
         #define ITC_Import  
      #else
         #define ITC_Export __attribute__((visibility("default"))) 
         #define ITC_Import __attribute__((weak_import))
      #endif
   #endif // _WINDOWS
#endif // HEKA_Export

#define HEKA_RELEASE_DATE  "03-Jun-2013"

