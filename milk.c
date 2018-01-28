/*
 * 'almond milk' for LG's almond drm
 *
 * Tim 'diff' Strazzere
 *   <strazz@gmail.com>
 *   <diff@sentinelone.com>
 *
 *
 * Special thanks to rovo89 for pushing me to
 *   release this sucker :D
 */

#include <stdlib.h> // avoid exit warning
#include <stdio.h>

#include <dlfcn.h>  // dlopen/dlclose
#include <unistd.h> // access
#include <jni.h>    // jni stuff

#define ALMOND_LIB "/system/lib/liblgalmond.so"

typedef int (*CopyDexToFileFn)(int, int, int, size_t*, uint8_t*, int32_t*, uint8_t*);
typedef int (*CopyDexToMemFn)(void*, size_t, size_t*, uint8_t*, uint8_t*);

// Checks first 0x20 bytes for signs of being a preloaded and encrypted dex
typedef int (*Almond_Is_DRMDex)(const void*, size_t);
// isDRM returns
#define NOT_PROTECTED 0 // Also used for an error occuring
#define PRELOADED_DEX 1
#define ODEX          2
#define OAT           3
// Anything will return PRELOADED_DEX if it has;
//    \x01\x18\x24application/octet
// at the start of the file.

typedef int (*almdUtilGetKey)(void*, void*, void*, void*);
typedef int (*almdUtilCheckPreloadID)(void*);
typedef int (*almdUtilGetPreloadKey)(void*, void*, void*, void*);


int main(int argc, const char* argv[]) {

  unsigned char header[] = {
    0x01, 0x18, 0x24, 0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x2F, 0x6F,
    0x63, 0x74, 0x65, 0x74, 0x2D, 0x73, 0x74, 0x72, 0x65, 0x61, 0x6D, 0x72, 0x65, 0x38, 0x5A, 0x2F
  };

  printf(" [+] Attempting to load Almond library\n");
  void* handle = dlopen(ALMOND_LIB, RTLD_LAZY);

  if(handle == NULL) {
    printf(" [!] Could not load Almond library : %s\n", dlerror());
    return -1;
  }
  printf(" [+] Library Loaded!\n");

  almdUtilCheckPreloadID checkPreloadID = dlsym(handle, "almdUtilCheckPreloadID");
  if(checkPreloadID == NULL) {
    printf(" [!] Function not found!\n");
    return -1;
  }

  if(checkPreloadID("testKey" != 1)) {
    printf("Wrong key!\n");
  } else {
    printf("Correct key!\n");
  }

  almdUtilGetPreloadKey getPreloadKey = dlsym(handle, "almdUtilGetPreloadKey");
  Almond_Is_DRMDex isDRMDex = dlsym(handle, "Almond_Is_DRMDex");

  if(isDRMDex(header, sizeof(header)) != PRELOADED_DEX) {
    printf("Not encrypted\n");
  } else {
    printf("Encrypted\n");
  }

  almdUtilGetKey getKey = dlsym(handle, "almdUtilGetKey");
  if(getKey("blah0", sizeof("blah0"), "blah2", sizeof("blah2")) != 0) {
    printf("wtf?\n");
  }

  printf(" [*] Attempting to obtain key\n");
  // 3rd and 4th get hashed
  if(getPreloadKey("blah0", "blah1", "blah2", "blah3") != 0) {
    printf(" [!] Failure occured getting preload key!\n");
  }

  printf(" [+] Closing library\n");
  dlclose(handle);

  return 0;
}

