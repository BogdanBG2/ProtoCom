#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char* data;
    size_t size;
} buffer;

// initializes a buffer
buffer buffer_init(void);

// destroys a buffer
void buffer_destroy(buffer*);

// adds data of size data_size to a buffer
void buffer_add(buffer*, const char*, size_t);

// checks if a buffer is empty
int buffer_is_empty(buffer*);

// finds data of size data_size in a buffer and returns its position
int buffer_find(buffer*, const char*, size_t);

// finds data of size data_size in a buffer in a
// case-insensitive fashion and returns its position
int buffer_find_insensitive(buffer*, const char*, size_t);
