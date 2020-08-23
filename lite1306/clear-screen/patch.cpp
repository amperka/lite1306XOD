
struct State { };

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto dev = getValue<input_DEV>(ctx);
    if (isInputDirty<input_DO>(ctx)) {
        dev->clearSsd1306();
    }
}
