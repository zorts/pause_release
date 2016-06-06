// -*- C++ -*-

#include "pe.h"
#include "timer.h"
#include "mvsalloc.h"
#include "waitpost.h"
#include "options.h"

#include <pthread.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


template <typename SYNC>
struct PingPongParm{
  PingPongParm(uint64_t count)
    : count_(count){}
  uint64_t count_;
  SYNC ping_;
  SYNC pong_;
};

template <typename SYNC>
void* pingPongThread(void* arg)
{
  PingPongParm<SYNC>* params = (PingPongParm<SYNC>*) arg;
  for (uint64_t i = 0; i < params->count_; ++i){
    params->ping_.pause();
    params->pong_.release();
  }
  return (void*) 0;
}

template <typename SYNC>
void pingPong(uint64_t n, bool verbose, const char* label)
{
  PingPongParm<SYNC> params(n);
  Timer t;
  pthread_t tid;
  int rc;
  if (0 != (rc = pthread_create(&tid, 0, &pingPongThread<SYNC>, &params))) {
    fprintf(stderr, "pthread_create failed for pingPong<%s> thread: %d\n", label, rc);
    exit(1);
  }
  if (verbose){
    sleep(1);
    params.ping_.printInfo(stdout, "ping");
    params.pong_.printInfo(stdout, "pong");
  }
  
  t.start();
  for (uint64_t i = 0; i < n; ++i){
    params.ping_.release();
    params.pong_.pause();
  }
  t.stop();
  printf("%s ping-pong test, %" PRIu64 " iterations\n", label, n);
  printf("user: %5.2f, system: %5.2f, total: %5.2f, elapsed: %5.2f\n",
         t.user(), t.system(), t.total(), t.wall());
  double cpuPer = t.total()/((double) n);
  double elapsedPer = t.wall()/((double) n);
  printf("CPU time per call:     %12.10f\n"
         "elapsed time per call: %12.10f\n",
         cpuPer, elapsedPer);
  fflush(stdout);
  if (verbose){
    params.ping_.printInfo(stdout, "ping");
    params.pong_.printInfo(stdout, "pong");
  }
  void* threadResult = 0;
  if (0 != (rc = pthread_join(tid, &threadResult))){
    fprintf(stderr, "pthread_join failed for pingPong<%s> thread: %d\n", label, rc);
    exit(1);
  }
}

class ECB{
public:
  ECB()
    : ecb_((uint32_t*) getmain31(8,0))
  {
    *ecb_ = 0;
  }
  ~ECB()
  {
    freemain31(ecb_, 8, 0);
    ecb_ = 0;
  }

  inline
  void pause()
  {
    mvs_wait(ecb_);
  }

  inline
  void release()
  {
    mvs_post(ecb_);
  }

  void printInfo(FILE* f, const char* label)
  {
    if (label){
      fprintf(f, "%s: ", label);
    }
    ;
    fprintf(f,
            "ecb address:  0x%016" PRIx64 "\n"
            "ecb value:    0x%" PRIx32 "\n",
            (uint64_t) ecb_, *ecb_);
    fflush(f);
  }

private:
  // prohibit copy/assign; do not implement
  ECB(const ECB&);
  ECB& operator=(const ECB&);
  uint32_t* ecb_;
};

class ECB_DIRECT_POST{
public:
  ECB_DIRECT_POST()
    : ecb_((uint32_t*) getmain31(8,0))
  {
    *ecb_ = 0;
  }
  ~ECB_DIRECT_POST()
  {
    freemain31(ecb_, 8, 0);
    ecb_ = 0;
  }

  inline
  void pause()
  {
    mvs_wait(ecb_);
  }

  inline
  void release()
  {
    mvs_direct_post(ecb_);
  }

  void printInfo(FILE* f, const char* label)
  {
    if (label){
      fprintf(f, "%s: ", label);
    }
    ;
    fprintf(f,
            "ecb address:  0x%016" PRIx64 "\n"
            "ecb value:    0x%" PRIx32 "\n",
            (uint64_t) ecb_, *ecb_);
    fflush(f);
  }

private:
  // prohibit copy/assign; do not implement
  ECB_DIRECT_POST(const ECB_DIRECT_POST&);
  ECB_DIRECT_POST& operator=(const ECB_DIRECT_POST&);
  uint32_t* ecb_;
};

