/*!
 ***********************************************************************
 \par Copyright
 \verbatim
  ________  _____           _____   _____           ____  ____   ____		
     /     /       /       /       /       /     /   /    /   \ /			
    /     /___    /       /___    /       /____ /   /    /____/ \___			
   /     /       /       /       /       /     /   /    /           \		
  /     /_____  /_____  /_____  /_____  /     / _ /_  _/_      _____/ 		
   																				
  Copyright (c) 2010 Telechips Inc.
  Korad Bldg, 1000-12 Daechi-dong, Kangnam-Ku, Seoul, Korea					
 \endverbatim
 ***********************************************************************
 */

#ifndef __COMMON_H__
#define	__COMMON_H__

#include	"audio_common.h"

#ifndef FALSE	
	#define FALSE	0 
#endif

#ifndef TRUE		
	#define TRUE		1
#endif

typedef void*		 (malloc_func	) ( unsigned int );
typedef void		 (free_func		) ( void* );
typedef void*		 (memcpy_func	) ( void*, const void*, unsigned int );
typedef void		 (memset_func	) ( void*, int, unsigned int );
typedef void*		 (memmove_func	) ( void*, const void*, unsigned int );

#ifndef NULL
#define NULL	0
#endif

//! Callback Func
typedef struct cdk_callback_func_t
{
	void* (*m_pfMalloc			) ( unsigned int );								//!< malloc
	void* (*m_pfNonCacheMalloc	) ( unsigned int );								//!< non-cacheable malloc 
	void  (*m_pfFree			) ( void* );									//!< free
	void  (*m_pfNonCacheFree	) ( void* );									//!< non-cacheable free
	void* (*m_pfMemcpy			) ( void*, const void*, unsigned int );			//!< memcpy
	void  (*m_pfMemset			) ( void*, int, unsigned int );					//!< memset
	void* (*m_pfRealloc			) ( void*, unsigned int );						//!< realloc
	void* (*m_pfMemmove			) ( void*, const void*, unsigned int );			//!< memmove
	void* (*m_pfCalloc			) ( unsigned int , unsigned int );				//!< calloc
	int*  (*m_pfMemcmp			) ( const void* , const void*, unsigned int );	//!< memcmp

	void* (*m_pfPhysicalAlloc	) ( unsigned int );								//!< alloc function for physical memory allocation
	void  (*m_pfPhysicalFree	) ( void*, unsigned int );						//!< free function for physical memory free
	void* (*m_pfVirtualAlloc	) ( int*, unsigned int, unsigned int );			//!< alloc function for virtual memory allocation
	void  (*m_pfVirtualFree		) ( int*, unsigned int, unsigned int );			//!< free function for virtual memory free
	int m_Reserved1[16-13];

	void*		 (*m_pfFopen ) ( const char *, const char * );						//!< fopen
	unsigned int (*m_pfFread ) ( void*, unsigned int, unsigned int, void* );		//!< fread
	int			 (*m_pfFseek ) ( void*, long, int );								//!< fseek
	long		 (*m_pfFtell ) ( void* );											//!< ftell
	unsigned int (*m_pfFwrite) ( const void*, unsigned int, unsigned int, void* );	//!< fwrite
	int			 (*m_pfFclose) ( void* );											//!< fclose
	int			 (*m_pfUnlink) ( const char* );										//!< _unlink
	unsigned int (*m_pfFeof  ) ( void* );											//!< feof
	unsigned int (*m_pfFflush) ( void* );											//!< fflush

	int			 (*m_pfFseek64) ( void*, TCAS_S64, int );							//!< fseek 64bit io
	TCAS_S64	 (*m_pfFtell64) ( void* );											//!< ftell 64bit io
	int m_Reserved2[16-11];
} cdk_callback_func_t;

//#define DPRINTF	printf
#define DPRINTF

#endif
