#define _XOPEN_SOURCE 700

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
int main(void) {
    // strcpy copies the entire source string (including '\0') into the destination.
    char src1[] = "Hello";
    char dest1[20];
    strcpy(dest1, src1);
    printf("strcpy: %s\n", dest1);

    // strncpy copies only n characters; may NOT null-terminate if src >= n.
    char dest2[10];
    strncpy(dest2, "WorldExample", 5);
    dest2[5] = '\0';  // manually terminate
    printf("strncpy: %s\n", dest2);

    // strcat appends one string to another (must have enough space!)
    char greet[20] = "Hello, ";
    strcat(greet, "Alice");
    printf("strcat: %s\n", greet);

    // strncat appends up to n characters
    char msg[30] = "Hi ";
    strncat(msg, "Universe123", 8); // appends "Universe"
    printf("strncat: %s\n", msg);

    // Returns length of string (excluding '\0')
    printf("strlen(\"Hello\"): %zu\n", strlen("Hello"));

    // Returns 0 if equal, <0 if first < second, >0 if first > second (lexicographically)
    printf("strcmp(\"abc\", \"abd\"): %d\n", strcmp("abc", "abd"));
    printf("strncmp(\"abcde\", \"abcfg\", 3): %d\n", strncmp("abcde", "abcfg", 3));

    // Find first and last occurrence of a character in a string
    const char *text = "banana";
    char *first_a = strchr(text, 'a'); // first 'a'
    char *last_a  = strrchr(text, 'a'); // last 'a'
    printf("strchr: first 'a' in '%s' at position %ld\n", text, first_a - text);
    printf("strrchr: last 'a' in '%s' at position %ld\n", text, last_a - text);

    // Finds first occurrence of a substring
    const char *haystack = "The quick brown fox";
    const char *needle = "brown";
    char *found = strstr(haystack, needle);
    if (found)
        printf("strstr: found '%s' inside '%s'\n", needle, haystack);

    // Allocates new memory and copies the string there
    char *copy = strdup("Dynamic string");
    printf("strdup: %s\n", copy);
    free(copy); // must free after strdup()

    // Splits string into tokens separated by delimiters
    char text2[] = "apple,banana,orange";
    char *token = strtok(text2, ","); // first token
    printf("strtok tokens:\n");
    while (token != NULL) {
        printf("  %s\n", token);
        token = strtok(NULL, ","); // continue with NULL
    }

    //. memset(), memcpy(), memmove(), memcmp() ---
    char buffer[20];
    memset(buffer, '*', 10);  // fill with '*'
    buffer[10] = '\0';
    printf("memset: %s\n", buffer);

    char src3[] = "12345";
    memcpy(buffer, src3, strlen(src3) + 1); // copy including '\0'
    printf("memcpy: %s\n", buffer);

    char overlap[] = "123456789";
    memmove(overlap + 2, overlap, 5); // safe for overlapping regions
    overlap[7] = '\0';
    printf("memmove: %s\n", overlap);

    printf("memcmp(\"abc\", \"abd\", 3): %d\n", memcmp("abc", "abd", 3));

    // reading from the stdin
    char * line = NULL;
    size_t size = 0;
    ssize_t read;
    printf("Please input the strings if you want to stop press enter\n");
    read = getline(&line, &size, stdin);
    printf("The STRING IS : %s truesize: %ld potentialsize : %ld", line, read, size);
    free(line);
    return 0;
}
