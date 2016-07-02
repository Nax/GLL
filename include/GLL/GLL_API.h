#ifndef GLL_API_H
#define GLL_API_H

#ifdef __cplusplus
# define GLL_EXTERN extern "C"
#else
# define GLL_EXTERN
#endif

#ifdef _WIN32
# ifdef GLL_STATIC
#  define GLL_API_DLL
# elif defined(GLL_EXPORT)
#  define GLL_API_DLL __declspec(dllexport)
# else
#  define GLL_API_DLL __declspec(dllimport)
# endif
#else
# define GLL_API_DLL
#endif

#define GLL_API GLL_EXTERN GLL_API_DLL

#endif
