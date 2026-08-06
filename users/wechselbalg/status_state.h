#pragma once

/*
Der *Zustand*, den eine Statusanzeige zeigt -- ohne jede Hardware.

Herausgezogen am 2026-08-06 aus status_led.c, weil es seitdem zwei Anzeigen
gibt, die genau dasselbe meinen und dieselbe Palette benutzen sollen:

  - die WS2812 an GP25 der Liatris-Sofle (status_led.c, eigener PIO-Treiber)
  - der seitliche Lichtbalken der GMMK Pro (16 LEDs ohne Matrixposition,
    umschaltbar zwischen Animation und Status)

Die Palette gehoert an *eine* Stelle: wer die Farben nachjustiert, soll das
nicht zweimal tun muessen und nicht damit rechnen, dass die Boards
auseinanderlaufen.

Eingeschaltet wird die Datei nicht direkt, sondern von den Features, die sie
brauchen (users/wechselbalg/rules.mk).
*/

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} wb_status_color_t;

#define WB_SC(r_, g_, b_) ((wb_status_color_t){(r_), (g_), (b_)})

// Momentzustaende, absteigend nach Dringlichkeit -- Caps Word dauert einen
// Augenblick und muss jetzt gesehen werden, der Jiggler laeuft stundenlang.
#define WB_SC_CAPS_WORD  WB_SC(255, 255, 255)
#define WB_SC_LAYER_LOCK WB_SC(  0, 255, 255)  // dasselbe Cyan wie die Taste
#define WB_SC_JIGGLER    WB_SC(170, 255,   0)

/*
Ruhe: kein Zustand an. Dann leuchtet die Anzeige nur noch, wenn der Mac-Modus
an ist -- am PC ist sie aus (Michael, 2026-08-04).

Damit ist "sie leuchtet ueberhaupt" schon die halbe Information, und das Board
ist im Normalfall dunkel statt dauerhaft mit einem Punkt zu leuchten, der
nichts sagt.
*/
#define WB_SC_MAC WB_SC(0, 120, 255)
#define WB_SC_OFF WB_SC(0, 0, 0)

enum wb_status_flag {
    WB_STATUS_CAPS_WORD  = 1 << 0,
    WB_STATUS_LAYER_LOCK = 1 << 1,
    WB_STATUS_JIGGLER    = 1 << 2,
    WB_STATUS_MAC        = 1 << 3,
};

// Die Flags dieser Haelfte/dieses Boards, frisch abgefragt.
uint8_t wb_status_flags_local(void);

/*
Die Farbe zu einem Flag-Satz, Momentzustaende vor Dauerzustaenden.
`is_idle` meldet zurueck, ob es der blosse Ruhezustand ist (Mac-Hinweis oder
aus) -- der Mac-Hinweis wird auf der Sofle zusaetzlich gedaempft, weil er
dauerhaft steht. Muss ein gueltiger Zeiger sein.
*/
wb_status_color_t wb_status_color(uint8_t flags, bool *is_idle);
