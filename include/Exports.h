#pragma once 

#ifndef WIN32
	#define CORE_API
#else
	#ifdef CORE_EXPORT
		#define CORE_API  __declspec( dllexport )
	#else
		#define CORE_API  __declspec( dllimport )
	#endif // 0
#endif // !WIN32