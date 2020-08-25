#ifndef PARSER_H
#define PARSER_H

#include <string.h> // memset memcpy
#include <stdlib.h> // free exit

#include <regex.h>
#include <limits.h>

#include "utils.h"
#include "que.h"
#include "bloom_filter.h"

int doc_parse(char *document, que *q, bloom_filter *bf);

#endif
