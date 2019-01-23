#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <regex>
#include <string.h>
#include <string>
#include <sys/stat.h>

using namespace std;

/* auxiliary functions */

bool pattern_match(const char *str, const char *pattern) {
    string patternstr("(.*)(");
    patternstr += string(pattern) + string(")(.*)");
    regex exp(patternstr);
    string str_(str);

    if (regex_match(str_, exp))
        return true;
    return false;
}

#define PATH_APPEND(newpath, path, addition)                                   \
    {                                                                          \
        newpath = (char *)malloc(strlen(path) + strlen(addition) + 2);         \
        if (newpath == NULL) {                                                 \
            cout << "Error number = " << to_string(errno)                      \
                 << " calling malloc()";                                       \
            return -1;                                                         \
        }                                                                      \
        if (strlen(path) == 0) {                                               \
            sprintf(newpath, "%s", addition);                                  \
        } else if (path[strlen(path) - 1] == '/') {                            \
            sprintf(newpath, "%s%s", path, addition);                          \
        } else {                                                               \
            sprintf(newpath, "%s/%s", path, addition);                         \
        }                                                                      \
    }

int process_directory(const char *dirname, const char *pattern) {
    DIR *dp;
    struct dirent *dirp;
    struct stat st;
    char *entryname;

    if ((dp = opendir(dirname)) == NULL) {
        cout << "Error number = " << to_string(errno) << " opening " << dirname
             << endl;
        return -1;
    }
    while ((dirp = readdir(dp)) != NULL) {
        PATH_APPEND(entryname, dirname, dirp->d_name);

        if (pattern_match(dirp->d_name, pattern))
            cout << entryname << endl;

        stat(entryname, &st);
        if (st.st_mode & S_IFDIR) {
            if (strcmp(".", dirp->d_name) != 0 &&
                strcmp("..", dirp->d_name) != 0) {
                if (process_directory(entryname, pattern) < 0)
                    return -1;
            }
        }
        free(entryname);
    }
    return 0;
}

/*
 * MAIN
 */
int main(int argc, char *argv[]) {
    /* reading params */
    if (argc < 3) {
        cout << "USE " << string(argv[0]) << " root-dir pattern";
        cout << endl << flush;
        return 1;
    }
    return process_directory(argv[1], argv[2]);
}
