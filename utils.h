#include "vector.h"

typedef struct _meta_t meta_t;

struct _meta_t {
        char* command;
        double running_time;
        double arrival_time;
};

double getTime();
void *meta_ctor(void *elem);
void meta_dtor(void *elem);
int comparer_sjf(const void *a, const void *b);
char **strsplit(const char *str, const char *delim, size_t *numtokens);
Vector* map_parser();
void Vector_to_file(Vector* map);
void example_generator(long total_seconds, long upper_limit);
