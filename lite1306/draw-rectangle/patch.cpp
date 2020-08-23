
struct State { };

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto scr = getValue<input_SCR>(ctx);
    if (isInputDirty<input_DO>(ctx)) {
        int16_t x = getValue<input_X>(ctx);
        int16_t y = getValue<input_Y>(ctx);
        int16_t w = getValue<input_W>(ctx);
        int16_t h = getValue<input_H>(ctx);
        bool filled = getValue<input_FILL>(ctx);
        scr->dev->fixPlace(&x, &y, &w, &h);
        scr->dev->setChanged(x, w+1);
        int16_t X = 0, Y = 0, W = 0, H = 0;
        if (w < 0) {
            w = -w;
            x = x - w;
        }
        if (h < 0) {
            h = -h;
            y = y - h;
        }
        if (x < 0) {
            X = 0;
            W = w + x;
        } else {
            X = x;
            W = ((x + w) > scr->dev->width) ? scr->dev->width : w;
        }
        if (y < 0) {
            Y = 0;
            H = h + y;
        } else {
            Y = y;
            H = ((y + h) > scr->dev->height) ? scr->dev->height : h;
        }
        if (W > 0 && H > 0 && y < scr->dev->height && x < scr->dev->width) {
            scr->dev->storeUint8((filled) ? 4 /*DRAW_BLOCK*/ : 3 /*DRAW_FRAME*/);
            scr->dev->storeUint8((uint8_t)X);
            scr->dev->storeUint8((uint8_t)Y);
            scr->dev->storeUint8((uint8_t)W);
            scr->dev->storeUint8((uint8_t)H);
            if (scr->dev->autoUpdate)
                scr->dev->render();
        }
        emitValue<output_DONE>(ctx, true);
    }
    emitValue<output_SCRU0027>(ctx, scr);
}
