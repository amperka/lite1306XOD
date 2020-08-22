
struct State { };

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto scr = getValue<input_SCR>(ctx);
    if (isInputDirty<input_DO>(ctx)) {
        int16_t x = getValue<input_X>(ctx);
        int16_t y = getValue<input_Y>(ctx);
        int16_t w = 1;
        int16_t h = 1;
        scr->dev->fixPlace(&x, &y, &w, &h);
        scr->dev->setChanged(x, w);
        if (x < scr->dev->width && x >= 0 && y < scr->dev->height && y >= 0) {
            scr->dev->storeUint8(1 /*DRAW_DOT*/);
            scr->dev->storeUint8((uint8_t)x);
            scr->dev->storeUint8((uint8_t)y);
            if (scr->dev->autoUpdate)
                scr->dev->render();
        }
        emitValue<output_DONE>(ctx, true);
    }
    emitValue<output_SCRU0027>(ctx, scr);
}
