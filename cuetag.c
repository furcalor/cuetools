#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#define strcasecmp _stricmp
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

static void tag_flac(const char *filename, const char *tag, const char *value)
{
    char cmd[4096];

    snprintf(cmd, sizeof(cmd),
             "metaflac --remove-tag=\"%s\" --set-tag=\"%s=%s\" \"%s\"",
             tag, tag, value, filename);

    system(cmd);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char line[1024];

    char album_artist[256] = "";
    char album_title[256] = "";

    char track_title[256] = "";
    char track_artist[256] = "";

    int track = 0;
    int file_index = 2;

    if (argc < 3) {
        fprintf(stderr, "Usage: cuetag <file.cue> <track01.flac> [track02.flac ...]\n");
        return 1;
    }

    fp = fopen(argv[1], "r");
    if (!fp) {
        perror("cuetag");
        return 1;
    }

    while (fgets(line, sizeof(line), fp)) {
        char *p = trim(line);

        if (strncasecmp(p, "PERFORMER", 9) == 0 && track == 0) {
            sscanf(p, "PERFORMER \"%255[^\"]\"", album_artist);
        }
        else if (strncasecmp(p, "TITLE", 5) == 0 && track == 0) {
            sscanf(p, "TITLE \"%255[^\"]\"", album_title);
        }
        else if (strncasecmp(p, "TRACK", 5) == 0) {
            track++;
            track_title[0] = 0;
            track_artist[0] = 0;
        }
        else if (strncasecmp(p, "TITLE", 5) == 0 && track > 0) {
            sscanf(p, "TITLE \"%255[^\"]\"", track_title);
        }
        else if (strncasecmp(p, "PERFORMER", 9) == 0 && track > 0) {
            sscanf(p, "PERFORMER \"%255[^\"]\"", track_artist);
        }
        else if (strncasecmp(p, "INDEX 01", 8) == 0 && track > 0) {
            if (file_index >= argc) {
                fprintf(stderr, "cuetag: not enough FLAC files for tracks\n");
                fclose(fp);
                return 1;
            }

            const char *filename = argv[file_index++];

            const char *artist = track_artist[0] ? track_artist : album_artist;
            const char *title  = track_title[0]  ? track_title  : "Unknown";

            tag_flac(filename, "ARTIST", artist);
            tag_flac(filename, "TITLE",  title);
            tag_flac(filename, "ALBUM",  album_title);
            tag_flac(filename, "TRACKNUMBER", track_title);
        }
    }

    fclose(fp);
    return 0;
}