class ECB_DIRECT_WAIT{
public:
  ECB_DIRECT_WAIT()
    : ecb_((uint32_t*) getmain31(8,0))
  {
    *ecb_ = 0;
  }
  ~ECB_DIRECT_WAIT()
  {
    freemain31(ecb_, 8, 0);
    ecb_ = 0;
  }

  inline
  void pause()
  {
    mvs_direct_wait(ecb_);
  }

  inline
  void release()
  {
    mvs_post(ecb_);
  }

  void printInfo(FILE* f, const char* label)
  {
    if (label){
      fprintf(f, "%s: ", label);
    }
    ;
    fprintf(f,
            "ecb address:  0x%016" PRIx64 "\n"
            "ecb value:    0x%" PRIx32 "\n",
            (uint64_t) ecb_, *ecb_);
    fflush(f);
  }

private:
  // prohibit copy/assign; do not implement
  ECB_DIRECT_WAIT(const ECB_DIRECT_WAIT&);
  ECB_DIRECT_WAIT& operator=(const ECB_DIRECT_WAIT&);
  uint32_t* ecb_;
};

class ECB_DIRECT_WAIT_POST{
public:
  ECB_DIRECT_WAIT_POST()
    : ecb_((uint32_t*) getmain31(8,0))
  {
    *ecb_ = 0;
  }
  ~ECB_DIRECT_WAIT_POST()
  {
    freemain31(ecb_, 8, 0);
    ecb_ = 0;
  }

  inline
  void pause()
  {
    mvs_direct_wait(ecb_);
  }

  inline
  void release()
  {
    mvs_direct_post(ecb_);
  }

  void printInfo(FILE* f, const char* label)
  {
    if (label){
      fprintf(f, "%s: ", label);
    }
    ;
    fprintf(f,
            "ecb address:  0x%016" PRIx64 "\n"
            "ecb value:    0x%" PRIx32 "\n",
            (uint64_t) ecb_, *ecb_);
    fflush(f);
  }

private:
  // prohibit copy/assign; do not implement
  ECB_DIRECT_WAIT_POST(const ECB_DIRECT_WAIT_POST&);
  ECB_DIRECT_WAIT_POST& operator=(const ECB_DIRECT_WAIT_POST&);
  uint32_t* ecb_;
};

// Use transferAndPause

template <typename SYNC>
void* transferAndPauseThread(void* arg)
{
  PingPongParm<SYNC>* params = (PingPongParm<SYNC>*) arg;
  const char* releaseCode = params->ping_.pause();
  while (releaseCode[0] != 'X'){
    params->ping_.transferAndPause(params->pong_);
  }
  return (void*) 0;
}

template <typename SYNC>
void transferAndPause(uint64_t n, bool verbose, const char* label)
{
  PingPongParm<SYNC> params(n);
  Timer t;
  pthread_t tid;
  int rc;
  if (0 != (rc = pthread_create(&tid, 0, &transferAndPauseThread<SYNC>, &params))) {
    fprintf(stderr, "pthread_create failed for transferAndPause<%s> thread: %d\n", label, rc);
    exit(1);
  }
  if (verbose){
    sleep(1);
    params.ping_.printInfo(stdout, "ping");
    params.pong_.printInfo(stdout, "pong");
  }
  
  t.start();
  for (uint64_t i = 0; i < n; ++i){
    params.pong_.transferAndPause(params.ping_);
  }
  params.ping_.release("X");

  t.stop();
  printf("%s transferAndPause test, %" PRIu64 " iterations\n", label, n);
  printf("user: %5.2f, system: %5.2f, total: %5.2f, elapsed: %5.2f\n",
         t.user(), t.system(), t.total(), t.wall());
  double cpuPer = t.total()/((double) n);
  double elapsedPer = t.wall()/((double) n);
  printf("CPU time per call:     %12.10f\n"
         "elapsed time per call: %12.10f\n",
         cpuPer, elapsedPer);
  fflush(stdout);
  if (verbose){
    params.ping_.printInfo(stdout, "ping");
    params.pong_.printInfo(stdout, "pong");
  }
  void* threadResult = 0;
  if (0 != (rc = pthread_join(tid, &threadResult))){
    fprintf(stderr, "pthread_join failed for transferAndPause<%s> thread: %d\n", label, rc);
    exit(1);
  }
}

