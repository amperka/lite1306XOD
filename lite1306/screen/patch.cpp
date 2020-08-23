
struct State {
    xod::____ssd1306_device::State* dev;
    bool clear;
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
        state->clear = getValue<input_CLR>(ctx);
        state->dev->clearBuffer();
        if(state->clear) {
            /*state->dev->clearSsd1306();
        } else {*/
            state->dev->clearSsd1306(state->dev->unclearedLeft,state->dev->unclearedRight);
        }
        emitValue<output_DONE>(ctx, true);
    }
    emitValue<output_SCRU0027>(ctx, state);
}
