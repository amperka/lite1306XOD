struct Font {
    uint8_t w;
    uint8_t h;
    const uint8_t* reloc;
    const uint8_t* font;
};

struct State { };

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto scr = getValue<input_SCR>(ctx);
    if (isInputDirty<input_DO>(ctx)) {
        int16_t x = getValue<input_X>(ctx);
        int16_t y = getValue<input_Y>(ctx);
        auto text = getValue<input_TEXT>(ctx);
        uint8_t* size;
        uint8_t* xx;
        uint8_t* yy;

        int16_t w = scr->dev->font->w * length(text);
        int16_t h = scr->dev->font->h;

        scr->dev->storeUint8(9 /*DRAW_STRING*/);

        xx = &scr->dev->cmd[scr->dev->cmdPtr];
        scr->dev->cmdPtr += 2;
        yy = &scr->dev->cmd[scr->dev->cmdPtr];
        scr->dev->cmdPtr += 2;
        size = &scr->dev->cmd[scr->dev->cmdPtr];
        scr->dev->cmdPtr++;

        dump(text, (char*)&scr->dev->cmd[scr->dev->cmdPtr]);
        uint8_t numOut = length(text);
        scr->dev->cmdPtr += numOut;

        /*uint8_t in = scr->dev->cmdPtr; // patches for UTF8, now do not work
    uint8_t numOut = 0;
    char c = 0;
      
    while (scr->dev->cmd[in] != 0 && in < 255) {
        c = scr->dev->cmd[in];
        scr->dev->debug_print_int((int)c);
        if (c == 0xd0) {
          in++;
          c = scr->dev->cmd[in];
          if (c == 0x81) {
            scr->dev->storeUint8((uint8_t)0xf1);
          } else if (c >= 0x90 && c <= 0xbf) {
            scr->dev->storeUint8((uint8_t)c - 0x10);
          } else {
            scr->dev->storeUint8((uint8_t)c);
          }
        } else if (c == 0xd1) {
          in++;
          c = scr->dev->cmd[in];
          if (c == 0x91) {
            scr->dev->storeUint8((uint8_t)0xf2);
          } else if (c >= 0x80 && c <= 0x8f) {
            scr->dev->storeUint8((uint8_t)c + 0x61);
          } else {
            scr->dev->storeUint8((uint8_t)c);
          }
        } else {
          scr->dev->storeUint8((uint8_t)c);
        }
    in++;
    numOut++;
    }*/

        /*auto c = text.iterate();
    switch (scr->dev->encoding) {
    case 1 /*ENC_UTF8* /:
      while (c) {
          scr->dev->debug_print_int((int)*c);
        if (*c == 0xd0) {
          ++c; //// test it
          if (*c == 0x81) {
            scr->dev->storeUint8((uint8_t)0xf1);
          } else if (*c >= 0x90 && *c <= 0xbf) {
            scr->dev->storeUint8((uint8_t)*c - 0x10);
          } else {
            scr->dev->storeUint8((uint8_t)*c);
          }
        } else if (*c == 0xd1) {
          ++c; //// test it
          if (*c == 0x91) {
            scr->dev->storeUint8((uint8_t)0xf2);
          } else if (*c >= 0x80 && *c <= 0x8f) {
            scr->dev->storeUint8((uint8_t)*c + 0x61);
          } else {
            scr->dev->storeUint8((uint8_t)*c);
          }
        } else {
          scr->dev->storeUint8((uint8_t)*c);
        }
        ++c;
        numOut++;
      }
      break;
    case 2 /*ENC_CP1251* /:
      while (c) {
        if (*c >= 0xc0 && *c <= 0xef) {
          scr->dev->storeUint8((uint8_t)*c - 0x40);
        } else if (*c >= 0xf0) {
          scr->dev->storeUint8((uint8_t)*c - 0x10);
        } else if (*c == 0xa8) {
          scr->dev->storeUint8((uint8_t)0xf0);
        } else if (*c == 0xb8) {
          scr->dev->storeUint8((uint8_t)0xf1);
        } else {
          scr->dev->storeUint8((uint8_t)*c);
        }
        ++c;
        numOut++;
      }
      break;
    default:
      while (c) {
        scr->dev->storeUint8((uint8_t)*c);
        ++c;
        numOut++;
      }
      break;
    }*/
        *size = numOut;
        w = numOut * scr->dev->font->w;
        scr->dev->fixPlace(&x, &y, &w, &h);
        scr->dev->setChanged(x, w);
        *xx = (x >> 8) & 0xff;
        *(xx + 1) = x & 0xff;
        *yy = (y >> 8) & 0xff;
        *(yy + 1) = y & 0xff;
        if (scr->dev->autoUpdate)
            scr->dev->render();
        emitValue<output_DONE>(ctx, true);
    }
    emitValue<output_SCRU0027>(ctx, scr);
}
