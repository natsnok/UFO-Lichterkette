#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <stdint.h>

#define B_SET_COLLOR 3
#define LED_SET_COLLOR 2
#define LED_COUNT_SET_COLLOR 5

#define SetCollorLED_R 4
#define SetCollorLED_G 3
#define SetCollorLED_B 2
#define SetCollorLED_W 1
#define SetCollorLED_Preview 0

// Set Collor Potis_______
#define Poti_R A1
#define Poti_G A2
#define Poti_B A3
#define Poti_W A4

int raw_R, raw_G, raw_B, raw_W;
byte maped_R, maped_G, maped_B, maped_TimeLED;
int maped_Time;

// #define DATA_OUTPUT /* LICHTERKETTE*/
#define DATA_OUTPUT 12

#define LED_Count 260  // 40*3

#define LAMPEN_TOTAL 6
#define LED_proLampe 40
// #define BUTTON_A0 /* ALTER BUTTON*/
#define BUTTON_A0 6

#define BUTTON_plus 8
#define BUTTON_min 7

#define potiR A7
#define potiG A3
#define potiB A4
#define potiW A5
#define potiSpeed A6

byte modus = 0;

// Wieder rein für LICHTERKETTE
Adafruit_NeoPixel strip(LED_Count, DATA_OUTPUT, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel stripSetColor(LED_COUNT_SET_COLLOR, LED_SET_COLLOR,
                                NEO_GRBW + NEO_KHZ800);

class Button {
   private:
    byte PIN_BUTTON;
    bool oldstate_PRESS;
    bool oldstate_DOWN;

   public:
    Button(byte pin_button_import) { PIN_BUTTON = pin_button_import; }

    bool press() {
        bool newstate = digitalRead(PIN_BUTTON);
        bool down = (oldstate_PRESS == LOW && newstate == HIGH);
        oldstate_PRESS = newstate == HIGH;
        return down;
    }

    bool hold() {
        bool newstate = digitalRead(PIN_BUTTON);
        if (newstate == true) {
            return newstate;
            Serial.print("JEPP");
        }
        return;
    }

    bool down() {
        bool press = digitalRead(PIN_BUTTON);
        if (press == HIGH) {
            delay(50);
            press = oldstate_DOWN;
            press = digitalRead(PIN_BUTTON);
            Serial.println("C");
            if (press == LOW) {
                Serial.println("D");
                return oldstate_DOWN;
            }
            return;
        }
    }
};
// GGF wiederrein für schalter...
Button button_A0 = Button(BUTTON_A0);
Button b_SetCollor = Button(B_SET_COLLOR);

// VARIABLE PRESETS pls change here:

// COLOR presets:
// ___________________ R __ G __ B __ W __
byte defaultColor[4] = {100, 20, 2, 0};  // default
byte blitzColor_1[4] = {0, 0, 20, 100};  // flash first
byte blitzColor_2[4] = {0, 0, 15, 180};  // flash second
byte blitzColor_3[4] = {0, 0, 0, 100};   // flash  third
int time_till_next_flash = 1000;         // dely till next flash

byte savedCollor_Lampe1[4] = {};
byte savedCollor_Lampe2[4] = {};
byte savedCollor_Lampe3[4] = {};
byte savedCollor_Lampe4[4] = {};
byte savedCollor_Lampe5[4] = {};
byte savedCollor_Lampe6[4] = {};

// AUSTAUSCHEN!!!
bool lampe_nr[] = {false, false, false, false, false, false};

bool lampe_0 = false;
bool lampe_1 = false;
bool lampe_2 = false;
bool lampe_3 = false;
bool lampe_4 = false;
bool lampe_5 = false;

unsigned long time;
unsigned long timeOld = 0;
// unsigned long delayTime = 0;

int cnt_RandomColors = 4;  // THE total number of "random" colors inside the
                           // switch Lampen nummer mit LED COUNT
enum lampeNR : int {
    one = 0,
    two = LED_proLampe,
    three = LED_proLampe * 2,
    four = LED_proLampe * 3,
    five = LED_proLampe * 4,
    six = LED_proLampe * 5
};

// sColor -> selected Color
enum sColor : byte { red = 0, green = 1, blue = 2, white = 3 };

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

//_______________OLD FLASH_______________________________
void setAllCollors(byte r, byte g, byte b, byte w) {
    for (int i = 0; i < LED_Count; i++) {
        strip.setPixelColor(i, strip.Color(r, g, b, w));
    }
    strip.show();
}

//_______________NEW RAND COLOR____________________________
byte randColor_spec(sColor colorSelect, byte minValue, byte maxValue) {
    byte newcolorValue = random(minValue, maxValue);  // calculate new
                                                      // Colorrange
    vafColor[colorSelect] = newcolorValue;            // selectFinalColor
    updated[colorSelect] = true;  // setFinalColor as Updatet
}

byte wechsel = 0;
byte wechsel2 = 0;
void randColorSpectum_base() {
    // ALWAYS UPDATE IF COLOR ADD!!!!!!!!!!!!!!!!!!!!!!
    Serial.println(" NUMMER = " + String(modus));

    switch (modus) {
        case 0:  // Standat Farben

            setAllCollors(
                defaultColor[sColor::red], defaultColor[sColor::green],
                defaultColor[sColor::blue], defaultColor[sColor::white]);

            // Serial.println(red_NOW);

            break;
        case 1:
            switch (wechsel) {
                case 0:  // ROT 1
                    randColor_spec(sColor::red, 180, 255);
                    randColor_spec(sColor::green, 0, 255);
                    randColor_spec(sColor::blue, 0, 0);
                    randColor_spec(sColor::white, 0, 0);
                    wechsel2++;
                    if (wechsel2 >= 500) {
                        wechsel++;
                        wechsel2 = 0;
                    };
                    // update reset???
                    break;

                case 1:                                 // GRÜN 1
                    randColor_spec(sColor::red, 0, 0);  // 50
                    randColor_spec(sColor::green, 150, 255);
                    randColor_spec(sColor::blue, 0, 255);
                    randColor_spec(sColor::white, 0, 0);
                    wechsel2++;
                    if (wechsel2 >= 500) {
                        wechsel++;
                        wechsel2 = 0;
                    };
                    // update reset???
                    break;
                case 2:  // BLAU
                    randColor_spec(sColor::red, 0, 255);
                    randColor_spec(sColor::green, 0, 0);
                    randColor_spec(sColor::blue, 190, 255);
                    randColor_spec(sColor::white, 0, 0);
                    wechsel2++;
                    if (wechsel2 >= 500) {
                        wechsel++;
                        wechsel2 = 0;
                    };

                    // update reset???
                    break;
                case 3:  // ROT 1
                    randColor_spec(sColor::red, 180, 255);
                    randColor_spec(sColor::green, 0, 50);
                    randColor_spec(sColor::blue, 0, 0);
                    randColor_spec(sColor::white, 0, 0);
                    wechsel2++;
                    if (wechsel2 >= 500) {
                        wechsel++;
                        wechsel2 = 0;
                    };

                    // update reset???
                    break;

                case 4:                                   // GRÜN 1
                    randColor_spec(sColor::red, 0, 150);  // 50
                    randColor_spec(sColor::green, 150, 255);
                    randColor_spec(sColor::blue, 0, 0);
                    randColor_spec(sColor::white, 0, 0);
                    wechsel2++;
                    if (wechsel2 >= 500) {
                        wechsel++;
                        wechsel2 = 0;
                    };

                    // update reset???
                    break;
                case 5:  // BLAU
                    randColor_spec(sColor::red, 0, 50);
                    randColor_spec(sColor::green, 0, 150);
                    randColor_spec(sColor::blue, 190, 255);
                    randColor_spec(sColor::white, 0, 0);
                    // update reset???
                    wechsel2++;
                    if (wechsel2 >= 500) {
                        wechsel = 0;
                        wechsel2 = 0;
                    };
                    break;
            }

            break;
        case 2:  // ROT 1
            randColor_spec(sColor::red, 180, 255);
            randColor_spec(sColor::green, 0, 255);
            randColor_spec(sColor::blue, 0, 0);
            randColor_spec(sColor::white, 0, 0);
            // update reset???
            break;

        case 3:                                 // GRÜN 1
            randColor_spec(sColor::red, 0, 0);  // 50
            randColor_spec(sColor::green, 150, 255);
            randColor_spec(sColor::blue, 0, 255);
            randColor_spec(sColor::white, 0, 0);
            // update reset???
            break;
        case 4:  // BLAU
            randColor_spec(sColor::red, 0, 255);
            randColor_spec(sColor::green, 0, 0);
            randColor_spec(sColor::blue, 190, 255);
            randColor_spec(sColor::white, 0, 0);
            // update reset???
            break;
        case 5:  // ROT 1
            randColor_spec(sColor::red, 180, 255);
            randColor_spec(sColor::green, 0, 50);
            randColor_spec(sColor::blue, 0, 0);
            randColor_spec(sColor::white, 0, 0);
            // update reset???
            break;

        case 6:                                   // GRÜN 1
            randColor_spec(sColor::red, 0, 150);  // 50
            randColor_spec(sColor::green, 150, 255);
            randColor_spec(sColor::blue, 0, 0);
            randColor_spec(sColor::white, 0, 0);
            // update reset???
            break;
        case 7:  // BLAU
            randColor_spec(sColor::red, 0, 50);
            randColor_spec(sColor::green, 0, 150);
            randColor_spec(sColor::blue, 190, 255);
            randColor_spec(sColor::white, 0, 0);
            // update reset???
            break;

        default:
            Serial.print("FUCK SHOULD NEVER ENCOUNT ");
    }
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
        strip.setPixelColor(i, strip.Color(blitzColor_3[sColor::red],
                                           blitzColor_3[sColor::green],
                                           blitzColor_3[sColor::blue],
                                           blitzColor_3[sColor::white]));
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
void blitz_flashenPLUS(byte von, byte bis) {
    for (int i = von; i < bis; i++) {
        strip.setPixelColor(i, strip.Color(blitzColor_1[sColor::red],
                                           blitzColor_1[sColor::green],
                                           blitzColor_1[sColor::blue],
                                           blitzColor_1[sColor::white]));
    }
    strip.show();
    delay(20);
    for (int i = von; i < bis; i++) {
        strip.setPixelColor(i, strip.Color(blitzColor_3[sColor::red],
                                           blitzColor_3[sColor::green],
                                           blitzColor_3[sColor::blue],
                                           blitzColor_3[sColor::white]));
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
void lampeReset_setControllerColor(byte von, byte bis) {
    for (int i = von; i < bis; i++) {
        strip.setPixelColor(i, strip.Color(maped_R, maped_G, maped_B, 0));
    }
    strip.show();
}
void lampeReset_setControllerColor_toSavedValue(byte von, byte bis, byte _r,
                                                byte _g, byte _b, byte _w) {
    for (int i = von; i < bis; i++) {
        strip.setPixelColor(i, strip.Color(_r, _g, _b, _w));
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
            startAtLED = lampeNR::one;
            break;
        case 1:
            startAtLED = lampeNR::two;
            break;
        case 2:
            startAtLED = lampeNR::three;
            break;
        case 3:
            startAtLED = lampeNR::four;
            break;
        case 4:
            startAtLED = lampeNR::five;
            break;
        case 5:
            startAtLED = lampeNR::six;
            break;

        default:
            Serial.println("THIS SHOULD NEVER ENCOUNT ");
            break;
    }
    // ggf plus minus 1 muss ausgelotet werden!!!
    int endAtLED = startAtLED + LED_proLampe;
    blitz_start(startAtLED, endAtLED);
    blitz_flashen(startAtLED, endAtLED);

    int addLightning = rand() % 3;  // Witch Lampe? btw 0-6
    if (addLightning == 0) {
        blitz_flashenPLUS(startAtLED, endAtLED);
        blitz_flashen(startAtLED, endAtLED);

        int addLightning2 = rand() % 2;
        if (addLightning2 == 0) {
            blitz_flashenPLUS(startAtLED, endAtLED);
            delay(150);
            blitz_flashenPLUS(startAtLED, endAtLED);
        }
    }
    if (addLightning == 1) {
        blitz_flashenPLUS(startAtLED, endAtLED);
        blitz_flashen(startAtLED, endAtLED);
        delay(150);
        blitz_flashenPLUS(startAtLED, endAtLED);
        blitz_flashen(startAtLED, endAtLED);

        int addLightning2 = rand() % 2;
        if (addLightning2 == 0) {
            blitz_flashenPLUS(startAtLED, endAtLED);
            blitz_flashen(startAtLED, endAtLED);
            blitz_flashenPLUS(startAtLED, endAtLED);
            delay(150);
            blitz_flashen(startAtLED, endAtLED);
            blitz_flashenPLUS(startAtLED, endAtLED);
            delay(100);
            blitz_flashen(startAtLED, endAtLED);
            blitz_flashenPLUS(startAtLED, endAtLED);
        }
    }

    delay(20);
    // lampeReset_defaultColor(startAtLED, endAtLED);
    if (modus != 0)
        lampeRandColor(startAtLED, endAtLED);
    else {
        // byte red_NOW = analogRead(potiR);
        // byte green_NOW = analogRead(potiG);
        // byte blue_NOW = analogRead(potiB);
        // byte white_NOW = analogRead(potiW);
        // byte speed_NOW = analogRead(potiSpeed);

        lampeReset_defaultColor(startAtLED, endAtLED);
    }
}
void lampeBlitz_SetControllerCollor_OLD(byte welcheLampe, bool firstTime) {
    int startAtLED = 0;
    switch (welcheLampe) {  // Select startPIXEL
        case 0:
            startAtLED = lampeNR::one;
            savedCollor_Lampe1[0] = maped_R;
            savedCollor_Lampe1[1] = maped_G;
            savedCollor_Lampe1[2] = maped_B;
            savedCollor_Lampe1[3] = 0;
            break;
        case 1:
            startAtLED = lampeNR::two;
            savedCollor_Lampe2[0] = maped_R;
            savedCollor_Lampe2[1] = maped_G;
            savedCollor_Lampe2[2] = maped_B;
            savedCollor_Lampe2[3] = 0;
            break;
        case 2:
            startAtLED = lampeNR::three;
            savedCollor_Lampe3[0] = maped_R;
            savedCollor_Lampe3[1] = maped_G;
            savedCollor_Lampe3[2] = maped_B;
            savedCollor_Lampe3[3] = 0;
            break;
        case 3:
            startAtLED = lampeNR::four;
            savedCollor_Lampe4[0] = maped_R;
            savedCollor_Lampe4[1] = maped_G;
            savedCollor_Lampe4[2] = maped_B;
            savedCollor_Lampe4[3] = 0;
            break;
        case 4:
            startAtLED = lampeNR::five;
            savedCollor_Lampe5[0] = maped_R;
            savedCollor_Lampe5[1] = maped_G;
            savedCollor_Lampe5[2] = maped_B;
            savedCollor_Lampe5[3] = 0;
            break;
        case 5:
            startAtLED = lampeNR::six;
            savedCollor_Lampe6[0] = maped_R;
            savedCollor_Lampe6[1] = maped_G;
            savedCollor_Lampe6[2] = maped_B;
            savedCollor_Lampe6[3] = 0;
            break;

        default:
            Serial.println("THIS SHOULD NEVER ENCOUNT ");
            break;
    }
    // ggf plus minus 1 muss ausgelotet werden!!!
    int endAtLED = startAtLED + LED_proLampe;
    blitz_start(startAtLED, endAtLED);
    blitz_flashen(startAtLED, endAtLED);

    int addLightning = rand() % 3;  // Witch Lampe? btw 0-6
    if (addLightning == 0) {
        blitz_flashenPLUS(startAtLED, endAtLED);
        blitz_flashen(startAtLED, endAtLED);

        int addLightning2 = rand() % 2;
        if (addLightning2 == 0) {
            blitz_flashenPLUS(startAtLED, endAtLED);
            delay(150);
            blitz_flashenPLUS(startAtLED, endAtLED);
        }
    }
    if (addLightning == 1) {
        blitz_flashenPLUS(startAtLED, endAtLED);
        blitz_flashen(startAtLED, endAtLED);
        delay(150);
        blitz_flashenPLUS(startAtLED, endAtLED);
        blitz_flashen(startAtLED, endAtLED);

        int addLightning2 = rand() % 2;
        if (addLightning2 == 0) {
            blitz_flashenPLUS(startAtLED, endAtLED);
            blitz_flashen(startAtLED, endAtLED);
            blitz_flashenPLUS(startAtLED, endAtLED);
            delay(150);
            blitz_flashen(startAtLED, endAtLED);
            blitz_flashenPLUS(startAtLED, endAtLED);
            delay(100);
            blitz_flashen(startAtLED, endAtLED);
            blitz_flashenPLUS(startAtLED, endAtLED);
        }
    }
    delay(20);
    if (firstTime) {
        lampeReset_setControllerColor(startAtLED, endAtLED);
    } else {
        switch (welcheLampe) {  // Select startPIXEL
            case 0:
                lampeReset_setControllerColor_toSavedValue(
                    startAtLED, endAtLED, savedCollor_Lampe1[sColor::red],
                    savedCollor_Lampe1[sColor::green],
                    savedCollor_Lampe1[sColor::blue],
                    savedCollor_Lampe1[sColor::white]);
                break;
            case 1:
                lampeReset_setControllerColor_toSavedValue(
                    startAtLED, endAtLED, savedCollor_Lampe2[sColor::red],
                    savedCollor_Lampe2[sColor::green],
                    savedCollor_Lampe2[sColor::blue],
                    savedCollor_Lampe2[sColor::white]);
                break;
            case 2:
                lampeReset_setControllerColor_toSavedValue(
                    startAtLED, endAtLED, savedCollor_Lampe3[sColor::red],
                    savedCollor_Lampe3[sColor::green],
                    savedCollor_Lampe3[sColor::blue],
                    savedCollor_Lampe3[sColor::white]);
                break;
            case 3:
                lampeReset_setControllerColor_toSavedValue(
                    startAtLED, endAtLED, savedCollor_Lampe4[sColor::red],
                    savedCollor_Lampe4[sColor::green],
                    savedCollor_Lampe4[sColor::blue],
                    savedCollor_Lampe4[sColor::white]);
                break;
            case 4:
                lampeReset_setControllerColor_toSavedValue(
                    startAtLED, endAtLED, savedCollor_Lampe5[sColor::red],
                    savedCollor_Lampe5[sColor::green],
                    savedCollor_Lampe5[sColor::blue],
                    savedCollor_Lampe5[sColor::white]);
                break;
            case 5:
                lampeReset_setControllerColor_toSavedValue(
                    startAtLED, endAtLED, savedCollor_Lampe6[sColor::red],
                    savedCollor_Lampe6[sColor::green],
                    savedCollor_Lampe6[sColor::blue],
                    savedCollor_Lampe6[sColor::white]);
                break;

            default:
                Serial.println("THIS SHOULD NEVER ENCOUNT ");
                break;
        }
    }
}
void lampeBlitz_SetControllerCollor_ALLE(byte welcheLampe, bool firstTime) {
    int startAtLED = 0;
    switch (welcheLampe) {  // Select startPIXEL
        case 0:
            startAtLED = lampeNR::one;
            break;
        case 1:
            startAtLED = lampeNR::two;
            break;
        case 2:
            startAtLED = lampeNR::three;
            break;
        case 3:
            startAtLED = lampeNR::four;
            break;
        case 4:
            startAtLED = lampeNR::five;
            break;
        case 5:
            startAtLED = lampeNR::six;
            break;

        default:
            Serial.println("THIS SHOULD NEVER ENCOUNT ");
            break;
    }
    // ggf plus minus 1 muss ausgelotet werden!!!
    int endAtLED = startAtLED + LED_proLampe;
    blitz_start(startAtLED, endAtLED);
    blitz_flashen(startAtLED, endAtLED);

    stripSetColor.setPixelColor(SetCollorLED_Preview, strip.Color(0, 0, 0, 0));
    stripSetColor.show();
    int addLightning = rand() % 3;  // Witch Lampe? btw 0-6
    if (addLightning == 0) {
        blitz_flashenPLUS(startAtLED, endAtLED);
        blitz_flashen(startAtLED, endAtLED);

        int addLightning2 = rand() % 2;
        if (addLightning2 == 0) {
            blitz_flashenPLUS(startAtLED, endAtLED);
            delay(150);
            blitz_flashenPLUS(startAtLED, endAtLED);
        }
    }
    if (addLightning == 1) {
        blitz_flashenPLUS(startAtLED, endAtLED);
        blitz_flashen(startAtLED, endAtLED);
        delay(150);
        blitz_flashenPLUS(startAtLED, endAtLED);
        blitz_flashen(startAtLED, endAtLED);

        int addLightning2 = rand() % 2;
        if (addLightning2 == 0) {
            blitz_flashenPLUS(startAtLED, endAtLED);
            blitz_flashen(startAtLED, endAtLED);
            blitz_flashenPLUS(startAtLED, endAtLED);
            delay(150);
            blitz_flashen(startAtLED, endAtLED);
            blitz_flashenPLUS(startAtLED, endAtLED);
            delay(100);
            blitz_flashen(startAtLED, endAtLED);
            blitz_flashenPLUS(startAtLED, endAtLED);
        }
    }
    delay(20);
    lampeReset_setControllerColor(startAtLED, endAtLED);
}

void readPotis() {
    int raw_R = analogRead(Poti_R);
    int raw_G = analogRead(Poti_G);
    int raw_B = analogRead(Poti_B);
    int raw_Time = analogRead(Poti_W);
    maped_R = map(raw_R, 0, 1023, 0, 200);
    maped_G = map(raw_G, 0, 1023, 0, 200);
    maped_B = map(raw_B, 0, 1023, 0, 200);
    // delayTime = map(raw_Time, 0, 1023, 0, 700);
    maped_Time = map(raw_Time, 0, 1023, 0, 255);
    //  maped_TimeLED = map(raw_Time, 0, 1023, 0, 200);
    // delayTime = maped_Time;
    // Serial.println("R = " + String(raw_R) + " | G = " + String(raw_G) +
    //                " | B = " + String(raw_B) + " | W = " + String(raw_W));

    Serial.println("RAW = " + String(maped_Time) +
                   "maped= " + String(raw_Time));
}

void setup_Controller() {}
void setup() {
    Serial.begin(9600);
    strip.begin();
    strip.clear();

    // SETUP CONTROLER ...
    pinMode(B_SET_COLLOR, INPUT_PULLUP);
    stripSetColor.begin();
    stripSetColor.show();
    //...setup controler

    pinMode(BUTTON_A0, INPUT_PULLUP);
    pinMode(BUTTON_plus, INPUT_PULLUP);
    pinMode(BUTTON_min, INPUT_PULLUP);

    setAllCollors(defaultColor[sColor::red], defaultColor[sColor::green],
                  defaultColor[sColor::blue],
                  defaultColor[sColor::white]);  // Einstellung Grundfarbe

    // Serial.println("Start Fertig");
}

bool blinkeLED = true;

// ALTE Steuerung aus papier!!!!!!
//  void readButtonAndSpeed() {
//      if (button_A0.press()) {
//          modus++;
//          if (modus >= 8) modus = 0;
//          Serial.println("Modus Knopf ... modus = " + String(modus));
//      }
//     if (digitalRead(BUTTON_plus) == HIGH) {
//         Serial.println("min BUTTON" + String(delayTime));
//         delayTime = delayTime + 100;
//         // delayTime++;  // = delayTime + 1000;
//         if (delayTime >= 60000) delayTime = 60000;
//     }
//     if (digitalRead(BUTTON_min) == HIGH) {
//         Serial.println("max BUTTON" + String(delayTime));
//         delayTime = delayTime - 100;
//         // delayTime--;  // = delayTime - 1000;
//         if (delayTime <= 1001 || delayTime >= 60000) delayTime = 0;
//     }
// }
void loop() {
    // CONTROLLER REFRESH...

    //...controler refresh

    for (int i = 0; i < 240; i++) {
        time = millis();

        if (time - timeOld > maped_Time) {
            timeOld = time;
            //    Serial.println("map= " + String(maped_Time));

            blinkeLED = !blinkeLED;
            if (blinkeLED) {
                //   Serial.println("AN" + String(blinkeLED));
                stripSetColor.setPixelColor(SetCollorLED_W,
                                            strip.Color(0, 0, 0, 100));
            } else {
                //  Serial.println("aus" + String(blinkeLED));

                stripSetColor.setPixelColor(SetCollorLED_W,
                                            strip.Color(0, 0, 0, 0));
            }
            stripSetColor.show();
            // Serial.println("LED= " + String(i));
            strip.setPixelColor(i, strip.Color(maped_R, maped_G, maped_B, 0));
            strip.show();
        }

        readPotis();
        stripSetColor.setPixelColor(SetCollorLED_R,
                                    strip.Color(maped_R, 0, 0, 0));
        stripSetColor.setPixelColor(SetCollorLED_G,
                                    strip.Color(0, maped_G, 0, 0));
        stripSetColor.setPixelColor(SetCollorLED_B,
                                    strip.Color(0, 0, maped_B, 0));

        stripSetColor.setPixelColor(SetCollorLED_Preview,
                                    strip.Color(maped_R, maped_G, maped_B, 0));
        stripSetColor.show();

        if (b_SetCollor.press()) {
            int updateLampeNR = rand() % 6;  // which Lampe? btw 0-6
            // Serial.println("Lampe nr = " + String(updateLampeNR));
            lampeBlitz_SetControllerCollor_ALLE(updateLampeNR, true);
        }
    }

    // if BUTTON -> set RANDOM LAMPE ...

    // time = millis();
    // if (time - timeOld > delayTime) {
    //     int updateLampeNR = rand() % 6;  // which Lampe? btw 0-6
    //     lampeBlitz_SetControllerCollor_ALLE(updateLampeNR, false);

    //     time = millis();
    //     timeOld = time;
    // }

    // ORGINAL OLD LOOP....
    //  time = millis();
    //  readButtonAndSpeed();
    //  if (time - timeOld > delayTime) {
    //      int updateLampeNR = rand() % 6;  // Witch Lampe? btw 0-6
    //      lampeBlitz(updateLampeNR);

    //     time = millis();
    //     timeOld = time;
    // }
}
