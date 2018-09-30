#ifndef OLYLUMORAY_API_H
#define OLYLUMORAY_API_H

#if defined(D_BAM_PLATFORM_WINDOWS)
# ifdef D_BAM_DYNAMICLIBRARY_BUILD
#  define OLYLUMORAYAPI __declspec(dllexport)
# else
#  define OLYLUMORAYAPI __declspec(dllimport)
# endif
#elif __GNUC__ >= 4
#define OLYLUMORAYAPI __attribute__ ((visibility("default")))
#else
#error Unsupported platform
#endif

#endif // OLYLUMORAY_API_H
