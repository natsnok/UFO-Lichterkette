#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <stdint.h>

#define DATA_OUTPUT 2
#define LED_Count 260  // 40*3

#define LAMPEN_TOTAL 6
#define LED_proLampe 40

Adafruit_NeoPixel strip(LED_Count, DATA_OUTPUT, NEO_GRBW + NEO_KHZ800);

// VARIABLE PRESETS pls change here:

// COLOR presets:
// ___________________ R __ G __ B __ W __
byte defaultColor[4] = {100, 20, 2, 0};  // default
byte blitzColor_1[4] = {0, 0, 20, 100};  // flash first
byte blitzColor_2[4] = {0, 0, 15, 180};  // flash second
byte blitzColor_3[4] = {0, 0, 0, 100};   // flash  third
const int time_till_next_flash = 1000;   // dely till next flash

// AUSTAUSCHEN!!!
bool lampe_nr[] = {false, false, false, false, false, false};

bool lampe_0 = false;
bool lampe_1 = false;
bool lampe_2 = false;
bool lampe_3 = false;
bool lampe_4 = false;
bool lampe_5 = false;

int cnt_RandomColors =
    4;  // THE total number of "random" colors inside the switch
// Lampen nummer mit LED COUNT
const enum lampeNR : int {
    one = 0,
    two = LED_proLampe,
    three = LED_proLampe * 2,
    four = LED_proLampe * 3,
    five = LED_proLampe * 4,
    six = LED_proLampe * 5
};

// sColor -> selected Color
const enum sColor : byte { red = 0, green = 1, blue = 2, white = 3 };

// (V)isual (A)fter (F)lash - FinalVisualisColor!
// byte vaf_red, vaf_green, vaf_blue, vaf_white; //OLDVERSION!!!
byte vafColor[4] = {0, 0, 0, 0};

// UPDATE - has allready Updatet???
// bool u_red, u_green, u_blue, u_white; //OLDVERSION
bool updated[4] = {false, false, false, false};  // Werden die Gebraucht???

// COLOR SETUP...
struct RGBW {
    byte red;
    byte green;
    byte blue;
    byte white;
};

//_______________NEW RAND COLOR____________________________
byte randColor_spec(sColor colorSelect, byte minValue, byte maxValue) {
    byte newcolorValue = random(minValue, maxValue);  // calculate new
                                                      // Colorrange
    vafColor[colorSelect] = newcolorValue;            // selectFinalColor
    updated[colorSelect] = true;  // setFinalColor as Updatet
}

void randColorSpectum_base() {
    // ALWAYS UPDATE IF COLOR ADD!!!!!!!!!!!!!!!!!!!!!!
    int colorBase = random(0, 3);  // btw 0 und 3 !!!!
    switch (colorBase) {
        case 0:  // ROT 1
            randColor_spec(sColor::red, 150, 256);
            randColor_spec(sColor::green, 0, 50);
            randColor_spec(sColor::blue, 0, 50);
            randColor_spec(sColor::white, 0, 10);
            // update reset???
            break;

        case 1:  // GRÜN 1
            randColor_spec(sColor::red, 0, 50);
            randColor_spec(sColor::green, 150, 256);
            randColor_spec(sColor::blue, 0, 50);
            randColor_spec(sColor::white, 0, 10);
            // update reset???
            break;
        case 2:  // BLAU

            randColor_spec(sColor::red, 0, 50);
            randColor_spec(sColor::green, 0, 50);
            randColor_spec(sColor::blue, 150, 255);
            randColor_spec(sColor::white, 0, 10);
            // update reset???
            break;

        default:
            Serial.print("FUCK SHOULD NEVER ENCOUNT!!!!!!!!!! = " +
                         String(colorBase));
    }
    Serial.println("Zufällige farbe erfolgreich gewählt!");
}

void setColorAfterFlash() {}

//_______________OLD FLASH_______________________________
void setAllCollors(byte r, byte g, byte b, byte w) {
    for (int i = 0; i < LED_Count; i++) {
        strip.setPixelColor(i, strip.Color(r, g, b, w));
    }
    strip.show();
}

