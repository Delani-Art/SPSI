#include "utils.h"
#include <stdio.h>

time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm t = {0};
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    return mktime(&t);
}