#pragma once

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
) 



#define Base_code                  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2A2, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CR3_code                   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2A3, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define Read_code                  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2A4, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define Write_code                 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2A5, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define Pattern_code               CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2A6, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define Mouse_code                 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x3A1, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
