#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>

#include "apdu.h"

static int tlvGet_u8buf(uint32_t tag, size_t *index,
			uint8_t *buf, size_t len,
			uint8_t *rsp, size_t *olen);

enum se05x_tag {
	SE05x_TAG_NA = 0,
	SE05x_TAG_SESSION_ID = 0x10,
	SE05x_TAG_POLICY = 0x11,
	SE05x_TAG_MAX_ATTEMPTS = 0x12,
	SE05x_TAG_IMPORT_AUTH_DATA = 0x13,
	SE05x_TAG_IMPORT_AUTH_KEY_ID = 0x14,
	SE05x_TAG_POLICY_CHECK = 0x15,
	SE05x_TAG_1 = 0x41,
	SE05x_TAG_2 = 0x42,
	SE05x_TAG_3 = 0x43,
	SE05x_TAG_4 = 0x44,
	SE05x_TAG_5 = 0x45,
	SE05x_TAG_6 = 0x46,
	SE05x_TAG_7 = 0x47,
	SE05x_TAG_8 = 0x48,
	SE05x_TAG_9 = 0x49,
	SE05x_TAG_10 = 0x4A,
	SE05x_TAG_11 = 0x4B,
	SE05x_GP_TAG_CONTRL_REF_PARM = 0xA6,
	SE05x_GP_TAG_AID = 0x4F,
	SE05x_GP_TAG_KEY_TYPE = 0x80,
	SE05x_GP_TAG_KEY_LEN = 0x81,
	SE05x_GP_TAG_GET_DATA = 0x83,
	SE05x_GP_TAG_DR_SE = 0x85,
	SE05x_GP_TAG_RECEIPT = 0x86,
	SE05x_GP_TAG_SCP_PARMS = 0x90,
};

enum se05x_status {
	SE05x_NOT_OK = 0xFFFF,
	SE05x_OK = 0x9000,
};

uint8_t SE05X_CMD_VERSION[] = {
	SE05x_TAG_1, 0x04, 0x7F, 0xFF, 0x02, 0x06, 0x43, 0x02, 0x00, 0x12,
};

uint8_t SE05X_CMD_HEADER[4] = {
	0x80, 0x02, 0x00, 0x00,
};

static void se05x_get_uuid(uint8_t *buf, size_t len)
{
	enum se05x_status ret = SE05x_NOT_OK;
	uint8_t uuid[18];
	size_t uuid_len = sizeof(uuid);
	size_t i = 0;

	if (tlvGet_u8buf(SE05x_TAG_1, &i, buf, len, uuid, &uuid_len)) {
		printf("apdu decode failed\n");
		return;
	}

	if (i + 2 != len) {
		printf("invalid data from the secure element\n");
		return;
	}

	ret = (buf[i] << 8) | (buf[i + 1]);
	if (ret != SE05x_OK) {
		printf("invalid response from the secure element\n");
		return;
	}

	printf("Secure Element Version: ");
	for (i = 0; i < uuid_len; i++)
		printf("%02X. ", uuid[i]);
	printf("\n");

	return;
}

#define CMD_VERSION SE05X_CMD_VERSION
#define CMD_HEADER SE05X_CMD_HEADER

/* ISO 7816-4 Annex D */
int tlvGet_u8buf(uint32_t tag, size_t *index,
		 uint8_t *buf, size_t len,
		 uint8_t *rsp, size_t *olen)
{
	size_t extended_len = 0;
	size_t rsp_len = 0;
	uint8_t *p = NULL;
	int ret = 1;

	if (!rsp || !olen || !index || *index > len)
		return -EINVAL;

	p = buf + *index;

	if (*p++ != tag)
		return -EINVAL;

	rsp_len = *p++;

	switch (rsp_len) {
	case 0x00 ... 0x7F:
		extended_len = rsp_len;
		*index += 2;
		break;
	case 0x81:
		extended_len = *p++;
		*index += 3;
		break;
	case 0x82:
		extended_len = *p++;
		extended_len = (extended_len << 8) | *p++;
		*index += 4;
		break;
	default:
		return -EINVAL;
	}

	if (extended_len > *olen)
		return -EINVAL;

	if (extended_len > len)
		return -EINVAL;

	*olen = extended_len;
	*index += extended_len;

	while (extended_len-- > 0)
		*rsp++ = *p++;

	return 0;
}

static int error;

static void handle(int signal_number)
{
	/* issued by libapduteec constructor */
	if (signal_number == SIGABRT)
		error = 1;
}

__attribute__((constructor(101)))static void init(void)
{
	signal(SIGABRT, handle);
}

int main()
{
	size_t ilen = sizeof(CMD_VERSION);
	size_t hlen = sizeof(CMD_HEADER);
	uint8_t out[892];
	size_t olen = sizeof(out);

	if (error) {
		printf("error, cant access SE050\n");
		return -ENODEV;
	}

	if (C_APDU_request(APDU_TYPE_4, CMD_HEADER, hlen, CMD_VERSION, ilen,
			   out, &olen)) {
		printf("error, cant communicate with TEE core\n");
		return -EINVAL;
	}

	if (!olen) {
		printf("error, failure processing the APDU request\n");
		return -EINVAL;
	}

	se05x_get_uuid(out, olen);

	return 0;
}
