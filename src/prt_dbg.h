
#ifndef prt_dbg_h__
#define prt_dbg_h__

#include <stdio.h>



#ifdef LIBSCH_DEBUG_PRINT
#define dbg_prt_fmt(m, ...)    \
    do {              \
        fprintf(stderr, "DEBUG %s:%d: " m "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while(0)        


#define dbg_prt(m) dbg_prt_fmt("%s", m)

#define dbg_prt_fn() dbg_prt_fmt("%s", __func__)

        
#else
#define dbg_prt(m, ...)
#endif


#endif /* prt_dbg_h__ */

