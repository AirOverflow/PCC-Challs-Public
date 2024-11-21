/*
    This is BETA implementation, tons and tons of formatting and code cleanup is needed.
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rc4_state {
    int x, y, m[256];
};

using namespace std;

namespace crypto {

    void setup( struct rc4_state *s, unsigned char *key,  int length ) {
        int i, j, k, *m, a;

        s->x = 0;
        s->y = 0;
        m = s->m;

        for( i = 0; i < 256; i++ )
            m[i] = i;

        j = k = 0;

        for( i = 0; i < 256; i++ ) {
            a = m[i];
            j = (unsigned char) ( j + a + key[k] );
            m[i] = m[j]; m[j] = a;
            if( ++k >= length ) k = 0;
        }
    }

    void do_algo(struct rc4_state *s, unsigned char *data, int length ) {
        int i, x, y, *m, a, b;

        x = s->x;
        y = s->y;
        m = s->m;

        for( i = 0; i < length; i++ ) {
            x = (unsigned char) ( x + 1 ); a = m[x];
            y = (unsigned char) ( y + a );
            m[x] = b = m[y];
            m[y] = a;
            data[i] ^= m[(unsigned char) ( a + b )];
        }

        s->x = x;
        s->y = y;
    }


    unsigned char* crypt(const unsigned char* key, const unsigned char* data) {

        // Create local copies:
        unsigned char* key_copy = (unsigned char*)malloc(strlen((char*)key));
        unsigned char* data_copy = (unsigned char*)malloc(strlen((char*)data));

        memset(key_copy, 0, strlen((char*)key));
        memset(data_copy, 0, strlen((char*)data));

        strncpy((char*)key_copy, (char*)key, strlen((char*)key));
        strncpy((char*)data_copy, (char*)data, strlen((char*)data));

        struct rc4_state state;
        setup(&state, key_copy, strlen((char*)key_copy));
        do_algo(&state, data_copy, strlen((char*)data_copy));

        free(key_copy);

        return data_copy;
    }

}
