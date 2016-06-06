// -*- C++ -*-
#ifndef _PE_H_
#define _PE_H_

#include <stdint.h>
#include <ieac.h>

#include <exception>
#include <string>

class PauseElement{
public:

  class Error: public std::exception{
  public:
    
    ~Error() throw();
    Error(const char* what, int code);

    virtual 
    const char* what() const throw();

    int code() const throw();
  private:
    std::string what_;
    int code_;
  };

  PauseElement();
  ~PauseElement();

  inline
  const char* pause() 
  {
    Token updatedPET;
    int32_t returnCode = 0;
    uint32_t authLevel = IEA_UNAUTHORIZED;
    iea4pse(&returnCode, authLevel, pet_, updatedPET, lastReleaseCode_);
    if (0 != returnCode) {
      throw Error("pause (iea4pse)", returnCode);
    }
    pet_ = updatedPET;
    return lastReleaseCode_;
  }

  inline
  void release(const char* code=0) 
  {
    uint32_t zero = 0;
    if (0 == code){
      code = (const char*) &zero;
    }
    int32_t returnCode = 0;
    uint32_t authLevel = IEA_UNAUTHORIZED;
    iea4rls(&returnCode, authLevel, pet_, (char*) code);
    if (0 != returnCode) {
      throw Error("release (iea4dpe)", returnCode);
    }
  }

  inline
  void transferTo(const char* code=0)
  {
    uint32_t zero = 0;
    if (0 == code){
      code = (const char*) &zero;
    }
    Token doNotPausePET;
    Token unusedPET;
    char unusedReleaseCode[3];
    int32_t returnCode = 0;
    uint32_t authLevel = IEA_UNAUTHORIZED;
    iea4xfr(&returnCode, authLevel, doNotPausePET, unusedPET, unusedReleaseCode, pet_, (char*) code);
    if (0 != returnCode) {
      throw Error("transferTo (iea4xfr)", returnCode);
    }
  }

  inline
  const char* transferAndPause(PauseElement& otherDU, const char* code=0)
  {
    uint32_t zero = 0;
    if (0 == code){
      code = (const char*) &zero;
    }
    Token updatedPET;
    int32_t returnCode = 0;
    uint32_t authLevel = IEA_UNAUTHORIZED;
    iea4xfr(&returnCode, authLevel, pet_, updatedPET, lastReleaseCode_, otherDU.pet_, (char*) code);
    if (0 != returnCode) {
      throw Error("transferAndPause (iea4xfr)", returnCode);
    }
    pet_ = updatedPET;
    return lastReleaseCode_;
  }

  class Token {
  public:
    inline
    Token()
    {
      d_[0] = 0;
      d_[1] = 0;
    }

    inline
    Token& operator=(const Token& rhs)
    {
      if (this != &rhs){
        d_[0] = rhs.d_[0];
        d_[1] = rhs.d_[1];
      }
      return *this;
    }

    inline
    Token(const Token& rhs)
    {
      *this = rhs;
    }

    inline
    bool operator==(const Token& rhs) const
    {
      return (d_[0] == rhs.d_[0]) && (d_[1] == rhs.d_[1]);
    }

    inline
    bool operator!=(const Token& rhs) const
    {
      return !(*this == rhs);
    }

    inline
    operator const char*() const
    {
      return (const char*) d_;
    }

    inline
    operator char*() 
    {
      return (char*) d_;
    }

    std::string text() const;

  private:
    uint64_t d_[2];
  };

  inline
  const Token& token() const throw()
  {
    return pet_;
  }

  inline
  const char* lastReleaseCode() const throw()
  {
    return lastReleaseCode_;
  }

  struct PauseElementInfo {
    // default ctor/dtor/copy/assign OK
    int32_t returnCode_;
    int creatorAuthLevel_;
    Token token_;
    uint64_t owner_;
    int state_;
    char releaseCode_[3];

    void print(FILE* f);
    const char* state();
  };

  PauseElementInfo info() const;

  void printInfo(FILE* f, const char* label=0) const;

private:
  Token pet_;
  char lastReleaseCode_[3];
};

#endif // _PE_H_
