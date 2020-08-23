#ifdef __AVR__
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#endif // __AVR__

/*
 * Not all non-AVR boards installs define macros
 * for compatibility with existing PROGMEM-reading AVR code.
 */
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char*)(addr))
#endif

// clang-format off
{{#global}}
#include <Wire.h>
{{/global}}
// clang-format on
constexpr uint8_t SSD1306_ADDRESS = 0x3C;
constexpr uint8_t SSD1306_SET_CONTRAST = 0x81;
constexpr uint8_t SSD1306_RAM_ON = 0xA4;
constexpr uint8_t SSD1306_RAM_OFF = 0xA5;
constexpr uint8_t SSD1306_INVERT_OFF = 0xA6;
constexpr uint8_t SSD1306_INVERT_ON = 0xA7;
constexpr uint8_t SSD1306_DISPLAY_OFF = 0xAE;
constexpr uint8_t SSD1306_DISPLAY_ON = 0xAF;
constexpr uint8_t SSD1306_NOP = 0xE3;
constexpr uint8_t SSD1306_SCROLL_ON = 0x2F;
constexpr uint8_t SSD1306_SCROLL_OFF = 0x2E;
constexpr uint8_t SSD1306_SCROLL_HORIZONTAL_RIGHT = 0x26;
constexpr uint8_t SSD1306_SCROLL_HORIZONTAL_LEFT = 0x27;
constexpr uint8_t SSD1306_SCROLL_DIAGONAL_RIGHT = 0x29;
constexpr uint8_t SSD1306_SCROLL_DIAGONAL_LEFT = 0x2A;
constexpr uint8_t SSD1306_SCROLL_VERTICAL = 0xA3;
constexpr uint8_t SSD1306_ADDR_COLUMN_LBS = 0x00;
constexpr uint8_t SSD1306_ADDR_COLUMN_MBS = 0x10;
constexpr uint8_t SSD1306_ADDR_MODE = 0x20;
constexpr uint8_t SSD1306_ADDR_COLUMN = 0x21;
constexpr uint8_t SSD1306_ADDR_PAGE = 0x22;
constexpr uint8_t SSD1306_ADDR_ONE_PAGE = 0xB0;
constexpr uint8_t SSD1306_SET_START_LINE = 0x40;
constexpr uint8_t SSD1306_SET_REMAP_R_TO_L = 0xA0;
constexpr uint8_t SSD1306_SET_REMAP_L_TO_R = 0xA1;
constexpr uint8_t SSD1306_SET_MULTIPLEX_RATIO = 0xA8;
constexpr uint8_t SSD1306_SET_REMAP_D_TO_T = 0xC0;
constexpr uint8_t SSD1306_SET_REMAP_T_TO_D = 0xC8;
constexpr uint8_t SSD1306_SET_DISPLAY_OFFSET = 0xD3;
constexpr uint8_t SSD1306_SET_COM_PINS = 0xDA;
constexpr uint8_t SSD1306_SET_DISPLAY_CLOCK = 0xD5;
constexpr uint8_t SSD1306_SET_PRECHARGE_PERIOD = 0xD9;
constexpr uint8_t SSD1306_SET_VCOM_DESELECT = 0xDB;
constexpr uint8_t SSD1306_CHARGE_DCDC_PUMP = 0x8D;
constexpr uint8_t SSD1306_SHIFT_DC = 0x06;
constexpr uint8_t SSD1306_SHIFT_CO = 0x07;
constexpr uint8_t SSD1306_COMMAND = (0 << SSD1306_SHIFT_CO) | (0 << SSD1306_SHIFT_DC);
constexpr uint8_t SSD1306_DATA = (0 << SSD1306_SHIFT_CO) | (1 << SSD1306_SHIFT_DC);

constexpr int16_t ARRANGE_TOP = 32762;
constexpr int16_t ARRANGE_BOTTOM = 32763;
constexpr int16_t ARRANGE_LEFT = 32764;
constexpr int16_t ARRANGE_RIGHT = 32765;
constexpr int16_t ARRANGE_CENTER = 32766;

// clang-format off
const uint8_t initialisationSequence[] PROGMEM = { 
    SSD1306_DISPLAY_OFF,
    SSD1306_SET_DISPLAY_CLOCK, 0x80,
    SSD1306_SET_MULTIPLEX_RATIO, 0x3f,
    SSD1306_SET_DISPLAY_OFFSET, 0,
    SSD1306_SET_START_LINE | 0,
    SSD1306_CHARGE_DCDC_PUMP, 0x14,
    SSD1306_ADDR_MODE, 0x01,
    SSD1306_SET_REMAP_L_TO_R,
    SSD1306_SET_REMAP_T_TO_D,
    SSD1306_SET_COM_PINS, 0x12,
    SSD1306_SET_CONTRAST, 0xff,
    SSD1306_SET_PRECHARGE_PERIOD, 0xf1,
    SSD1306_SET_VCOM_DESELECT, 0x40,
    SSD1306_RAM_ON,
    SSD1306_INVERT_OFF,
    SSD1306_DISPLAY_ON
};
// clang-format on

struct Font {
    uint8_t w;
    uint8_t h;
    const uint8_t* reloc;
    const uint8_t* font;
};

struct State {
    uint8_t i2cAddress;
    uint8_t width;
    uint8_t height;
    uint8_t color;
    bool autoUpdate;
    struct Font* font;
    int16_t changedLeft;
    int16_t changedRight;
    int16_t unclearedLeft;
    int16_t unclearedRight;

    union {
        uint64_t u;
        uint8_t a[8];
    } row;

    uint8_t cmd[256];
    int16_t cmdSize;
    int16_t cmdPtr;

    void clearBuffer(void) {
        memset(cmd, 0, cmdSize);
        cmdPtr = 0;
        changedLeft = width;
        changedRight = -1;
    }

    void setChanged(Number x, Number w) {
        if (x <= (x + w)) {
            if (x < changedLeft)
                changedLeft = (x > 0) ? (x) : 0;
            if ((x + w) > changedRight)
                changedRight = ((x + w) < width) ? (x + w) : (width - 1);
        } else {
            if ((x + w) < changedLeft)
                changedLeft = ((x + w) > 0) ? (x + w) : 0;
            if (x > changedRight)
                changedRight = (x < width) ? (x) : (width - 1);
        }
        unclearedLeft = (unclearedLeft < changedLeft)? changedLeft : unclearedLeft;
        unclearedRight = (unclearedRight > changedRight)? changedRight : unclearedRight;
    }

    void fixPlace(int16_t* x, int16_t* y, int16_t* w, int16_t* h) {
        switch (*x) {
        case ARRANGE_LEFT:
            *x = 0;
            break;
        case ARRANGE_CENTER:
            *x = (width - *w) / 2;
            break;
        case ARRANGE_RIGHT:
            *x = width - *w - 1;
            break;
        default:
            break;
        }
        switch (*y) {
        case ARRANGE_TOP:
            *y = 0;
            break;
        case ARRANGE_CENTER:
            *y = (height - *h) / 2;
            break;
        case ARRANGE_BOTTOM:
            *y = height - *h - 1;
            break;
        default:
            break;
        }
    }

    void sendCommand(uint8_t command) {
        Wire.beginTransmission(i2cAddress);
        Wire.write(0x80);
        Wire.write(command);
        Wire.endTransmission();
    }

    void pushBufferToSsd1306(uint8_t rowNumber) {
        sendCommand(SSD1306_ADDR_PAGE);
        sendCommand(0);
        sendCommand(7);
        sendCommand(SSD1306_ADDR_COLUMN);
        sendCommand(rowNumber);
        sendCommand(rowNumber);
        Wire.beginTransmission(i2cAddress);
        Wire.write(0x40);
        for (uint8_t x = 0; x < 8; x++) {
            Wire.write(row.a[x]);
        }
        Wire.endTransmission();
    }

    void sendArray(const uint8_t a[], uint16_t size) {
        for (uint16_t i = 0; i < size; i++) {
            sendCommand(pgm_read_byte(&a[i]));
        }
    }

    void clearSsd1306(uint8_t b = 0, uint8_t e = 127) {
        row.u = 0;
        for (uint8_t i = b; i < e; i++)
            pushBufferToSsd1306(i);
        unclearedLeft = width;
        unclearedRight = -1;
    }

    void punch(uint64_t temp, uint8_t c) {
        if (temp != 0) {
            switch (c) {
            case 1 /*COLOR_WHITE*/:
                row.u |= temp;
                break;
            case 0 /*COLOR_BLACK*/:
                row.u &= ~temp;
                break;
            case 2 /*COLOR_INVERSE*/:
                row.u ^= temp;
                break;
            default:
                break;
            }
        }
    }

    uint8_t loadUint8(int16_t* pos) {
        uint8_t temp = cmd[*pos];
        *pos += 1;
        return temp;
    }

    int16_t loadInt16(int16_t* pos) {
        int16_t temp = (int16_t)((uint16_t)cmd[*pos] << 8 | (uint16_t)cmd[*pos + 1]);
        *pos += 2;
        return temp;
    }

    uint32_t loadUint32(int16_t* pos) {
        uint16_t temp = (uint32_t)cmd[*pos] << 24 | (uint32_t)cmd[*pos + 1] << 16 | (uint32_t)cmd[*pos + 2] << 8 | (uint32_t)cmd[*pos + 3];
        *pos += 4;
        return temp;
    }

    void storeUint8(uint8_t n) { cmd[cmdPtr++] = n; }

    void storeInt16(int16_t n) {
        cmd[cmdPtr++] = (uint8_t)(((int16_t)n >> 8) & 0xff);
        cmd[cmdPtr++] = (uint8_t)(((int16_t)n) & 0xff);
    }

    void storeUint32(uint32_t n) {
        cmd[cmdPtr++] = (uint8_t)(((uint32_t)n >> 24) & 0xff);
        cmd[cmdPtr++] = (uint8_t)(((uint32_t)n >> 16) & 0xff);
        cmd[cmdPtr++] = (uint8_t)(((uint32_t)n >> 8) & 0xff);
        cmd[cmdPtr++] = (uint8_t)(((uint32_t)n) & 0xff);
    }

    void render() {
        uint8_t x, y, w, h, x0, y0, x1, y1, x2, y2, c, g, offset;
        int16_t X, Y, R, q;
        uint64_t temp;
        for (uint8_t r = changedLeft; r < changedRight; r++) {
            int16_t i = 0;
            row.u = 0;
            while (i != cmdPtr) {
                switch (loadUint8(&i)) {
                case 1 /*DRAW_DOT*/:
                    x = loadUint8(&i);
                    y = loadUint8(&i);
                    if (x == r)
                        punch((uint64_t)1 << y, color);
                    break;
                case 2 /*DRAW_LINE*/:
                    x = loadUint8(&i);
                    y = loadUint8(&i);
                    x2 = loadUint8(&i);
                    y2 = loadUint8(&i);
                    if (x < x2) {
                        x0 = x;
                        x1 = x2;
                        y0 = y;
                        y1 = y2;
                    } else {
                        x0 = x2;
                        x1 = x;
                        y0 = y2;
                        y1 = y;
                    }
                    if (abs(x1 - x0) >= abs(y1 - y0)) {
                        int16_t position = (r - x0) * (y1 - y0) / (x1 - x0) + y0;
                        if (r >= x0 && r <= x1)
                            punch((uint64_t)1 << position, color);
                    } else {
                        temp = 0;
                        for (int16_t i = min(y0, y1); i < max(y0, y1); i++) {
                            if (((i - y0) * (x1 - x0 + 1) / (y1 - y0) + x0) == r) {
                                temp |= ((uint64_t)1) << i;
                            }
                        }
                        if (r >= x0 && r <= x1)
                            punch(temp, color);
                    }
                    break;
                case 3 /*DRAW_FRAME*/:
                    x = loadUint8(&i);
                    y = loadUint8(&i);
                    w = loadUint8(&i);
                    h = loadUint8(&i);
                    y0 = (min(y, (y + h)) >= 0) ? min(y, (y + h)) : 0;
                    y1 = (max(y, (y + h)) < 64) ? max(y, (y + h)) : 63;
                    temp = 0;
                    if (r == x || r == x + w) {
                        for (int16_t i = y0; i <= y1; i++) {
                            temp |= ((uint64_t)1) << i;
                        }
                    } else {
                        temp |= ((uint64_t)1) << y0 | ((uint64_t)1) << y1;
                    }
                    if (r >= x && r <= x + w)
                        punch(temp, color);
                    break;
                case 4 /*DRAW_BLOCK*/:
                    x = loadUint8(&i);
                    y = loadUint8(&i);
                    w = loadUint8(&i);
                    h = loadUint8(&i);
                    y0 = (min(y, (y + h)) >= 0) ? min(y, (y + h)) : 0;
                    y1 = (max(y, (y + h)) < 64) ? max(y, (y + h)) : 63;
                    temp = 0;
                    for (int16_t i = y0; i <= y1; i++) {
                        temp |= ((uint64_t)1) << i;
                    }
                    if (r >= x && r <= x + w)
                        punch(temp, color);
                    break;
                case 5 /*DRAW_AROUND*/:
                    X = loadInt16(&i);
                    Y = loadInt16(&i);
                    R = loadInt16(&i);
                    temp = 0;
                    for (uint8_t i = Y - R; i <= Y + R; i++) {
                        if (i < 64) {
                            int16_t kx = abs(X - r);
                            int16_t ky = abs(Y - i);
                            if ((int16_t)sqrt(kx * kx + ky * ky) == R) {
                                temp |= ((uint64_t)1) << i;
                            }
                        }
                    }
                    if (r >= X - R && r <= X + R)
                        punch(temp, color);
                    break;
                case 6 /*DRAW_CIRCLE*/:
                    X = loadInt16(&i);
                    Y = loadInt16(&i);
                    R = loadInt16(&i);
                    temp = 0;
                    for (uint8_t i = Y - R; i <= Y + R; i++) {
                        if (i < 64) {
                            int16_t kx = abs(X - r);
                            int16_t ky = abs(Y - i);
                            if ((int16_t)sqrt(kx * kx + ky * ky) <= R) {
                                temp |= ((uint64_t)1) << i;
                            }
                        }
                    }
                    if (r >= X - R && r <= X + R)
                        punch(temp, color);
                    break;
                case 9 /*DRAW_STRING*/:
                    X = loadInt16(&i);
                    Y = loadInt16(&i);
                    g = loadUint8(&i); // size

                    w = font->w;
                    h = font->h;

                    for (uint8_t k = 0; k < g; k++) {
                        c = loadUint8(&i);
                        if (X + w * k <= r && X + w * (k + 1) >= r) {
                            offset = pgm_read_byte(&font->reloc[(uint8_t)c]);
                            temp = 0;
                            for (int16_t j = (h / 8) - 1; j > -1; j--) {
                                uint8_t c = pgm_read_byte(&font->font[offset * w * (h / 8) + abs(r - X) + (w * j) - w * k]);
                                temp = (temp << 8) | (uint64_t)c;
                            }
                            temp = temp << Y;
                            if (r >= X && r < X + w * g)
                                punch(temp, color);
                        }
                    }
                    break;
                case 10 /*SET_COLOR*/:
                    color = loadUint8(&i);
                    break;
                case 11 /*SET_FONT*/:
                    font = loadUint32(&i);
                    break;

                default:
                    break;
                }
            }
            if (row.u != 0L)
                pushBufferToSsd1306(r);
        }
        changedLeft = width;
        changedRight = -1;
    }

    /*void debug_print_int(int n) {
    char s[10];
    memset(s, 0, 10);
    char *p = &s[9];
    if (n == 0) {
      *(--p) = '0';
    } else {
      while (n > 0) {
        *(--p) = (char)(n % 10 + '0');
        n = n / 10;
      }
    }
    while (*p != 0) {
      XOD_DEBUG_SERIAL.write((char)*(p++));
    }
    XOD_DEBUG_SERIAL.write('\r');
    XOD_DEBUG_SERIAL.write('\n');
    XOD_DEBUG_SERIAL.flush();
  }*/
};

using Type = State*;

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    if (isSettingUp()) {
        State* state = getState(ctx);
        state->i2cAddress = getValue<input_ADDR>(ctx);
        state->width = getValue<input_W>(ctx);
        state->height = getValue<input_H>(ctx);
        state->color = 2;
        state->font = -1;
        state->autoUpdate = true;
        Wire.begin();
        state->sendArray(initialisationSequence, sizeof(initialisationSequence));
        emitValue<output_DEVU0027>(ctx, state);
    }
}