// Use transfer

template <typename SYNC>
void* transferThread(void* arg)
{
  try{
    PingPongParm<SYNC>* params = (PingPongParm<SYNC>*) arg;
    const char* releaseCode = params->ping_.pause();
    while (releaseCode[0] != 'X'){
      params->pong_.transferTo();
      releaseCode = params->ping_.pause();
    }
  } catch(std::exception &e){
      fprintf(stderr, "exception in transfer thread: %s\n", e.what());
      fflush(stderr);
  }
    return (void*) 0;
}

template <typename SYNC>
void transfer(uint64_t n, bool verbose, const char* label)
{
  PingPongParm<SYNC> params(n);
  Timer t;
  pthread_t tid;
  int rc;
  if (0 != (rc = pthread_create(&tid, 0, &transferThread<SYNC>, &params))) {
    fprintf(stderr, "pthread_create failed for transfer<%s> thread: %d\n", label, rc);
    exit(1);
  }
  if (verbose){
    sleep(1);
    params.ping_.printInfo(stdout, "ping");
    params.pong_.printInfo(stdout, "pong");
  }
  
  t.start();
  for (uint64_t i = 0; i < n; ++i){
    params.ping_.transferTo();
    params.pong_.pause();
  }
  params.ping_.transferTo("X");

  t.stop();
  printf("%s transfer test, %" PRIu64 " iterations\n", label, n);
  printf("user: %5.2f, system: %5.2f, total: %5.2f, elapsed: %5.2f\n",
         t.user(), t.system(), t.total(), t.wall());
  double cpuPer = t.total()/((double) n);
  double elapsedPer = t.wall()/((double) n);
  printf("CPU time per call:     %12.10f\n"
         "elapsed time per call: %12.10f\n",
         cpuPer, elapsedPer);
  fflush(stdout);
  if (verbose){
    params.ping_.printInfo(stdout, "ping");
    params.pong_.printInfo(stdout, "pong");
  }
  void* threadResult = 0;
  if (0 != (rc = pthread_join(tid, &threadResult))){
    fprintf(stderr, "pthread_join failed for transfer<%s> thread: %d\n", label, rc);
    exit(1);
  }
  if (verbose){
    params.ping_.printInfo(stdout, "ping");
    params.pong_.printInfo(stdout, "pong");
  }

}

int main(int argc, char* const argv[])
{
  int myoptind = 1;
  char* myoptarg = 0;
  char opt = 0;
  bool verbose = false;
  uint64_t N = 1000000;

  while (((char) -1) != (opt = (char) getopt(argc, argv, "vn:"))){
    myoptind = optind;
    myoptarg = optarg;

    switch(opt){

    case 'v':
      verbose = true;
      break;

    case 'n':
      processNumericArg(myoptarg, &N, "number of iterations");
      break;

    default:
      fprintf(stderr, "unrecognized option\n");
      exit(1);
    }
  }

  if (myoptind < argc){
    fprintf(stderr, "unused arguments\n");
    exit(1);
  }

  pingPong<ECB>(N, verbose, "EventControlBlock");
  pingPong<ECB_DIRECT_POST>(N, verbose, "EventControlBlock_Direct_Post");
  pingPong<ECB_DIRECT_WAIT>(N, verbose, "EventControlBlock_Direct_Wait");
  pingPong<ECB_DIRECT_WAIT_POST>(N, verbose, "EventControlBlock_Direct_Wait_Post");
  pingPong<PauseElement>(N, verbose, "PauseElement");
  transferAndPause<PauseElement>(N, verbose, "PauseElement");
  transfer<PauseElement>(N, verbose, "PauseElement");

  return 0;
}
