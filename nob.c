#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"
#define TARGET       "Lang"

#define SOURCES "main.c"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    Nob_Cmd cmd = {0};

    nob_cc(&cmd);
    nob_cc_flags(&cmd);
    nob_cmd_append(&cmd, "-I"SRC_FOLDER);
    nob_cc_output(&cmd, BUILD_FOLDER TARGET);
    nob_cc_inputs(&cmd, SRC_FOLDER SOURCES);
    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}
