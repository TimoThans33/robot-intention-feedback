#include "galvo_modern.h"
#include "Arduino.h"

int getPattern(char character, int* pattern, int mirror)
{
    for (int i = 0; i < N_CHARACTERS; i++)
    {
        if (characterSet[i][0] == character)
        {
            // This is the one, copy pattern and get the length
            int pattern_n = (int)characterSet[i][1];
            for (int j = 0; j <(pattern_n * 2 + 1); j++)
            {
                pattern[j] = characterSet[i][j + 2];
                if (j % 2 == 0)
                {
                  pattern[j] = pattern[j] * mirror;
                }
            }
            
            return pattern_n;
        }
    }

    return -1;
}
