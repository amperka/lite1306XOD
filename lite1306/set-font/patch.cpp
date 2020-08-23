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
        scr->dev->font = (uint32_t)getValue<input_FONT>(ctx);
        scr->dev->storeUint8(11 /*SET_FONT*/);
        scr->dev->storeUint32(scr->dev->font);
        emitValue<output_DONE>(ctx, true);
    }
    emitValue<output_SCRU0027>(ctx, scr);
}
