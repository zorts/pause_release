// -*- C++ -*-
#ifndef WAITPOST_H_
#define WAITPOST_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * NOTE: ECBs MUST reside below the bar, in 31-bit land.
 */

void mvs_wait(uint32_t* ecb);
void mvs_post(uint32_t* ecb);

/*
  These variants are solely for timing purposes and 
  should not be used in real code.
 */
void mvs_direct_wait(uint32_t* ecb);
void mvs_direct_post(uint32_t* ecb);

#ifdef __cplusplus
}
#endif

#endif // WAITPOST_H_

