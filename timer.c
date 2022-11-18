#include <Windows.h>
#include <wchar.h>
#include <stdio.h>
#include <stdint.h>
#include <winuser.h>

int main(int argc, char** argv) {
    HANDLE stdoutConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (stdoutConsole == INVALID_HANDLE_VALUE) {
        exit(GetLastError());
    }

    uint64_t numberOfSeconds = 0; /*  Will be given from parsing */
    char timeToParse[45] = {0};

    if (argc < 2) {
        /*  Double-click from file explorer */
        printf("How long should this timer go for (NhNmNs format, with\
all being optional e.g. 20s or 30m40s)? ");
        fgets(timeToParse, 45, stdin);
    }
    /* If we're here probs called from cmd. NOTE: NOT A QUESTION OF TTY OR NOT
     * So we need to make argv[1] the time to parse*/
    if (argc > 1)
        strcpy_s(timeToParse, 45, argv[1]);

    /*  Parse it to grab the hours, minutes and seconds from the format
     *  of "NhNmNs", where 'N' is a user specified number */
    char numBuf[5] = {0}; /*  Holds the time as we convert it into seconds into numberOfSeconds */
                          /*  5 digits seems very reasonable */

    /*  Parse the number */
    for (size_t i = 0, numbufiIter = 0; i < strlen(timeToParse); i++) {
        switch (tolower(timeToParse[i])) {
            case 'h': /*  convert into seconds */
                numberOfSeconds += strtoll(numBuf, NULL, 10)*3600;
                /*  Clear numBuf and reset numbufiIter */
                memset(&numBuf, 0, sizeof(numBuf));
                numbufiIter = 0;
                continue;
                break;
            case 'm': /*  convert into seconds */
                numberOfSeconds += strtoll(numBuf, NULL, 10)*60;
                memset(&numBuf, 0, sizeof(numBuf));
                numbufiIter = 0;
                continue;
                break;
            case 's':
                numberOfSeconds += strtoll(numBuf, NULL, 10);
                memset(&numBuf, 0, sizeof(numBuf));
                numbufiIter = 0;
                continue;
                break;
            case '\n': /*When argc < 2 and fgets is used, newline is included*/
                break;
            default:
                if (isdigit(timeToParse[i])) /*  No alarm if it's a digit */
                    break;
                printf("ERROR: Unknown time unit used.\n");
                exit(-1);
        }
        numBuf[numbufiIter] = timeToParse[i];
        numbufiIter++;
    }
    /*  numberOfSeconds is now correct, so we handle the sleeping */

    while (numberOfSeconds > 0) { /*  While there is still time to count */
        if (numberOfSeconds > 3600) { /*  Nicely handles hours, mins and secs I believe */
            printf("%lluh:%llum:%llus left\n", numberOfSeconds / 3600,
                    numberOfSeconds / 60, numberOfSeconds % 60);
        }
        if (numberOfSeconds < 3600 && numberOfSeconds >= 60) {
            printf("%llum:%llus left\n", numberOfSeconds / 60, numberOfSeconds % 60);
        }
        if (numberOfSeconds < 60) {
            printf("%llus left\n", numberOfSeconds);
        }
        Sleep(1000); /*  Milliseconds */
        numberOfSeconds--;
    }
    /*  Finished timer, alert user */
    /*  Use console window HWND to make the SYSTEMMODAL work. */
    MessageBox(GetConsoleWindow(), "Timer finished!!", "Timer done",
        MB_OK | MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
    return 0;
}

