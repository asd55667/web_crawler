#include "parser.h"
#include "utils.h"

int doc_parse(char *document){

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
    
//    char *scheme, *host, *path, *query, *fragment;
//    scheme = host = path = query = fragment = "";
    char scheme[1<<4], host[1<<6], path[BUFSIZ], query[BUFSIZ], fragment[1<<6];
    char *cursor = document;
    int cnt = 0;
    for (size_t m = 0; m < maxMatches; m++)
    {
        if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0))
            break;  // No more matches
        
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
//            printf("%lu ", g);
            if(DEBUG && g == 0){
                fprintf(stdout, "%lu: [%2llu-%2llu]: %s\n",
                        g, groupArray[g].rm_so, groupArray[g].rm_eo, match);
                }
            else if(g == 1) strcpy(scheme, match);
            else if(g == 2) strcpy(host, match);
            else if(g == 3) strcpy(path, match);
            else if(g == 4) strcpy(query, match);
            else if(g == 5) strcpy(fragment, match);
            
        }
        cnt++;
        cursor += offset;
        char *url_components[5];
        url_components[0] = scheme;
        url_components[1] = host;
        url_components[2] = path;
        url_components[3] = query;
        url_components[4] = fragment;
//        for(int i = 0; i < 5; i++)
//            printf("%s\n", url_components[i]);
//        printf("\n");
    }
    
    printf("%d matches\n", cnt);
    regfree(&regexCompiled);
    return 0;
}
