
struct State { };

// clang-format off
{{ GENERATED_CODE }}
// clang-format on

void evaluate(Context ctx) {
    auto scr = getValue<input_SCR>(ctx);
    if (isInputDirty<input_DO>(ctx)) {
        int16_t x = getValue<input_X1>(ctx);
        int16_t y = getValue<input_Y1>(ctx);
        int16_t x2 = getValue<input_X2>(ctx);
        int16_t y2 = getValue<input_Y2>(ctx);
        int16_t w = 1;
        int16_t h = 1;
        scr->dev->fixPlace(&x, &y, &w, &h);
        scr->dev->fixPlace(&x2, &y2, &w, &h);
        scr->dev->setChanged(x, x2 - x);
        int16_t X = 0, Y = 0, X2 = 0, Y2 = 0;
        if (x2 == x) {
            if (x >= 0 && x < scr->dev->width) {
                X = X2 = x;
                Y = (y > scr->dev->height) ? (scr->dev->height - 1) : ((y < 0) ? 0 : y);
                Y2 = (y2 > scr->dev->height) ? (scr->dev->height - 1)
                                             : ((y2 < 0) ? 0 : y2);
            }
        } else if (y2 == y) {
            if (y >= 0 && y < scr->dev->height) {
                Y = Y2 = y;
                X = (x > scr->dev->width) ? (scr->dev->width - 1) : ((x < 0) ? 0 : x);
                X2 = (x2 > scr->dev->width) ? (scr->dev->width - 1)
                                            : ((x2 < 0) ? 0 : x2);
            }
        } else if (x >= 0 && x < scr->dev->width && x2 >= 0 && x2 < scr->dev->width && y >= 0 && y < scr->dev->height && y2 >= 0 && y2 < scr->dev->height) {
            X = x;
            X2 = x2;
            Y = y;
            Y2 = y2;
        } else {
            int16_t a = (y2 - y) / (x2 - x);
            int16_t b = -y + x * a;
            int16_t yat0 = b;
            int16_t yath = a * (scr->dev->height - 1) + b;
            int16_t xat0 = -b / a;
            int16_t xatw = ((scr->dev->width - 1) - b) / a;
            if (yat0 >= 0 && yat0 <= scr->dev->height) {
                X = 0;
                Y = yat0;
            } else if (yath >= 0 && yath <= scr->dev->height) {
                X = scr->dev->width - 1;
                Y = yath;
            } else if (xat0 >= 0 && xat0 <= scr->dev->width) {
                X = xat0;
                Y = 0;
            } else if (xatw >= 0 && xatw <= scr->dev->height) {
                X = xatw;
                Y = scr->dev->height - 1;
            }

            if (xatw >= 0 && xatw <= scr->dev->height) {
                X = xatw;
                Y = scr->dev->height - 1;
            } else if (xat0 >= 0 && xat0 <= scr->dev->width) {
                X = xat0;
                Y = 0;
            } else if (yath >= 0 && yath <= scr->dev->height) {
                X = scr->dev->width - 1;
                Y = yath;
            } else if (yat0 >= 0 && yat0 <= scr->dev->height) {
                X = 0;
                Y = yat0;
            }
        }
        if (!(X == 0 && Y == 0 && X2 == 0 && Y2 == 0)) {
            scr->dev->storeUint8(2 /*DRAW_LINE*/);
            scr->dev->storeUint8((uint8_t)X);
            scr->dev->storeUint8((uint8_t)Y);
            scr->dev->storeUint8((uint8_t)X2);
            scr->dev->storeUint8((uint8_t)Y2);
            if (scr->dev->autoUpdate)
                scr->dev->render();
        }
        emitValue<output_DONE>(ctx, true);
    }
    emitValue<output_SCRU0027>(ctx, scr);
}
