#include <stdio.h>
    #include <stdlib.h>
    #include <sys/select.h>

    int
    main(void)
    {
        struct timeval tv;
        int retval;

        /* Watch stdin (fd 0) to see when it has input. */
        while(1){
            struct timeval tv;
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(0, &rfds);

            /* Wait up to five seconds. */

            tv.tv_sec = 5;
            tv.tv_usec = 0;

            retval = select(1, &rfds, NULL, NULL, &tv);
            /* Don't rely on the value of tv now! */

            if (retval == -1)
                perror("select()");
            else if (retval)
                printf("%d\n", retval);
                /* FD_ISSET(0, &rfds) will be true. */
            else
                printf("No data within five seconds.\n");
            
            FD_CLR(0, &rfds);
        }

        exit(EXIT_SUCCESS);
    }