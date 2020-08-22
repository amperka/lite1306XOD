struct Font {
    uint8_t w;
    uint8_t h;
    const uint8_t* reloc;
    const uint8_t* font;
};

using Type = uint32_t;

struct State {
    struct Font font;
};

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto state = getState(ctx);
    emitValue<output_FONT>(ctx, (uint32_t)&state->font);
}