//________________Blitz Funktionen FLASHING...............
void blitz_start(byte von, byte bis) {
    for (int i = von; i < bis; i++) {
        strip.setPixelColor(i, strip.Color(blitzColor_1[sColor::red],
                                           blitzColor_1[sColor::green],
                                           blitzColor_1[sColor::blue],
                                           blitzColor_1[sColor::white]));
        strip.show();
        delay(5);
    }
    delay(100);
}
void blitz_flashen(byte von, byte bis) {
    for (int i = von; i < bis; i++) {
        strip.setPixelColor(i, strip.Color(blitzColor_2[sColor::red],
                                           blitzColor_2[sColor::green],
                                           blitzColor_2[sColor::blue],
                                           blitzColor_2[sColor::white]));
    }
    strip.show();
    delay(20);

    for (int i = von; i < bis; i++) {
        strip.setPixelColor(i, strip.Color(blitzColor_2[sColor::red],
                                           blitzColor_2[sColor::green],
                                           blitzColor_2[sColor::blue],
                                           blitzColor_2[sColor::white]));
    }
    strip.show();
    delay(30);

    for (int i = von; i < bis; i++) {
        strip.setPixelColor(i, strip.Color(blitzColor_2[sColor::red],
                                           blitzColor_2[sColor::green],
                                           blitzColor_2[sColor::blue],
                                           blitzColor_2[sColor::white]));
    }
    strip.show();
    delay(10);
}
void lampeReset_defaultColor(byte von, byte bis) {
    for (int i = von; i < bis; i++) {
        strip.setPixelColor(i, strip.Color(defaultColor[sColor::red],
                                           defaultColor[sColor::green],
                                           defaultColor[sColor::blue],
                                           defaultColor[sColor::white]));
    }
    strip.show();
}
void lampeRandColor(byte von, byte bis) {
    randColorSpectum_base();

    for (int i = von; i < bis; i++) {
        strip.setPixelColor(
            i, strip.Color(vafColor[sColor::red], vafColor[sColor::green],
                           vafColor[sColor::blue], vafColor[sColor::white]));
    }
    strip.show();
}

void lampeBlitz(byte welcheLampe) {
    int startAtLED = 0;
    switch (welcheLampe) {  // Select startPIXEL
        case 0:
            int startAtLED = lampeNR::one;
            break;
        case 1:
            int startAtLED = lampeNR::two;
            break;
        case 2:
            int startAtLED = lampeNR::three;
            break;
        case 3:
            int startAtLED = lampeNR::four;
            break;
        case 4:
            int startAtLED = lampeNR::five;
            break;
        case 5:
            int startAtLED = lampeNR::six;
            break;

        default:
            Serial.println("THIS SHOULD NEVER ENCOUNT !!!!!!! -> " +
                           String(welcheLampe) + " <- FUCK");
            break;
    }
    // ggf plus minus 1 muss ausgelotet werden!!!
    int endAtLED = startAtLED + LED_proLampe;
    blitz_start(startAtLED, endAtLED);
    blitz_flashen(startAtLED, endAtLED);
    // lampeReset_defaultColor(startAtLED, endAtLED);
    lampeRandColor(startAtLED, endAtLED);
}
void setup() {
    Serial.begin(9600);

    strip.begin();
    strip.clear();
    setAllCollors(defaultColor[sColor::red], defaultColor[sColor::green],
                  defaultColor[sColor::blue],
                  defaultColor[sColor::white]);  // Einstellung Grundfarbe

    Serial.println("Start Fertig");
}

void loop() {
    int updateLampeNR = rand() % 7;  // Witch Lampe? btw 0-6
    Serial.println("BLITZ start...  auf lampe ->" + String(updateLampeNR));

    lampeBlitz(updateLampeNR);

    Serial.println("Flash & update COMPLETE -> " + String(updateLampeNR));

    delay(time_till_next_flash);
}

/*

void blitz() {
    for (int i = 0; i < LED_Count; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 20, 100));
        strip.show();

        if (i >= 39 && lampe_0 == false) {
            setAllCollors(100, 20, 2, 0);  // Einstellung Grundfarbe
            lampe_0 = true;
            delay(500);
        }
        if (i >= 79 && lampe_1 == false) {
            setAllCollors(100, 20, 2, 0);  // Einstellung Grundfarbe

            lampe_1 = true;
            delay(500);
        }
        if (i >= 119 && lampe_2 == false) {
            setAllCollors(100, 20, 2, 0);  // Einstellung Grundfarbe
            lampe_2 = true;
            delay(500);
        }
    }

    delay(1000);

    lampe_0 = false;
    lampe_1 = false;
    lampe_2 = false;
    lampe_3 = false;
    lampe_4 = false;
    lampe_5 = false;
}

ALSTES SZWITCH STATEMENT...

    // //OLD FLASH .... Delite?
    // switch (welcheLampe) {
    //     case 0:
    //         blitz_start(0, 40);
    //         blitz_flashen(0, 40);
    //         lampeReset_defaultColor(0, 40);
    //         break;

    //     case 1:
    //         blitz_start(40, 80);
    //         blitz_flashen(40, 80);
    //         lampeReset_defaultColor(40, 80);
    //         break;

    //     case 2:
    //         blitz_start(80, 120);
    //         blitz_flashen(80, 120);
    //         lampeReset_defaultColor(80, 120);
    //         break;
    //     case 3:
    //         blitz_start(120, 160);
    //         blitz_flashen(120, 160);
    //         lampeReset_defaultColor(120, 160);
    //         break;
    //     case 4:
    //         blitz_start(160, 220);
    //         blitz_flashen(160, 220);
    //         lampeReset_defaultColor(160, 220);
    //         break;
    //     case 5:
    //         blitz_start(220, 260);
    //         blitz_flashen(220, 260);
    //         lampeReset_defaultColor(220, 260);
    //         break;
    // }



*/
