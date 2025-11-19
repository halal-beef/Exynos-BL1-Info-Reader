#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <openssl/sha.h>

#include "bl1_structures.h"

int main(int argc, char *argv[])
{
    bl1_header_t bl1_header;
    bl1_footer_t bl1_footer;
    uint8_t *bl1_file;

    uint32_t file_hash[SHA512_DIGEST_LENGTH / sizeof(uint32_t)];

    char target_ap_info[4] = {'S', 'L', 'S', 'I'};

    char human_date[64];

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <BL1>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Failed to open BL1");
        return 1;
    }

    size_t read_len = fread(&bl1_header, 1, sizeof(bl1_header_t), file);
    if (read_len != sizeof(bl1_header_t))
    {
        fprintf(stderr, "Unable to read header\n");
        return 1;
    }

    uint32_t bl1_sz = bl1_header.bl1_blk_size * 512;
    bl1_file = malloc(bl1_sz);

    fseek(file, 0, SEEK_SET);

    read_len = fread(bl1_file, 1, bl1_sz, file);

    if (read_len != bl1_sz)
    {
        fprintf(stderr, "Unable to read entire BL1 file\n");
        return 1;
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Unable to close file\n");
        return 1;
    }

    memcpy(&bl1_footer, bl1_file + bl1_sz + FOOTER_OFFSET, sizeof(bl1_footer_t));

    SHA512(&bl1_file[sizeof(bl1_header_t)], bl1_sz - sizeof(bl1_header_t), (uint8_t *)file_hash); // SHA256 for older devices

    free(bl1_file);

    struct tm *tm = gmtime(&bl1_footer.timestamp);
    if(!tm)
    {
        fprintf(stderr, "gmtime failed");
        return 1;
    }
 
    if (strftime(human_date, sizeof(human_date), "%d/%m/%Y %H:%M:%S", tm) == 0)
    {
        fprintf(stderr, "Failed to format time into string\n");
        return 1;
    }

    printf("BL1 Header Information:\n");
    printf("Size in Blocks: %u (%d bytes)\n", bl1_header.bl1_blk_size, bl1_sz);
    printf("BL1 Expected Hash: 0x%X\n", bl1_header.bl1_hash);

    if(bl1_header.bl1_hash == file_hash[0])
        printf("Actual BL1 Hash: 0x%X (MATCH)\n", file_hash[0]);
    else
        printf("Actual BL1 Hash: 0x%X (MISMATCHED)\n", file_hash[0]);

    printf("\n");
    printf("BL1 Footer Information:\n");
    printf("Target SoC ID: 0x%x\n", bl1_footer.machine_id & 0xFFFF0000);
    printf("Target SoC Main Rev [1]: %u\n", bl1_footer.machine_id & 0x0000FF);
    printf("Target SoC Sub Rev [1]: %u\n", bl1_footer.machine_id & 0x000000FF);
    printf("Codesigner Version: %u\n", bl1_footer.codesigner_version);

    if(!memcmp(bl1_footer.ap_info, target_ap_info, 4))
        printf("AP Info: %.4s (MATCHED)\n", bl1_footer.ap_info);
    else
        printf("AP Info: %.4s (MISMATCHED, expected SLSI)\n", bl1_footer.ap_info);

    printf("Sign Timestamp (DD/MM/YYYY HH:MM:SS): %s\n", human_date);
    printf("Rollback Counter: %u\n", bl1_footer.rollback_count);

    if (bl1_footer.sign_type > ECDSA_BRAINPOOLTWISTED_512)
        printf("Sign Type: Unknown (0x%x)\n", bl1_footer.sign_type);
    else
        printf("Sign Type: %s\n", sign_type_str[bl1_footer.sign_type]);

    printf("\n");
    printf("[1]:\n");
    printf("It is unknown if those parameters actually do refer to SoC revision or\n");
    printf("something else, so take those values with a grain of salt.\n");
}
