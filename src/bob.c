#define BOB_STRIP_PREFIXES
#define BOB_IMPL
#include "bob.h"

int main(int argc, char **argv) {
    printf("Bob says Hi!\n");
    Cmd cmd = {0};

    cmd_append(&cmd, "gcc", "src/main.c", "-o", "build/main");
    cmd_run(&cmd);

    cmd_destroy(&cmd);
    return 0;
}
