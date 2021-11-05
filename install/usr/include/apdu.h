/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021, Foundries.io
 */

#ifndef APDU_H
#define APDU_H

#include <unistd.h>

/* The library throws SIGABRT before the main is executed - the main program can
 * therefore decide on how best to proceed with its boot sequence.
 */

#ifdef __cplusplus
extern "C" {
#endif

enum apdu_type {
	APDU_NONE,
	APDU_TYPE_0,
	APDU_TYPE_1,
	APDU_TYPE_2,
	APDU_TYPE_3,
	APDU_TYPE_4,
	APDU_TYPE_5,
	APDU_TYPE_6,
};

int C_APDU_request(enum apdu_type type,
		   unsigned char *hdr, size_t hdr_len,
		   unsigned char *src, size_t src_len,
		   unsigned char *dts, size_t *dst_len);

#ifdef __cplusplus
}
#endif

#endif /*APDU_H*/
