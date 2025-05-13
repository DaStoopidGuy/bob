#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#define BOB_STRIP_PREFIXES
#define BOB_IMPL
#include "bob.h"

#define BUILD_DIR "build"
#define TARGET BUILD_DIR"/main"

void make_directory(const char *dirname) {
    if (check_dir_exists(dirname)) return;

    Cmd cmd = {0};
    cmd_append(&cmd, "mkdir", "-p", dirname);
    // cmd_run_and_reset_silent(&cmd);
    cmd_run_and_reset(&cmd);
    cmd_destroy(&cmd);
}

void build_target(Cmd *cmd) {
    printf("Build Target...\n");

    make_directory(BUILD_DIR);

    cmd_append(cmd, "gcc", "src/main.c", "-o", TARGET);
    cmd_run_and_reset(cmd);

}

void run_target(Cmd *cmd) {
    printf("Run Target...\n");
    cmd_append(cmd, "./"TARGET);
    cmd_run_and_reset(cmd);
}

void clean(Cmd *cmd) {
    printf("Clean...\n");
    cmd_append(cmd, "rm", "-rf", BUILD_DIR);
    cmd_run_and_reset(cmd);
}

int main(int argc, char **argv) {
    rebuild_yourself(argc, argv);

    Cmd cmd = {0};
    if (argc > 1) {
        const char *arg = argv[1];

        if (strcmp(arg, "run") == 0) {
            build_target(&cmd);
            run_target(&cmd);
        }
        else if (strcmp(arg, "clean") == 0) {
            clean(&cmd);
        }
        else
            printf("Unknown command: %s\n", arg);
    }
    else 
        build_target(&cmd);

    cmd_destroy(&cmd);
    return 0;
}
