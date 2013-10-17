#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

/* Because pause() */
void unfreeze (int unused)
{
    return;
}

int main (int argc, char **argv)
{
    char **new_argv = NULL;
    struct sigaction sig;
    memset (&sig, 0, sizeof(sig));

    int fd = open ("/proc/systemtap/attach_gdb/wrapper_path", O_WRONLY);
    if ( fd == -1 ) {
        perror ("Open file");
        return EXIT_FAILURE;
    }
    write (fd, argv[0], strlen(argv[0]));
    close (fd);

    sig.sa_handler = &unfreeze;
    sigaction (SIGUSR1, &sig, NULL);

    pause ();
    
    new_argv = argv;
    ++new_argv;
    execvp (argv[1], new_argv);
    
    return EXIT_FAILURE;
}
