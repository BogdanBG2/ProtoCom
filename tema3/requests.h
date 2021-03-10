#ifndef _REQUESTS_
#define _REQUESTS_

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char* compute_get_request(char*, char*, char*, char*, char*);

// computes and returns a POST request string (cookies can be NULL if not needed)
char* compute_post_request(char*, char*, char*, char**, int, char*, char*);

char* compute_delete_request(char*, char*, char*, char*, char*);
#endif
