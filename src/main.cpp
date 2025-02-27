#include <csignal>
#include "test.h"

bool app_running = true;

static void SigIntHandler(int sig_num)
{
    signal(SIGINT, SigIntHandler);
    app_running = false;
}

int main(int argc, char **argv)
{
    signal(SIGINT, SigIntHandler);

    Test(argc, argv);
    return 0;
}
