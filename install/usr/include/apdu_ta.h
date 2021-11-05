/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (C) 2021, Foundries Limited
 */

#ifndef APDU_TA_H
#define APDU_TA_H

#define PTA_APDU_UUID { 0x3f3eb880, 0x3639, 0x11ec, \
			{ 0x9b, 0x9d, 0x0f, 0x3f, 0xc9, 0x46, 0x8f, 0x50 } }
/*
 * ADPU based communication with the SE05X secure element
 *
 * [in]  value[0].a           apdu request type.
 * [in]  memref[1].buffer     apdu header.
 * [in]  memref[1].size       apdu header len.
 * [in]  memref[2].buffer     source data (raw apdu request frame data).
 * [in]  memref[2].size       source len (raw apdu source frame length).
 * [out] memref[3].buffer     response data (raw apdu response frame data).
 * [out] memref[3].size       response len (raw apdu respose length).
 *
 * Result:
 * TEE_SUCCESS - Invoke command success
 * TEE_ERROR_BAD_PARAMETERS - Incorrect input param
 * TEE_ERROR_NOT_IMPLEMENTED - Invoke command not implemented
 * TEE_ERROR_GENERIC - Invoke command failure
 */
#define PTA_CMD_SEND_APDU_RAW_FRAME		0

#endif /* APDU_TA_H */
