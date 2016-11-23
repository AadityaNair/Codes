#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

char *readFile(FILE *fp)
{
    int size = 0, seq_size=0, c, enc, digl, digr;
    char *ch, *seq, *enq;
    char found;

    if (fp == NULL)
        return NULL;

    ch = (char *)malloc(sizeof(char) * 400000);
    seq = (char *)malloc(sizeof(char) * 1600000);

    while((c = fgetc(fp)) != EOF){
        ch[size++] = (char) c;
        found = (char) c;
        if (found == ' ' || found == ',' || found == '.' || found == '!'){
            switch(found){
                case ' ' : seq[seq_size++] = '0';
                           seq[seq_size++] = '0';
                           break;
                case ',' : seq[seq_size++] = '6';
                           seq[seq_size++] = '2';
                           break;
                case '.' : seq[seq_size++] = '6';
                           seq[seq_size++] = '3';
                           break;
                case '!' : seq[seq_size++] = '6';
                           seq[seq_size++] = '4';
                           break;
            }
        }
        else
        {
            if(islower(c))
            {
                enc = found - 'a' + 26;
                digl = enc % 10;
                digr = (enc / 10) % 10;
                seq[seq_size++] = (char) (digr + '0');
                seq[seq_size++] = (char) (digl + '0');
                //printf("%d ", enc);    
            }
            else if(isupper(c))
            {
                enc = found - 'A' + 1;
                if(enc < 10)
                {
                    digl = 0;
                    digr = enc;
                    seq[seq_size++] = (char) (digl + '0');
                    seq[seq_size++] = (char) (digr + '0');
                }
                else
                {
                    digl = enc % 10;
                    digr = (enc / 10) % 10;
                    seq[seq_size++] = (char) (digr + '0');
                    seq[seq_size++] = (char) (digl + '0');
                }
            }
            else if(isdigit(c))
            {
                enc = found - '0' + 52;
                digl = enc % 10;
                digr = (enc / 10) % 10;
                seq[seq_size++] = (char) (digr + '0');
                seq[seq_size++] = (char) (digl + '0');
            }
        }
    }
    seq[seq_size++] = '\0';     
    return seq;
}

char* decode_string(char* text)
{
    char dec[2];
    char* send = (char* )malloc(sizeof(char) * 400000);
    int make, i, size=0;

    if(text == NULL)
        return NULL;

    for(i = 0; i < strlen(text); i += 2)
    {
        dec[0] = text[i];
        dec[1] = text[i + 1];
        make = (((int)dec[0] - '0') * 10) + ((int)dec[1] - '0');
        if(make == 0)
            send[size++] = ' ';
        else if((make >= 1) && (make <= 25))
            send[size++] = 'A' + make - 1;
        else if((make >= 26) && (make <= 51))
            send[size++] = (make - 26) + 'a';
        else if((make >= 52) && (make <= 61))
            send[size++] = (make - 52) + '0';
        else if(make == 62)
            send[size++] = ',';
        else if(make == 63)
            send[size++] = '.';
        else if(make == 64)
            send[size++] = '!';
    }
    send[size++] = '\0';
    return send;
}
