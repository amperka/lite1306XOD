
struct State {
    xod::____ssd1306_device::State* dev;
};

using Type = State*;

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    State* state = getState(ctx);
    state->dev = getValue<input_DEV>(ctx);
    if (isInputDirty<input_DO>(ctx)) {
        state->dev->autoUpdate = getValue<input_AUTO>(ctx);
        state->dev->clearBuffer();
        state->dev->clearSsd1306();
        emitValue<output_DONE>(ctx, true);
    }
    emitValue<output_SCRU0027>(ctx, state);
}
