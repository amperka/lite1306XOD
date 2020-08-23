
struct State { };

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto scr = getValue<input_SCR>(ctx);
    if (isInputDirty<input_DO>(ctx)) {
        scr->dev->color = getValue<input_COLOR>(ctx);
        scr->dev->storeUint8(10 /*SET_COLOR*/);
        scr->dev->storeUint8((uint8_t)scr->dev->color);
        emitValue<output_DONE>(ctx, true);
    }
    emitValue<output_SCRU0027>(ctx, scr);
}
