#include "parser.h"

int doc_parse(char *document, que *q){

    char *pattern = "href=\"([a-zA-Z][a-zA-Z0-9+.-]*)://"
                    "([a-zA-Z0-9+.-]*sohu.com):?"
                    "([/[a-zA-Z0-9_-]+]*)?"
                    "[?]?([a-zA-Z0-9]*=[a-zA-z0-9=]*)?"
                    "[#]?([.]*)?";
    size_t maxMatches = 1 << 10;
    size_t maxGroups = 6;
    
    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];
    
    if (regcomp(&regexCompiled, pattern, REG_EXTENDED))
    {
        printf("Could not compile regular expression.\n");
        return 1;
    };
    
        
    char *cursor = document;
    int cnt = 0;
    for (size_t m = 0; m < maxMatches; m++)
    {
        if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
            break;  // No more matches
        
        url_comp *url_components;
        url_components = xmalloc(sizeof *url_components);
        bzero(url_components, sizeof(*url_components));
        
        size_t offset = 0;
        for (size_t g = 0; g < maxGroups; g++){
            if (groupArray[g].rm_so == (size_t)-1)
                break;  // No more groups
            
            if (g == 0)
                offset = groupArray[g].rm_eo;
            
            char cursorCopy[strlen(cursor) + 1];
            strcpy(cursorCopy, cursor);
            cursorCopy[groupArray[g].rm_eo] = 0;
            
            char *match = cursorCopy + groupArray[g].rm_so;

            if(DEBUG && g == 0){
                fprintf(stdout, "%lu: [%2llu-%2llu]: %s\n",
                        g, groupArray[g].rm_so, groupArray[g].rm_eo, match);
                }
            else if(g == 1) strcpy(url_components->scheme, match);
            else if(g == 2) strcpy(url_components->host, match);
            else if(g == 3) strcpy(url_components->path, match);
            else if(g == 4) strcpy(url_components->query, match);
            else if(g == 5) strcpy(url_components->fragment, match);
            
        }
        cnt++;
        cursor += offset;
        if(!full(q))
          que_append(q, url_components);
        else {
            free(url_components);
            break;
        }
    }
    
    printf("%d matches\n", cnt);
    regfree(&regexCompiled);
    return 0;
}
