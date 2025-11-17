#pragma once

#include <stdint.h>

#define FOOTER_OFFSET (-0x938)

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
    // There is more to the footer that remains to be understood
} __attribute__((packed))
bl1_footer_t;
