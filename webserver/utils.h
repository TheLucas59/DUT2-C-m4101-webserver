char* rewrite_target(char* target);
FILE* check_and_open(const char *target, const char *document_root, int* status);
int get_file_size(FILE* file);
int copy(FILE *in, FILE *out);