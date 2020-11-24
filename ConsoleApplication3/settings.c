#include "settings.h"

const settingRow settingRomData[] = {
	{ 0x1010101, {144, 170, 68, 22, 19, 55 }},
	{ 0x1050001, {121, 51, 68, 232, 9, 1 }},
	{ 0x1050002, {122, 34, 68, 232, 9, 2 }},
	{ 0x1050101, {124, 53, 68, 232, 49, 0 }},
	{ 0x2040001, {255, 54, 68, 232, 123, 9 }},
	{ 0x3030303, {126, 55, 68, 23, 43, 8 }},
	{ 0x4050002, {132, 255, 68, 50, 89, 98 }},
	{ 0x6060606, {129, 85, 68, 232, 89, 86 }},
	{ 0x7070707, {128, 57, 68, 232, 89, 77 }},
	{ 0x8080808, {127, 56, 44, 32, 89, 7 }},
};

int getSettingRomDataLen() 
{
	 return sizeof(settingRomData);
}