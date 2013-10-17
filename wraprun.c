#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


void unfreeze (int unused)
{
    return;
}

int main (int argc, char **argv)
{
    char **new_argv = NULL;
    struct sigaction sig;
    memset (&sig, 0, sizeof(sig));

    sig.sa_handler = &unfreeze;
    sigaction (SIGUSR1, &sig, NULL);

    pause ();
    
    new_argv = argv;
    ++new_argv;
    execvp (argv[1], new_argv);
    
    return EXIT_FAILURE;
}
