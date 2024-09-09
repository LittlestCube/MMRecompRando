#ifndef __MODDING_H__
#define __MODDING_H__

// Do not edit these defines. They use special section names that the recomp mod tool recognizes for specific modding functionality.

#define RECOMP_IMPORT(mod, func) \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"") \
    _Pragma("GCC diagnostic ignored \"-Wreturn-type\"") \
    __attribute__((noinline, weak, used, section(".recomp_import." mod))) func {} \
    _Pragma("GCC diagnostic pop")

#define RECOMP_EXPORT __attribute__((section(".recomp_export")))

#define RECOMP_PATCH __attribute__((section(".recomp_patch")))

#define RECOMP_FORCE_PATCH __attribute__((section(".recomp_force_patch")))

#define RECOMP_DECLARE_EVENT(func) \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"") \
    __attribute__((noinline, weak, used, section(".recomp_event"))) void func {} \
    _Pragma("GCC diagnostic pop")

#define RECOMP_CALLBACK(mod, event) __attribute__((section(".recomp_callback." mod ":" #event)))

#endif
