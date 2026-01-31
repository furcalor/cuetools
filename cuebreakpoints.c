#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#define strncasecmp _strnicmp
#endif

static char *trim(char *s)
{
    char *end;

    while (isspace((unsigned char)*s)) s++;

    if (*s == 0)
        return s;

    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    end[1] = '\0';

    return s;
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char line[1024];

    if (argc != 2) {
        fprintf(stderr, "Usage: cuebreakpoints <file.cue>\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (!fp) {
        perror("cuebreakpoints");
        return 1;
    }

    while (fgets(line, sizeof(line), fp)) {
        char *p = trim(line);

        if (strncasecmp(p, "INDEX 01", 8) == 0) {
            int mm, ss, ff;

            if (sscanf(p, "INDEX 01 %d:%d:%d", &mm, &ss, &ff) == 3) {
                printf("%02d:%02d:%02d\n", mm, ss, ff);
            }
        }
    }

    fclose(fp);
    return 0;
}
