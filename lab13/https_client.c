#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include "stdio.h"
#include "string.h"

int main()
{
    BIO * bio;
    SSL * ssl;
    SSL_CTX * ctx;

    int p;

    char * request = "GET / HTTP/1.0\r\nHost: www.verisign.com\r\n\r\n";
    char r[1024];

    /* initializare librarie */

    SSL_library_init();
    ERR_load_BIO_strings();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();

    /* TO DO initializare context */
    ctx = SSL_CTX_new(SSLv23_client_method());

    /* incarca trust store */
    if(! SSL_CTX_load_verify_locations(ctx, "TrustStore.pem", NULL))
    {
        fprintf(stderr, "Error loading trust store\n");
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        exit(-1);
    }


    if(! SSL_CTX_load_verify_locations(ctx, "TrustStore.pem", NULL))
    {
        fprintf(stderr, "Error loading trust store\n");
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(ctx);
        return 0;
    }


    /* stabileste conexiune */

    bio = BIO_new_ssl_connect(ctx);
    if (bio == NULL)
        perror("ssl connect:");

    /* Seteaza flag SSL_MODE_AUTO_RETRY  */

    BIO_get_ssl(bio, & ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    /* TO DO setup conexiune */
    char connect_to[100];
    memset(connect_to, 0, 100);
    sprintf(connect_to, "%s:%d", "google.com", 443);

    BIO_get_ssl(bio, &ssl);
    BIO_set_conn_hostname(bio, connect_to);

    if(BIO_do_connect(bio) <= 0)
        perror("connect");
    
    /* TO DO verifica certificat */
    if(SSL_get_verify_result(ssl) != X509_V_OK)
        perror("verifiy certificate");

    /* Trimite request */

    BIO_write(bio, request, strlen(request));

    /* TO DO Citeste raspuns si pregateste output*/
    int retry_count = 1;
    int bytes_wrote;

write_label:
    bytes_wrote = BIO_write(bio, request, strlen(request));
    if (bytes_wrote < 0) {
        if (BIO_should_retry(bio) && retry_count <= 3) {
            retry_count++;
            goto write_label;
        }
        else
            perror("Could not write");
    }

    FILE *file = fopen("output.html", "wt");
    if (file == NULL)
        perror("File not found");

    int recv_count = 0;

    char buffer[1024];
    int bytes_read;
    do {
        retry_count = 1;
    
    read_label:
        memset(buffer, 0, 1024);
        bytes_read = BIO_read(bio, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            if (BIO_should_retry(bio) && retry_count <= 3) {
                retry_count++;
                goto read_label;
            }
            else
                perror("Could not read");
        }

        recv_count++;

        if (recv_count > 1)
            fwrite(buffer, bytes_read, 1, file);
    } while(bytes_read > 0);

    fclose(file);

    /* Inchide conexiune si elibereaza context */
    BIO_free_all(bio);
    SSL_CTX_free(ctx);

    return 0;
}
