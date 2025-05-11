#define BOB_IMPL
#include "bob.h"

int main(int argc, char **argv) {
    printf("Bob says Hi!\n");
    Bob_Cmd cmd = {0};

    bob_cmd_append(&cmd, "gcc", "src/main.c", "-o", "build/main");
    bob_cmd_run(&cmd);

    bob_cmd_destroy(&cmd);
    return 0;
}
