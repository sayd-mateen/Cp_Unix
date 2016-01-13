/*************************************************************************\
 * Name: Sayd Mateen
 * Class: CSC 60
* \*************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int
main(int argc, char *argv[])
{
    struct stat sb;
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    int fname;
    // This if statment does a check if user enter enouph arguments. 
    if (argc < 3)
        usageErr("%s old-file new-file\n", argv[0]);
    /* This if statement checks if there is more than 2 arguments which would mean the user added options arguments. If so we could have to ignore them and go to the last two arguments 
       which will be the source and destination. */  
    if (argc > 3) 
	fname = argc - 2;
    else 
	fname = 1;
    // This if statement checks if the file is trying to copy to itself.
    if (strcmp(argv[fname], argv[fname+1]) == 0) 
	usageErr("Command doesn't permit a file to copy to itself\n");
    // This if statement gets the file information while checking for any errors.
    if (stat(argv[fname+1], &sb) == -1)
	errExit("stat");
    // This if statement will check if the entered destination is a directory.
    if ((sb.st_mode & S_IFMT) == S_IFDIR)
	usageErr("Second argument can't be a directory\n");
    /* Open input and output files */

    inputFd = open(argv[fname], O_RDONLY);
    if (inputFd == -1)
        errExit("opening file %s", argv[fname]);

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    outputFd = open(argv[fname+1], openFlags, filePerms);
    if (outputFd == -1)
        errExit("opening file %s", argv[fname+1]);

    /* Transfer data until we encounter end of input or an error */

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead)
            fatal("couldn't write whole buffer");
    if (numRead == -1)
        errExit("read");

    if (close(inputFd) == -1)
        errExit("close input");
    if (close(outputFd) == -1)
        errExit("close output");

    exit(EXIT_SUCCESS);
}
