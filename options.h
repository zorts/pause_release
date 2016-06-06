/* -*- C -*- */
#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * value -> arg value to be processed
 * target -> uint64_t to receive the result; must be pre-initialzed
 *           with the default to use if the value doesn't parse
 * what -> string that says what the value is, for use in error messages
 */

void processNumericArg(const char* value, uint64_t *target, const char* what);

/*
 * source -> string the form <FIRST>[<SEP><REMAINDER>]
 * Sets "first" to point to <FIRST>, "remainder" to point to either
 * <REMAINDER> or to the empty string.
 *
 * NOTE: "source" is modified (<SEP>, if present, is replaced by a
 * null terminator).
 */

void split(char* source, char where, char** first, char** remainder);

#ifdef __cplusplus
}
#endif

#endif /* OPTIONS_H_ */
