#include <iostream>
#include <iterator> 
#include <map> 
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"

int stringToUnsignedByte(char *s) {
    
    char* p;
    int base = 10;

    if (NULL == s || *s == '-')
    {
        return -1;
    }

    if (strlen(s) > 2 && memcmp(s, "0x", strlen("0x")) == 0)
    {
        s += strlen("0x");
        base = 16;
    }

    uint32_t n = strtol(s, &p, base);
    if (s == p) {
        return -1;
    }

    return ((n > 255 || n < 0) ? -1 : n);

}

bool addSettingRowToMap(char* line, map<int, settingData> *settingMap) {
    union {
        struct {
            uint8_t keyByte[sizeof(uint32_t)];
        };
        uint32_t key;
    } key_t;

    settingData setting;

    //basic safety check
    if (NULL == settingMap || NULL == line) {
        return false;
    }

    line = strtok(line, ",\n");

    for (int i = 0; i < sizeof(key_t.key); i++) {
        line = strtok(line, "/");
        key_t.keyByte[sizeof(key_t.key) - i - 1] = stringToUnsignedByte(line);
        if (key_t.keyByte[sizeof(key_t.key) - i - 1] == -1) {
            return false;
        }
        line += strlen(line) + 1;
    }

    //printf("key value id %d\n", key_t.key);

    for (int i = 0; i < NUM_OF_SETTINGS; i++) {
        line = strtok(line, ",\n");
        if (NULL == line) {
            return false;
        }
        setting.data[i] = stringToUnsignedByte(line);
        if (setting.data[i] == -1) {
            return false;
        }
        line += strlen(line) + 1;
        
        //printf("Setting %d value is %d\n", i, setting.data[i]);
    }

    settingMap->insert(pair<int, settingData>(key_t.key, setting));

    return true;
}

bool createDataLineFromRow(char* line, settingRow* row)
{
    int i = NUM_OF_SETTINGS - 1;

    if (NULL == line || NULL == row) {
        return false;
    }
    line[0] = 0;
    sprintf(line, "\t{ 0x%x, {%d, %d, %d, %d, %d, %d }},\n", row->key, (int)row->setting.data[i--], (int)row->setting.data[i--],
    (int)row->setting.data[i--], (int)row->setting.data[i--], (int)row->setting.data[i--], (int)row->setting.data[i--]);

    return true;
}

#define C_FILE_TOP_STR      "#include \"settings.h\"\n\n" \
                            "const settingRow settingRomData[] = {\n"

#define C_FILE_BOTTOM_STR   "};\n\nint getSettingRomDataLen() \n{" \
                            "\n\t return sizeof(settingRomData);\n}\n"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("ERR: csv file name should be provide as an argument.");
        return -1;
    }

    FILE* csvfile = fopen(argv[1], "r");
    if (csvfile == NULL) {
        printf("Error openning csv file");
        fclose(csvfile);
        return 1;
    }
    //use map for future saving C compiler file data sorted by key
    map<int, settingData> settingMap;
    char line[1024];
    while (fgets(line, sizeof(line), csvfile))
    {
        if (!addSettingRowToMap(line, &settingMap))
        {
            printf("Error in csv row, skipping line!!!\n");
        }
    }

    fclose(csvfile);

    FILE* cfile = fopen("settings.cpp", "w");
    if (cfile == NULL) {
        printf("Error openning settings.cpp file");
        //fclose(cfile);
        return 1;
    }

    fwrite(C_FILE_TOP_STR, 1, strlen(C_FILE_TOP_STR), cfile);

    map<int, settingData>::iterator it = settingMap.begin();
    //while (it != settingMap.end())
    //for(auto it : settingMap) 
    {
        settingRow row;
      //  row.key = it.first;
      //  row.setting = it.second;
        row.key = it->first;
        row.setting = it->second;
        createDataLineFromRow(line, &row);
        fwrite(line, 1, strlen(line), cfile);
        // Increment the Iterator to point to next entry
        //it++;
    }

    fwrite(C_FILE_BOTTOM_STR, 1, strlen(C_FILE_BOTTOM_STR), cfile);

    fclose(cfile);
    printf("'settings.cpp' file successfully generated!\n");
    return 0;
}
