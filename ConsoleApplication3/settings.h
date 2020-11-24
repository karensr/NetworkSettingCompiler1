#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <stdint.h>

typedef struct settingData settingData;
typedef struct settingRow settingRow;

extern const settingRow settingRomData[];

#define NUM_OF_SETTINGS 6

#pragma pack(1)
struct settingData {
    uint8_t data[NUM_OF_SETTINGS];
};

#pragma pack(1)
struct settingRow {
    union {
        uint32_t key;
        struct {
            uint8_t cpuId;
            uint8_t channelId;
            uint8_t speedId;
            uint8_t profileId;
        };
        struct {
            uint8_t keyByte[sizeof(uint32_t)];
        };
    };
    settingData setting;
};

#endif // __SETTINGS_H__
