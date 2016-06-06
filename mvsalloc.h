// -*- C++ -*-
#ifndef MVSALLOC_H_
#define MVSALLOC_H_

#ifdef __cplusplus
extern "C" {
#endif

void* getmain31(int size, int subpool);
int freemain31(void * data, int size, int subpool);

#ifdef __cplusplus
}
#endif

#endif // MVSALLOC_H_

