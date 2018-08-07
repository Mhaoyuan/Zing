#include <iostream>
#include "util.h"

Z_conf_t conf;

int main() {
    read_conf("Zconf.conf",&conf);
    return 0;
}