#pragma once
#include "../../Includes.h"

namespace Randomizer
{
    // Generate a random alphanumeric character
    inline char RandomChar()
    {
        const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        return charset[Utility::InitRandom() % (sizeof(charset) - 1)];
    }

    // Generate a random hex character
    inline char RandomHexChar(bool uppercase = true)
    {
        const char charset[] = "0123456789abcdef";
        const char charset_up[] = "0123456789ABCDEF";
        return uppercase ? charset_up[Utility::InitRandom() % 16] : charset[Utility::InitRandom() % 16];
    }

    // Mutate a serial by changing some characters while keeping the format
    inline void MutateSerial(char* serial, float mutationRate = 0.3f)
    {
        if (!serial) return;
        size_t len = strlen(serial);
        for (size_t i = 0; i < len; i++)
        {
            // Skip separators
            if (serial[i] == '-' || serial[i] == '_' || serial[i] == ' ' || serial[i] == '.')
                continue;

            // Decision to mutate based on rate
            if ((Utility::InitRandom() % 100) < (mutationRate * 100))
            {
                if (serial[i] >= '0' && serial[i] <= '9')
                    serial[i] = '0' + (Utility::InitRandom() % 10);
                else if (serial[i] >= 'A' && serial[i] <= 'Z')
                    serial[i] = 'A' + (Utility::InitRandom() % 26);
                else if (serial[i] >= 'a' && serial[i] <= 'z')
                    serial[i] = 'a' + (Utility::InitRandom() % 26);
            }
        }
    }

    // Generate a randomized string of a given length
    inline void GenerateRandomString(char* buffer, size_t length)
    {
        for (size_t i = 0; i < length; i++)
        {
            buffer[i] = RandomChar();
        }
        buffer[length] = '\0';
    }

    // Generate a randomized MAC address
    inline void GenerateRandomMAC(unsigned char* mac)
    {
        for (int i = 0; i < 6; i++)
        {
            mac[i] = (unsigned char)(Utility::InitRandom() & 0xFF);
        }
        mac[0] &= 0xFE;
        mac[0] |= 0x02;
    }
}
