// -*- C++ -*-

#include "pe.h"
#include <strings.h>
#include <inttypes.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include <exception>
#include <string>

PauseElement::Error::~Error() throw() {};

PauseElement::Error::Error(const char* what, int code)
  : code_(code)
{
  char buf[1000];
  snprintf(buf, 1000, "%s failed with return code %d (0x%08x)\n",
           what, code, code);
  what_ = buf;
}

const char* PauseElement::Error::what() const throw() {
  return what_.c_str();
}

int PauseElement::Error::code() const throw(){
  return code_;
}

std::string PauseElement::Token::text() const 
{
  char buf[1000];
  snprintf(buf, 1000, "0x%016" PRIx64 "%016" PRIx64, d_[0], d_[1]);
  return std::string(buf);
}

PauseElement::PauseElement()
{
  memset(lastReleaseCode_, 0, 3);
  int32_t returnCode = 0;
  uint32_t authLevel = IEA_UNAUTHORIZED;
  iea4ape(&returnCode, authLevel, pet_);
  if (0 != returnCode) {
    throw Error("Allocate pause element (iea4ape)", returnCode);
  }
}

PauseElement::~PauseElement() 
{
  int32_t returnCode = 0;
  uint32_t authLevel = IEA_UNAUTHORIZED;
  iea4dpe(&returnCode, authLevel, pet_);
  if (0 != returnCode) {
    // We can't throw out of a dtor (well, we CAN, but it's bad form).
    // So...
    std::string ts = pet_.text();
    fprintf(stderr,
            "~PauseElement() failed for token %s with return code %" PRId32 " (0x%08" PRIx32 ")\n",
            ts.c_str(), returnCode, returnCode);
  }
}

void PauseElement::PauseElementInfo::print(FILE* f)
{
  std::string ts = token_.text();
  std::string rc(releaseCode_,3);
  fprintf(f,
          "return code:  %d (0x%08x)\n"
          "creator auth: %d (0x%08x)\n"
          "token:        %s\n"
          "owner:        %016" PRIx64 "\n"
          "state:        %d (0x%08x) %s\n"
          "release code: 0x%02x%02x%02x (\"%s\")\n",

          returnCode_, returnCode_,
          creatorAuthLevel_, creatorAuthLevel_,
          ts.c_str(),
          owner_,
          state_, state_, state(),
          releaseCode_[0], releaseCode_[1], releaseCode_[2],
          rc.c_str());
  fflush(f);
};

const char* PauseElement::PauseElementInfo::state() 
{
  switch(state_){
  case 1: return "IEA4_PET_PRERELEASED";
  case 2: return "IEA4_PET_RESET";
  case 64: return "IEA4_PET_RELEASED";
  case 128: return "IEA4_PET_PAUSED";
  default: break; 
  }
  return "UNKNOWN";
}

PauseElement::PauseElementInfo PauseElement::info() const
{
  PauseElement::PauseElementInfo result;
  uint32_t authLevel = IEA_UNAUTHORIZED;
  result.token_ = pet_;
  iea4rpi(&result.returnCode_,
          authLevel,
          result.token_,
          &result.creatorAuthLevel_,
          (char*) &result.owner_,
          &result.state_,
          result.releaseCode_);
  return result;
}

void PauseElement::printInfo(FILE* f, const char* label) const
{
  PauseElementInfo i = info();
  if (label){
    fprintf(f, "%s: ", label);
  }
  i.print(f);
}
