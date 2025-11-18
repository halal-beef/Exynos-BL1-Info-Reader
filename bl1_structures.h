#pragma once

#include <stdint.h>

#define FOOTER_OFFSET (-0x938)

typedef struct ecdsa_pubkey
{
    uint8_t x[68];
    uint8_t y[68];
    uint8_t pad[388];
} __attribute__((packed))
ecdsa_pubkey_t;

typedef enum sign_type
{
    RSASSA_PSS_2048 = 0,
    RSASSA_PSS_3072,
    RSASSA_PSS_4096,
    ECDSA_NIST_256,
    ECDSA_NIST_521,
    ECDSA_BRAINPOOLTWISTED_256,
    ECDSA_BRAINPOOLTWISTED_384,
    ECDSA_BRAINPOOLTWISTED_512
} sign_type_t;

/* Kinda dirty but meh */
static const char *sign_type_str[] = {
    "RSASSA_PSS_2048",
    "RSASSA_PSS_3072",
    "RSASSA_PSS_4096",
    "ECDSA_NIST_256",
    "ECDSA_NIST_521",
    "ECDSA_BRAINPOOLTWISTED_256",
    "ECDSA_BRAINPOOLTWISTED_384",
    "ECDSA_BRAINPOOLTWISTED_512"
};

typedef struct bl1_header
{
    uint32_t bl1_blk_size;
    uint32_t bl1_hash; // Only the (Last, on device, first on normal PCs) part of the SHA is used to verify the hash

    uint64_t reserved; // Unused, BL1 code starts at offset 0x10
} __attribute__((packed))
bl1_header_t;

typedef struct bl1_footer
{
    uint32_t machine_id; // Suspected this is SoC_ID + Revisions (MAIN/SUB)

    uint32_t unk_1;

    uint32_t codesigner_version;
    char ap_info[4]; // Always "SLSI"
    uint64_t timestamp;
    uint32_t rollback_count;
    sign_type_t sign_type;
    char build_desc[36]; // Unused, though is apparently a thing according to CodeSigner.
    uint32_t key_index;
    uint8_t debug_certificate[48]; // Has it's own structure but doesn't seem to be present on retail images atleast.

    // Key stuff, assumes Elliptic Curve, this is 990 and higher, older devices use a different algorithm.
    ecdsa_pubkey_t stage2_pubkey_tee;
    ecdsa_pubkey_t stage2_pubkey_ree;
    char func_ptr[128]; // Seems to be unused

    uint16_t major_id;
    uint16_t sub_id;
    uint64_t reserved;
    char unk_2[20]; // All zeroed but codesigner doesn't count this as reserved.

    ecdsa_pubkey_t bl1_pubkey; // Documented as stage1_pubkey but stage1 == BL1.

    uint8_t hmac[32];

    uint32_t signature_size; // Should return 136 here as ECDSA here uses 68 bytes each for x and y, adding up to 136 bytes.
    uint8_t signature[512];
} __attribute__((packed))
bl1_footer_t;
