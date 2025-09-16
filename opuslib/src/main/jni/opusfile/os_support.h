/* os_support.h
   Portable aliases for 64-bit-safe file positioning.
   Include this BEFORE other system headers where possible.
*/
#ifndef OS_SUPPORT_H
#define OS_SUPPORT_H

/* --- POSIX-ish platforms: request 64-bit off_t in stdio --- */
#if !defined(_WIN32)
/* Make fseeko/ftello visible and make off_t 64-bit where applicable. */
#ifndef _FILE_OFFSET_BITS
#  define _FILE_OFFSET_BITS 64
#endif
#ifndef _LARGEFILE_SOURCE
#  define _LARGEFILE_SOURCE 1
#endif
#endif

#include <stdio.h>

/* --- Windows/MSVC: map to 64-bit CRT functions --- */
#if defined(_WIN32) && !defined(__MINGW32__)
  /* MSVC exposes _fseeki64/_ftelli64 for 64-bit positioning. */
  #ifndef fseeko
  #  define fseeko _fseeki64
  #endif
  #ifndef ftello
  #  define ftello _ftelli64
  #endif

/* --- Android quirks: older API levels hide fseeko/ftello --- */
#elif defined(__ANDROID__) && defined(__ANDROID_API__) && (__ANDROID_API__ < 24)
  /* On very old Android API levels (esp. 32-bit), just fall back. */
  #ifndef fseeko
  #  define fseeko(stream, offset, whence) fseek((stream), (long)(offset), (whence))
  #endif
  #ifndef ftello
  #  define ftello(stream) ftell((stream))
  #endif

/* --- Generic fallback if fseeko/ftello aren’t available --- */
#elif !defined(HAVE_FSEEKO)
  #ifndef fseeko
  #  define fseeko(stream, offset, whence) fseek((stream), (long)(offset), (whence))
  #endif
  #ifndef ftello
  #  define ftello(stream) ftell((stream))
  #endif
#endif

#endif /* OS_SUPPORT_H */
