#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

#define PROC_PATH "/proc/systemtap/attach_gdb/wrapper_path"

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

    int fd = open (PROC_PATH, O_RDONLY);
    if (fd == -1) {
        perror ("Open proc interface");
        return EXIT_FAILURE;
    }
    
    char buf[1024] = { 0 };
    if (read (fd, buf, sizeof (buf) - 1) == -1) {
        perror ("Read from proc interface");
        close (fd);
        return EXIT_FAILURE;
    }

    if (strncmp (buf, argv[0], strlen (argv[0]) != 0)) {
        close (fd);
        int fd = open (PROC_PATH, O_WRONLY);
        if (fd == -1) {
            perror ("Open proc interface");
            return EXIT_FAILURE;
        }
        write (fd, argv[0], strlen(argv[0]));
        close (fd);
        execve (argv[0], argv, NULL);
        return EXIT_FAILURE;
    }
    close (fd);

    sig.sa_handler = &unfreeze;
    sigaction (SIGUSR1, &sig, NULL);

    pause ();
    
    new_argv = argv;
    ++new_argv;
    execvp (argv[1], new_argv);
    
    return EXIT_FAILURE;
}
