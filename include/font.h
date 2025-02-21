// Fontes para A-Z, 0-9 e a-z. Os caracteres tem 8x8 pixels

static uint8_t font[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // Vazio

    //Números
    0x7e,0xff,0xff,0xc3,0xff,0xff,0x7e,0x00, // 0
    0xc2,0xc2,0xff,0xff,0xff,0xc0,0xc0,0x00, // 1
    0xe6,0xf7,0xf7,0xd3,0xd3,0xdf,0xce,0x00, // 2
    0x66,0xe7,0xdb,0xdb,0xdb,0xff,0x66,0x00, // 3
    0x0f,0x1f,0x10,0x10,0x10,0xff,0xff,0x00, // 4
    0x67,0xef,0xc9,0xc9,0xc9,0xf9,0x71,0x00, // 5
    0x7e,0xff,0x8f,0x89,0xf9,0xf9,0x70,0x00, // 6
    0x01,0xc1,0xf1,0xf9,0x3f,0x0f,0x07,0x00, // 7
    0x66,0xff,0xdb,0xdb,0xdb,0xff,0x66,0x00, // 8
    0x0e,0x9f,0x9f,0x91,0xf1,0xff,0x7e,0x00, // 9

    // Letras Maiúsculas
    0xfe,0xff,0xff,0x09,0xff,0xff,0xfe,0x00, // A
    0xff,0xff,0xff,0xdb,0xdb,0xff,0x66,0x00, // B
    0x7e,0xff,0xff,0xc3,0xc3,0xc3,0xc3,0x00, // C
    0xff,0xff,0xff,0x81,0xff,0xff,0x7e,0x00, // D
    0x7e,0x7e,0xdb,0xdb,0xdb,0xc3,0xc3,0x00, // E
    0xfe,0xff,0x1b,0x1b,0x1b,0x03,0x03,0x00, // F
    0x7e,0xff,0x83,0x93,0xf3,0xf3,0xf3,0x00, // G
    0xff,0xff,0xff,0x18,0xff,0xff,0xff,0x00, // H
    0xc3,0xc3,0xff,0xff,0xff,0xc3,0xc3,0x00, // I
    0x60,0xe0,0xe3,0x83,0x83,0xff,0x7f,0x00, // J
    0xff,0xff,0xff,0x18,0xff,0xff,0xe7,0x00, // K
    0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0x00, // L
    0xff,0xff,0x01,0xff,0x01,0xff,0xfe,0x00, // M
    0xff,0xff,0xff,0x0c,0x18,0xff,0xff,0x00, // N
    0x7e,0xff,0xff,0xc3,0xff,0xff,0x7e,0x00, // O
    0xff,0xff,0xff,0x11,0x1f,0x1f,0x0e,0x00, // P
    0x3e,0x7f,0x7f,0x63,0xe3,0xff,0xbe,0x00, // Q
    0xff,0xff,0xff,0x19,0x19,0xff,0xf6,0x00, // R
    0xce,0xdf,0xdf,0xdb,0xdb,0xfb,0x73,0x00, // S
    0x03,0x03,0xff,0xff,0xff,0x03,0x03,0x00, // T
    0xff,0xff,0xff,0xc0,0xff,0xff,0xff,0x00, // U
    0x3f,0x7f,0xff,0xc0,0xff,0x7f,0x3f,0x00, // V
    0xff,0xff,0x80,0xff,0x80,0xff,0x7f,0x00, // W
    0xe7,0xff,0xff,0x18,0xff,0xff,0xe7,0x00, // X
    0x0f,0x1f,0xff,0xf8,0xff,0x1f,0x0f,0x00, // Y
    0xc3,0xe3,0xf3,0xdb,0xcf,0xc7,0xc3,0x00, // Z

    //Letras Minúsculas
    0x64,0xf6,0x92,0x92,0xfe,0xfc,0xc0,0x00, // a
    0xfe,0xfe,0xfe,0x88,0x88,0xf8,0x70,0x00, // b
    0x7c,0xfe,0xfe,0x82,0x82,0xee,0x6c,0x00, // c
    0x70,0xf8,0x88,0x88,0xfe,0xfe,0xfe,0x00, // d
    0x7c,0xfe,0xf2,0x92,0x92,0x9e,0x9c,0x00, // e
    0x00,0x10,0x10,0xfc,0xfe,0x12,0x12,0x00, // f
    0x4c,0xde,0xde,0x92,0x92,0xfe,0x7e,0x00, // g
    0xfe,0xfe,0xf8,0x18,0x18,0xf8,0xf0,0x00, // h
    0x80,0x80,0x88,0xfb,0xfb,0x80,0x80,0x00, // i
    0x00,0x40,0xc0,0x80,0x90,0xf6,0x76,0x00, // j
    0x00,0xfe,0xfe,0x10,0x38,0xee,0xc6,0x00, // k
    0x82,0x82,0xfe,0xfe,0xfe,0x80,0x80,0x00, // l
    0xfe,0xfe,0x06,0x7c,0x06,0xfe,0xfc,0x00, // m
    0xfe,0xfe,0xfe,0x02,0xfe,0xfe,0xfc,0x00, // n
    0x7c,0xfe,0xc6,0xc6,0xc6,0xfe,0x7c,0x00, // o
    0xfe,0xfe,0x36,0x36,0x3e,0x3e,0x1c,0x00, // p
    0x1c,0x3e,0x3e,0x36,0x36,0xfe,0xfe,0x00, // q
    0xfe,0xfe,0x06,0x06,0x06,0x0c,0x0c,0x00, // r
    0x4c,0xde,0x92,0x92,0x92,0xf6,0x64,0x00, // s
    0x08,0x08,0x7e,0xfe,0xfe,0x88,0x88,0x00, // t
    0x7e,0xfe,0x80,0x80,0x80,0xfe,0xfe,0x00, // u
    0x0e,0x3e,0xf0,0xe0,0xf0,0x3e,0x0e,0x00, // v
    0x3e,0xfe,0xc0,0x3c,0xc0,0xfe,0x3e,0x00, // w
    0xc6,0xee,0x38,0x38,0x38,0xee,0xc6,0x00, // x
    0x4e,0xde,0x90,0x90,0x90,0xfe,0x7e,0x00, // y
    0xc2,0xe2,0xf2,0x9a,0x8e,0x86,0x86,0x00, // z

    0x00,0x00,0x00,0x06,0x09,0x09,0x06,0x00, // º
    0xc3,0xc3,0x00,0x00,0x00,0x00,0x00,0x00, // :
    0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00, // -
    0x00,0x00,0x00,0xc0,0xc0,0x00,0x00,0x00, // .
    0x00,0x00,0x00,0xdf,0xdf,0x00,0x00,0x00, // !

    0xc0,0xe0,0xb0,0x98,0x8c,0x86,0x83,0xad, // ( alerta esquerdo
    0xad,0x83,0x86,0x8c,0x98,0xb0,0xe0,0xc0 // ) alerta direito
};