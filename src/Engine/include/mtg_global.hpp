#ifndef MTG_GLOBAL_HPP
#define MTG_GLOBAL_HPP


#ifdef MTG_STATIC
	#define MTG_DECL_EXPORT
	#define MTG_DECL_IMPORT
#else
	#ifndef MTG_SHARED
		#define MTG_SHARED
	#endif
	#ifdef _MSC_VER
		#define MTG_DECL_EXPORT __declspec(dllexport)
		#define MTG_DECL_IMPORT __declspec(dllimport)
	#else
		#define MTG_DECL_EXPORT
		#define MTG_DECL_IMPORT
	#endif
#endif


#ifdef MTG_ENGINE_LIB
	#define MTG_ENGINE_EXPORT MTG_DECL_EXPORT
#else
	#define MTG_ENGINE_EXPORT MTG_DECL_IMPORT
#endif

#endif //MTG_GLOBAL_HPP