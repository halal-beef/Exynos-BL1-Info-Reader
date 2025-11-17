# Exynos BL1 Reader

Reads information from the BL1 header and footer and prints them out.

## Compilation

Nothing fancy, just do

```gcc bl1.c -lcrypto -oExynosBL1Reader```

## Usage

```./ExynosBL1Reader <BL1>```

## Example Output

```
❯ ./ExynosBL1Reader ../bl1_x1s.bin 
BL1 Header Information:
Size in Blocks: 24 (12288 bytes)
BL1 Expected Hash: 0xE5FE26D2
Actual BL1 Hash: 0xE5FE26D2 (MATCH)

BL1 Footer Information:
Target SoC ID: 0x98300000
Target SoC Main Rev [1]: 0x1
Target SoC Sub Rev [1]: 0x1
Codesigner Version: 5
AP Info: SLSI (MATCHED)
Sign Timestamp (DD/MM/YYYY HH:MM:SS): 06/11/2024 15:14:33
Rollback Counter: 22
Sign Type: ECDSA_NIST_521

[1]:
It is unknown if those parameters actually do refer to SoC revision or
something else, so take those values with a grain of salt.
```
