struct State { };

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    if (isSettingUp()) {
        emitValue<output_C>(ctx, 32766);
    }
}
