#include <Adafruit_NeoPixel.h>

/* ------ Pin Definitions ------ */

// Single-channel strip outputs (behind desk)
#define PO_B 9
#define PO_R 10
#define PO_G 3

// Neopixel strip outputs (around screens, above desk)
#define PO_STRIP_L 5
#define PO_STRIP_R 6

// Potentiometer knob analog inputs
#define PI_POT_0 A6
#define PI_POT_1 A7
#define PI_POT_2 A5
#define PI_POT_3 A4

// IR receiver
#define PI_IR 11

/* ------ Predefined Colours ------ */
/*
  Preset colours, as ordered below:
  black/off,
  sunset orange,
  morning green,
  refreshing turqoise,
  edgy pink
*/

uint32_t colors[6] = {
  0x00000000,
  0xFF000000,
  0x2AFF0F00,
  0x14FFFF00,
  0x260AFF2A,
  0x14FF00EE,
};

uint8_t n_colors = 6;

/* ------ Knobs ------ */

uint8_t pots[4] = { PI_POT_0, PI_POT_1, PI_POT_2, PI_POT_3 };
uint8_t pot_positions[4] = {0, 0, 0, 0};

uint8_t n_extras = 0; // Number of additional modes registered to knob positions
uint16_t step_size = 1023/(n_colors + n_extras - 1);

void updatePotPositions() {
  for (int i = 0; i < 4; i++) {
    uint16_t raw = analogRead(pots[i]);
    pot_positions[i] = raw/step_size;

  }
}

/* ------ Colour Functions ------ */
// Extract specified component of 32-bit colour value

uint8_t whiteOf(uint32_t color)
{
  return (color >> 24) & 0xFF;
}

uint8_t redOf(uint32_t color, bool rgb=false)
{
  return rgb ? (color >> 24) & 0xFF :  (color >> 16) & 0xFF;
}

uint8_t greenOf(uint32_t color, bool rgb=false)
{
  return rgb ? (color >> 16) & 0xFF :  (color >> 8) & 0xFF;
}

uint8_t blueOf(uint32_t color, bool rgb=false)
{
  return rgb ? (color >> 8) & 0xFF :  color & 0xFF;
}

/* ------ Strip Classes ------ */
// OO method of strip initialisation

class AugPixel : public Adafruit_NeoPixel
{
  // "Smart" individually-addressable strip
  public:
    AugPixel(uint16_t pixels, uint8_t pin, uint8_t type) : Adafruit_NeoPixel(pixels, pin, type) { }

    void SetRange(uint16_t range[2], uint32_t color)
    {
      for (int i = range[0]; i <= range[1]; i++)
      {
        setPixelColor(i, color);
      }
    }

};

class DumbPixel
{
  // "Dumb" single-channel strip
  public:
    uint8_t pin_red;
    uint8_t pin_green;
    uint8_t pin_blue;

    uint8_t cur_red;
    uint8_t cur_green;
    uint8_t cur_blue;

    DumbPixel(uint8_t pin_r, uint8_t pin_g, uint8_t pin_b)
    {
      pin_red = pin_r;
      pin_green = pin_g;
      pin_blue = pin_b;
    }

    void SetColor(uint32_t color)
    {
      SetColor(redOf(color),greenOf(color),blueOf(color));
    }

    void SetColor(uint8_t r, uint8_t g, uint8_t b)
    {
      cur_red = r;
      cur_green = g;
      cur_blue = b;
    }

    void show()
    {
      analogWrite(pin_red, cur_red);
      analogWrite(pin_green, cur_green);
      analogWrite(pin_blue, cur_blue);
    }
};

/* ------ Strips ------ */

AugPixel strip_l = AugPixel(92, PO_STRIP_L, NEO_GRBW + NEO_KHZ800);
AugPixel strip_r = AugPixel(67, PO_STRIP_R, NEO_GRBW + NEO_KHZ800);
DumbPixel strip_c = DumbPixel(PO_R, PO_G, PO_B);

/* ------ Strip Zones ------ */

uint16_t zone_desk_l[2] = {0, 91};
uint16_t zone_desk_r[2] = {0, 66};
uint16_t zone_rear_l[2] = {32, 59};
uint16_t zone_rear_r[2] = {32, 46};

/* ------ Setter Functions ------ */

void setup()
{
  Serial.begin(9600);
  Serial.println(step_size);
  pinMode(PO_R, OUTPUT);
  pinMode(PO_G, OUTPUT);
  pinMode(PO_B, OUTPUT);
  pinMode(PO_STRIP_L, OUTPUT);
  pinMode(PO_STRIP_R, OUTPUT);
}

void loop()
{
  updatePotPositions();

  for (int i = 4 - 1; i >= 0; i--)
  {
    uint32_t tempCol = 0;
    if (pot_positions[i] < n_colors)
    {
       tempCol = colors[pot_positions[i]];
    }
    // TODO: Extra functionality

    switch(i) {
      case 1:
        strip_l.SetRange(zone_rear_l, tempCol);
        break;
      case 2:
        strip_r.SetRange(zone_rear_r, tempCol);
        break;
      case 3:
        strip_l.SetRange(zone_desk_l, tempCol);
        strip_r.SetRange(zone_desk_r, tempCol);
        break;
      case 0:
      default:
        strip_c.SetColor(tempCol);
    }
  }

  strip_l.show();
  strip_r.show();
  strip_c.show();
}
