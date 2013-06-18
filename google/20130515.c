integers. varint = variable length integer

8 bits = 1 byte.

MSB + 7 bits.

1xxxxxxx 1xxxxxxx 0xxxxxxx

green red black

Encoder() int32->varint

Decoder() varint->int32

-1

0xfff …. fff

0x01111111 127

int encode(char *decoded, int integer)

{

    int mask = 0x7f;

    unsigned int x = integer;

    if (x == 0) {

        decoded[0] = 0;

        return 1;

    }

    int num = 0;

    while (x != 0) {

        char group = x & mask;

        x >>= 7;

        if (x != 0) {

            group |= 0x80;

        }

       

        decoded[num++] = group;

    }

    return num;

}

int decode(char *encoded, int &output) {

    unsigned int charIn;

    int num = 0;

    output = 0;

    while (true) {

        charIn = encoded[num];

        output |= ((charIn & 0x7f) << (7*num));

       

        num++;

        if (!(charIn & 0x80)) {

            break;

        }

    }

    return num;

}
