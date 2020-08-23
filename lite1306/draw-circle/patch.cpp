
struct State { };

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto scr = getValue<input_SCR>(ctx);
    if (isInputDirty<input_DO>(ctx)) {
        int16_t x = getValue<input_X>(ctx);
        int16_t y = getValue<input_Y>(ctx);
        int16_t r = getValue<input_R>(ctx);
        bool filled = getValue<input_FILL>(ctx);
        int16_t w = 1;
        int16_t h = 1;
        scr->dev->fixPlace(&x, &y, &w, &h);
        scr->dev->setChanged(x - r, 2 * r + 1 /*w*/);
        if ((x - r >= 0 && x - r <= scr->dev->width) || (x + r >= 0 && x + r <= scr->dev->width)) {
            scr->dev->storeUint8((filled) ? 6 /*DRAW_CIRCLE*/ : 5 /*DRAW_AROUND*/);
            scr->dev->storeInt16((int16_t)x);
            scr->dev->storeInt16((int16_t)y);
            scr->dev->storeInt16((int16_t)r);
            if (scr->dev->autoUpdate)
                scr->dev->render();
        }
        emitValue<output_DONE>(ctx, true);
    }
    emitValue<output_SCRU0027>(ctx, scr);
}
