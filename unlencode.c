#include<stdio.h>

char * urlencode(char const *s, int len, int *new_length)
{
    printf( s);
    printf("\n");

    unsigned char const *from, *end;
    from = s;
    end = s + len;
    start = to = (unsigned char *) malloc(3 * len + 1);

    unsigned char hexchars[] = "0123456789ABCDEF";

    while (from < end) {
        c = *from++;

        if (c == ' ') {
            *to++ = '+';
        } else if ((c < '0' && c != '-' && c != '.') 

                   ||(c < 'A' && c > '9') 

                   ||(c > 'Z' && c < 'a' && c != '_') 

                   ||(c > 'z')) {
            to[0] = '%';
            to[1] = hexchars[c >> 4];
            to[2] = hexchars[c & 15];
            to += 3;
        } else {
            *to++ = c;
        }
    }
    *to = 0;
    if (new_length) {
        *new_length = to - start;
    }
    return (char *) start;

}
int main()
{
	char *buf=NULL;
	char *buf1="$%^&&****";
	int i = strlen(buf1);
	int new_length;
	buf = urlencode(buf1, i, &new_length);
	return 0;
}