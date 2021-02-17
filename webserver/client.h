void traitement_client(int socket_client, char* buff, FILE* fichier);
char* fgets_or_exit(char* buffer, int size, FILE* stream);

enum http_method {
    HTTP_GET,
    HTTP_UNSUPPORTED,
};

typedef struct {
    enum http_method method;
    int major_version;
    int minor_version;
    char target[MAX_TARGET_SIZE];
} http_request;
