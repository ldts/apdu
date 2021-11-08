/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021, Foundries.io
 * Jorge Ramirez-Ortiz <jorge@foundries.io>
 */

#ifndef SE_H
#define SE_H

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Device specific  requestflags to help the driver process the frames.
 *
 * Can be used to indicate if the APDU frame is of an extended type or if the
 * response frame to a particular request should be dropped.
 */
enum se_apdu_type {
	SE_APDU_NONE,
	SE_APDU_TYPE_0,
	SE_APDU_TYPE_1,
	SE_APDU_TYPE_2,
	SE_APDU_TYPE_3,
	SE_APDU_TYPE_4,
	SE_APDU_TYPE_5,
	SE_APDU_TYPE_6,
};

/**
 * C_SE_apdu_request() - Send an APDU message and get response
 *
 * @param type		Type of the APDU command
 * @param hdr		Pointer to APDU message header
 * @param hdr_len	Byte length of message header @hdr
 * @param src		Pointer to APDU message payload
 * @param src_len	Byte length of message payload @src
 * @param dst		Pointer to APDU message reponse buffer
 * @param dst_len	Byte length of reponse buffer @dst
 *
 * @return -EINVAL	Invalid arguments regarding an APDU message
 * @return -ENODEV	OP-TEE device not available
 * @return -EPERM	Access permission failure
 * @return -EAGAIN	Initialization mutex not available
 * @return -EACCESS	Secure element processing error
 */

int C_SE_apdu_request(enum se_apdu_type type,
		      unsigned char *hdr, size_t hdr_len,
		      unsigned char *src, size_t src_len,
		      unsigned char *dst, size_t *dst_len);

/**
 * C_SE_scp03_enable()  - Enable the SCP03 protocol
 *
 * Configures the Secure Element to enable the SCP03 protocol using the keys
 * stored in its non volatile memory
 *
 * @return -EINVAL	On error
 */
int C_SE_scp03_enable(void);

/**
 * C_SE_scp03_rotate_keys()  - Rotate the default SCP03 keys
 *
 * Generates the board specific SCP03 keys and writes them in the Secure Element
 * non volatile memory.
 *
 * @return -EINVAL	On error
 */
int C_SE_scp03_rotate_keys(void);

#ifdef __cplusplus
}
#endif

#endif /*SE_H*/
