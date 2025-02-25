#include "common.h"

inline void UIntToStr(u64 num, char *str)
{
    int i = 0;
    do
    {
        str[i++] = '0' + (num % 10); // Obtener el último dígito
        num /= 10;                   // Reducir el número
    } while (num > 0);

    str[i] = '\0'; // Terminar la cadena con un carácter nulo

    // Invertir la cadena
    for (int j = 0; j < i / 2; j++)
    {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}