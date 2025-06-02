//Ali Rashidi - t.me/WriteYourWay

#include <Adafruit_NeoPixel.h>
#include <IRremote.h>

#define IR_PIN      11
#define LED_PIN     10
#define NUMPIXELS   32 //160

// IR Remote Codes
#define BTN_0      0xFF9867
#define BTN_1      0xFFA25D
#define BTN_2      0xFF629D
#define BTN_3      0xFFE21D
#define BTN_4      0xFF22DD
#define BTN_5      0xFF02FD
#define BTN_6      0xFFC23D
#define BTN_7      0xFFE01F
#define BTN_8      0xFFA857
#define BTN_9      0xFF906F
#define BTN_OK     0xFF38C7
#define BTN_UP     0xFF18E7
#define BTN_DOWN   0xFF4AB5
#define BTN_LEFT   0xFF10EF
#define BTN_RIGHT  0xFF5AA5
#define BTN_star   0xFF6897
#define BTN_hash   0xFFB04F

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
IRrecv irrecv(IR_PIN);
decode_results results;

bool isPowered = true;
uint16_t delayTime = 50;
int currentEffect = -1;  // Variable to track the current effect
unsigned long lastEffectTime = 0; // Time tracking for effect loop

// Debounce variables
unsigned long lastButtonPress = 0;  // Last time a button was pressed
unsigned long debounceDelay = 100;  // Debounce delay in milliseconds

// Variables for color and brightness control
uint32_t currentColor = 0xFFFFFF;  // Initial color (white)
uint8_t currentBrightness = 255;   // Initial brightness (max)

void setup() {
  pixels.begin();
  pixels.clear();
  pixels.show();
  irrecv.enableIRIn();
}

void loop() {
  if (irrecv.decode(&results)) {
    // Only process the button press if the debounce time has passed
    if (millis() - lastButtonPress > debounceDelay) {
      handleIRCommand(results.value);
      lastButtonPress = millis();  // Update last button press time
    }
    irrecv.resume();
  }

  // Continuously run the current effect if it's active
  if (currentEffect != -1) {
    executeEffect();
  }
}

void handleIRCommand(uint32_t command) {
  // Toggle power (turn off all LEDs if power is turned off)
  if (command == BTN_OK) {
    isPowered = !isPowered;
    if (!isPowered) {
      pixels.clear();
      pixels.show();
    }
  } else if (command == BTN_LEFT) {
    // Change color left (previous color)
    changeColor(-1);
  } else if (command == BTN_RIGHT) {
    // Change color right (next color)
    changeColor(1);
  } else if (command == BTN_UP) {
    // Increase brightness
    changeBrightness(10);
  } else if (command == BTN_DOWN) {
    // Decrease brightness
    changeBrightness(-10);
  } else {
    // Run selected effect
    runEffect(command);
  }
}

void runEffect(uint32_t command) {
  // If a new effect is chosen, stop the current effect and start the new one
  if (currentEffect != command) {
    stopEffect(currentEffect); // Stop the previous effect
    currentEffect = command;   // Set the new effect
    lastEffectTime = millis(); // Reset the time tracker
  }
}

void stopEffect(int effect) {
  // Clear the LEDs when stopping any effect
  pixels.clear();
  pixels.show();
}

void changeColor(int direction) {
  // Change the color by cycling through a set of colors
static uint32_t colors[] = {
  // رنگ‌های اصلی
  0xFF0000,  // قرمز (Red)
  0x00FF00,  // سبز (Green)
  0x0000FF,  // آبی (Blue)

  // رنگ‌های ترکیبی
  0xFFFF00,  // زرد (Yellow)
  0xFF00FF,  // بنفش (Magenta)
  0x00FFFF,  // فیروزه‌ای (Cyan)

  // رنگ‌های اصلی و نور‌های مختلف
  0xFF7F00,  // نارنجی (Orange)
  0x800080,  // بنفش تیره (Purple)
  0xA52A2A,  // قهوه‌ای (Brown)
  0xFFD700,  // طلایی (Gold)
  0x008080,  // آبی نفتی (Teal)
  0xFFC0CB,  // صورتی (Pink)
  0x800000,  // شرابی (Maroon)
  0x808000,  // زیتونی (Olive)
  
  // رنگ‌های شاد و روشن
  0x00FA9A,  // سبز بهاری متوسط (Medium Spring Green)
  0x32CD32,  // سبز لیمویی (Lime Green)
  0xFF1493,  // صورتی تیره (Deep Pink)
  0x9400D3,  // بنفش تیره (Dark Violet)
  0x00BFFF,  // آبی آسمانی عمیق (Deep Sky Blue)
  0xB22222,  // قرمز آجر (Firebrick)
  0xFF6347,  // گوجه فرنگی (Tomato)
  0xF08080,  // مرجان روشن (Light Coral)
  
  // رنگ‌های سبز
  0x3CB371,  // سبز دریا (Medium Sea Green)
  0x2E8B57,  // سبز جنگلی (Sea Green)
  0x5F9EA0,  // آبی خاکی (Cadet Blue)
  
  // رنگ‌های آبی و آبی-سبز
  0x6495ED,  // آبی گل شب بو (Cornflower Blue)
  0x4682B4,  // آبی فولادی (Steel Blue)
  0x00CED1,  // آبی تاریک روشن (Dark Turquoise)
  0x20B2AA,  // سبز دریا روشن (Light Sea Green)

  // رنگ‌های خاکی
  0xF0E68C,  // خاکی (Khaki)
  0xBDB76B,  // خاکی تیره (Dark Khaki)
  
  // رنگ‌های آبی-بنفش
  0x8A2BE2,  // بنفش مایل به آبی (Blue Violet)
  0x6A5ACD,  // بنفش روشن (Slate Blue)
  0xFF4500,  // نارنجی آتشین (Orange Red)

  // رنگ‌های ویژه و سایه‌های تیره
  0x9400D3,  // بنفش تیره (Dark Violet)
  0xB22222,  // قرمز آجر (Firebrick)
  0xFF6347,  // گوجه فرنگی (Tomato)
  0x2F4F4F,  // سبز تیره (Dark Slate Green)
  0xD2691E,  // شکلاتی (Chocolate)
  
  // رنگ‌های طلایی و فلزی
  0xFFD700,  // طلایی (Gold)
  0xC0C0C0,  // نقره‌ای (Silver)
  0x8B4513,  // قهوه‌ای تیره (Saddle Brown)
  0xFF8C00,  // نارنجی تیره (Dark Orange)

  // رنگ‌های سفید و روشن
  0xFFFFFF,  // سفید (White)
  0xF5F5F5,  // خاکستری روشن (Light Gray)
  0xD3D3D3,  // خاکستری (Gray)
  
  
  // رنگ‌های تاریک و مرموز
  0x2F4F4F,  // سبز تیره (Dark Slate Green)
  0x00008B,  // آبی دریایی (Dark Blue)
  0x800080,  // بنفش (Purple)
  0x708090,  // خاکی مایل به آبی (Slate Gray)
  
  // طیف رنگ‌های پاستلی
  0xFFB6C1,  // صورتی روشن (Light Pink)
  0xB0E0E6,  // آبی پاستلی (Powder Blue)
  0xE6E6FA,  // لیلاک پاستلی (Lavender)
  0xFFF0F5,  // رز روشن (Lavender Blush)
  0xF0FFFF,  // آبی پاستلی (Azure)

  // رنگ‌های آبی روشن
  0xADD8E6,  // آبی روشن (Light Blue)
  0x87CEFA,  // آبی آسمانی (Sky Blue)
  0x00BFFF,  // آبی آسمانی عمیق (Deep Sky Blue)
  0x1E90FF,  // آبی آسمانی متوسط (Dodger Blue)
};

  static int colorIndex = 0;

  // Update the color index based on the direction
  colorIndex += direction;
  if (colorIndex < 0) colorIndex = sizeof(colors) / sizeof(colors[0]) - 1;
  if (colorIndex >= sizeof(colors) / sizeof(colors[0])) colorIndex = 0;

  currentColor = colors[colorIndex];

  // Update the color of all LEDs
  pixels.fill(currentColor, 0, NUMPIXELS);
  pixels.setBrightness(currentBrightness);
  pixels.show();
}

void changeBrightness(int change) {
  // Change the brightness within the range of 0 to 255
  currentBrightness += change;
  if (currentBrightness > 255) currentBrightness = 255;
  if (currentBrightness < 0) currentBrightness = 0;

  // Update the brightness of all LEDs
  pixels.setBrightness(currentBrightness);
  pixels.show();
}

void executeEffect() {
  switch(currentEffect) {
    case BTN_1: rainbowCycle(random(1, 10)); break; // Random delay for effect
    case BTN_2: oscillatingColorCycle(random(1, 10)); break; // Random delay for effect
    case BTN_3: swirlEffect(random(10, 50)); break; // Random delay for effect
    case BTN_4: randomColorCycle(random(1, 10)); plasmaEffect(random(10, 50)); break; // Random delay for effects
    case BTN_5: noiseEffect(random(50, 150)); break; // Random delay for effect
    case BTN_6: 
      waveEffect(randomColor(), random(10, 30)); // Random delay for effect
      cometEffect(randomColor(), random(10, 30)); // Random delay for effect
      sparkleEffect(randomColor(), random(10, 30)); // Random delay for effect
      snowfall(randomColor(), random(50, 150)); // Random delay for effect
      break;
    case BTN_7: theaterChase(randomColor(), random(20, 100)); break; // Random delay for effect
    case BTN_8: heartbeatEffect(randomColor(), random(1, 10)); break; // Random delay for effect
    case BTN_9: spiralEffect(randomColor(), random(20, 50)); break; // Random delay for effect
    case BTN_0: blinkEffect(randomColor(), random(100, 1000)); break; // Random delay for effect
    case BTN_star: randomTwinkle(randomColor(), random(50, 200)); break; // Random delay for effect
    case BTN_hash: lightningEffect(random(5, 20)); break; // Random delay for effect
  }
}












uint32_t randomColor() {
  // Generate a random color (RGB)
  uint8_t red = random(0, 256);
  uint8_t green = random(0, 256);
  uint8_t blue = random(0, 256);
  return pixels.Color(red, green, blue);
}

void oscillatingColorCycle(uint8_t wait) {
  for (int t = 0; t < 360; t += 10) { // 360 درجه
    for (int i = 0; i < pixels.numPixels(); i++) {
      float wave = (sin(radians(t + (i * 10))) + 1) / 2; // محاسبه موج سینوسی
      uint32_t color = pixels.Color(wave * 255, 0, (1 - wave) * 255); // تغییر رنگ
      pixels.setPixelColor(i, color);
    }
    pixels.show();
    delay(wait);
  }
}

void randomColorCycle(uint8_t wait) {
  for (int i = 0; i < pixels.numPixels(); i++) {
    uint32_t color = pixels.Color(random(0, 256), random(0, 256), random(0, 256)); // رنگ تصادفی
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
  }
}

void snowfall(uint32_t color, uint8_t wait) {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (random(0, 10) < 2) { // احتمال ایجاد برف
      pixels.setPixelColor(i, color);
      pixels.show();
      delay(wait);
      pixels.setPixelColor(i, 0); // خاموش کردن برف
    }
  }
}

void randomTwinkle(uint32_t color, uint8_t wait) {
  for (int i = 0; i < NUMPIXELS; i++) {
    if (random(0, 10) < 3) { // احتمال روشن شدن
      pixels.setPixelColor(i, color);
    } else {
      pixels.setPixelColor(i, 0); // خاموش کردن
    }
  }
  pixels.show();
  delay(wait);
}

// افکت رنگین کمان
void rainbowCycle(uint8_t wait) {
  uint16_t j;
  for (j = 0; j < 256; j++) {
    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

// تابع برای تولید رنگ
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

// افکت تئاتر
void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) {
    for (int b = 0; b < 3; b++) {
      pixels.clear();
      for (int c = b; c < pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color);
      }
      pixels.show();
      delay(wait);
    }
  }
}


void colorWipe(uint32_t color, uint8_t wait) {
  for (int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
  }
}

void starFall(uint32_t color, uint8_t wait) {
  for (int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
    pixels.setPixelColor(i, 0); // خاموش کردن ستاره
  }
}

// افکت چشمک‌زن
void blinkEffect(uint32_t color, uint8_t wait) {
  for (int i = 0; i < 5; i++) {
    pixels.fill(color);
    pixels.show();
    delay(wait);
    pixels.clear();
    pixels.show();
    delay(wait);
  }
}

// افکت جرقه زدن
void sparkleEffect(uint32_t color, uint8_t wait) {
  for (int i = 0; i < 10; i++) {
    int pixel = random(pixels.numPixels());
    pixels.setPixelColor(pixel, color);
    pixels.show();
    delay(wait);
    pixels.setPixelColor(pixel, 0); // خاموش کردن
  }
}

// افکت موج
void waveEffect(uint32_t color, uint8_t wait) {
  for (int i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(wait);
    pixels.setPixelColor(i, 0); // خاموش کردن
  }
}

// افکت تنفس
void breatheEffect(uint32_t color, uint8_t wait) {
  for (int brightness = 0; brightness < 256; brightness += 5) {
    uint32_t dimColor = pixels.Color((color >> 16) * brightness / 255,
                                     ((color >> 8) & 0xFF) * brightness / 255,
                                     (color & 0xFF) * brightness / 255);
    pixels.fill(dimColor);
    pixels.show();
    delay(wait);
  }
  for (int brightness = 255; brightness >= 0; brightness -= 5) {
    uint32_t dimColor = pixels.Color((color >> 16) * brightness / 255,
                                     ((color >> 8) & 0xFF) * brightness / 255,
                                     (color & 0xFF) * brightness / 255);
    pixels.fill(dimColor);
    pixels.show();
    delay(wait);
  }
}

// افکت توپ جهنده
void bouncingBall(uint8_t balls, uint32_t color, uint8_t wait) {
  float gravity = -9.81;
  float h0 = 1;
  float heights[balls];
  float velocities[balls];
  uint32_t colors[balls];

  for (int i = 0; i < balls; i++) {
    heights[i] = h0;
    velocities[i] = 0;
    colors[i] = color;
    h0 *= 0.75;
  }

  for (int t = 0; t < 1000; t++) {
    for (int i = 0; i < balls; i++) {
      velocities[i] += gravity * 0.02;
      heights[i] += velocities[i] * 0.02;

      if (heights[i] < 0) {
        heights[i] = 0;
        velocities[i] = -velocities[i] * 0.75;
      }

      int pos = (int)(heights[i] * (pixels.numPixels() - 1));
      pixels.clear();
      pixels.setPixelColor(pos, colors[i]);
      pixels.show();
    }
    delay(wait);
  }
}

// افکت آتش
void fireEffect(uint8_t cooling) {
  byte heat[NUMPIXELS];

  for (int i = 0; i < NUMPIXELS; i++) heat[i] = 0;

  for (int j = 0; j < 300; j++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      heat[i] = max(0, heat[i] - random(0, ((cooling * 10) / NUMPIXELS) + 2));
    }
    for (int k = NUMPIXELS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }
    if (random(255) < 120) {
      int pos = random(7);
      heat[pos] = heat[pos] + random(160, 255);
    }
    for (int l = 0; l < NUMPIXELS; l++) {
      uint32_t color = HeatColor(heat[l]);
      pixels.setPixelColor(l, color);
    }
    pixels.show();
    delay(50);
  }
}
// تولید رنگ بر اساس دما
uint32_t HeatColor(byte temp) {
  byte t192 = round((temp / 255.0) * 191);
  byte heatramp = t192 & 63;
  heatramp <<= 2;

  if (t192 > 128) {
    return pixels.Color(255, 255, heatramp);
  } else if (t192 > 64) {
    return pixels.Color(255, heatramp, 0);
  } else {
    return pixels.Color(heatramp, 0, 0);
  }
}


// افکت موج سینوسی
void sinWaveEffect(uint8_t wait) {
  for (int t = 0; t < 360; t += 10) { // زمان در زاویه‌های سینوسی
    for (int i = 0; i < NUMPIXELS; i++) {
      float angle = (t + (i * 20)) * (PI / 180); // زاویه بر اساس موقعیت
      uint8_t brightness = 128 + (127 * sin(angle)); // موج سینوسی
      pixels.setPixelColor(i, pixels.Color(brightness, 0, 255 - brightness));
    }
    pixels.show();
    delay(wait);
  }
}

// افکت مارپیچ
void spiralEffect(uint32_t color, uint8_t wait) {
  for (int t = 0; t < 360; t += 10) {
    for (int i = 0; i < NUMPIXELS; i++) {
      float angle = (t + i * 45) * (PI / 180); // چرخش مارپیچ
      uint8_t brightness = 128 + (127 * sin(angle));
      uint8_t r = ((color >> 16) & 0xFF) * brightness / 255;
      uint8_t g = ((color >> 8) & 0xFF) * brightness / 255;
      uint8_t b = (color & 0xFF) * brightness / 255;
      pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();
    delay(wait);
  }
}

// افکت نویز
void noiseEffect(uint8_t wait) {
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < NUMPIXELS; j++) {
      uint8_t red = random(0, 256);
      uint8_t green = random(0, 256);
      uint8_t blue = random(0, 256);
      pixels.setPixelColor(j, pixels.Color(red, green, blue));
    }
    pixels.show();
    delay(wait);
  }
}

// افکت پلاسما
void plasmaEffect(uint8_t wait) {
  for (int t = 0; t < 360; t += 5) {
    for (int i = 0; i < NUMPIXELS; i++) {
      float value = sin((t + i * 36) * (PI / 180)) + cos((t - i * 36) * (PI / 180));
      uint8_t brightness = 128 + (127 * value);
      pixels.setPixelColor(i, pixels.Color(0, brightness, 255 - brightness));
    }
    pixels.show();
    delay(wait);
  }
}

// افکت دنباله‌دار
void cometEffect(uint32_t color, uint8_t wait) {
  int tailLength = 5;
  for (int i = 0; i < NUMPIXELS + tailLength; i++) {
    for (int j = 0; j < NUMPIXELS; j++) {
      int distance = abs(i - j);
      if (distance < tailLength) {
        float brightness = 1.0 - (float)distance / tailLength;
        uint8_t r = ((color >> 16) & 0xFF) * brightness;
        uint8_t g = ((color >> 8) & 0xFF) * brightness;
        uint8_t b = (color & 0xFF) * brightness;
        pixels.setPixelColor(j, pixels.Color(r, g, b));
      } else {
        pixels.setPixelColor(j, 0);
      }
    }
    pixels.show();
    delay(wait);
  }
}

// تابع برای تولید عدد تصادفی با توزیع نرمال
int gaussianRandom(int mean, int stddev) {
  float u1 = random(0, 10000) / 10000.0; // تولید عدد تصادفی بین 0 و 1
  float u2 = random(0, 10000) / 10000.0; // تولید عدد تصادفی بین 0 و 1
  float z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * PI * u2);
  return constrain(mean + stddev * z0, 0, 255); // محدود کردن به بازه 0 تا 255
}

// افکت رعد و برق
void lightningEffect(int duration) {
  unsigned long startTime = millis();
  
  while (millis() - startTime < duration * 1000) {
    // فاز خاموش
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    delay(random(50, 150)); // تاخیر تصادفی

    // تعیین شدت نور با استفاده از توزیع نرمال
    int intensity = gaussianRandom(180, 40); // میانگین 180، انحراف معیار 40
    uint32_t baseColor = pixels.Color(intensity, intensity, random(200, 256)); // رنگ روشن

    for (int flash = 0; flash < random(2, 5); flash++) { // چندین فلاش در هر بار
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, baseColor);
      }
      pixels.show();
      delay(random(30, 80)); // کاهش تأخیر بین فلاش‌ها

      // خاموش کردن LEDها
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
      delay(random(30, 80));
    }

    // ایجاد نوسان در شدت نور با استفاده از توابع سینوسی
    for (int t = 0; t < 30; t++) { // 30 مرحله کاهش شدت
      float fadeFactor = pow(sin(PI * t / 30), 2); // نوسان سینوسی
      uint32_t dimmedColor = pixels.Color(int(baseColor >> 16) * fadeFactor,
                                          int(baseColor >> 8 & 0xFF) * fadeFactor,
                                          int(baseColor & 0xFF) * fadeFactor);
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, dimmedColor);
      }
      pixels.show();
      delay(30); // کاهش تأخیر
    }

    // ایجاد افکت بلنچینگ طبیعی با توزیع نمایی
    for (int t = 0; t < 15; t++) {
      float expFactor = exp(-t / 15.0); // توزیع نمایی برای محو شدن
      uint32_t dimmedColor = pixels.Color(int(baseColor >> 16) * expFactor,
                                          int(baseColor >> 8 & 0xFF) * expFactor,
                                          int(baseColor & 0xFF) * expFactor);
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, dimmedColor);
      }
      pixels.show();
      delay(30); // کاهش تأخیر
    }

    // فلاش نهایی قوی‌تر
    if (random(0, 100) < 25) { // 25% احتمال برای فلاش قوی‌تر
      uint32_t finalFlashColor = pixels.Color(gaussianRandom(255, 20), gaussianRandom(255, 20), gaussianRandom(255, 20));
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, finalFlashColor);
      }
      pixels.show();
      delay(250);
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
    }

    // تاخیر طبیعی قبل از شروع دوباره
    delay(random(400, 1200));
  }
}

// افکت گرداب (Swirl)
void swirlEffect(uint8_t wait) {
  for (int i = 0; i < NUMPIXELS; i++) {
    float angle = (i * 360.0 / NUMPIXELS) + (millis() / 10.0);
    int r = (sin(angle * (PI / 180)) * 127) + 128;
    int g = (cos(angle * (PI / 180)) * 127) + 128;
    int b = (sin(angle * (PI / 180)) * cos(angle * (PI / 180)) * 127) + 128;

    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
  delay(wait);
}

// افکت ضربان قلب (Heartbeat)
void heartbeatEffect(uint32_t color, uint8_t wait) {
  for (int i = 0; i < 255; i++) {
    int brightness = (i * i) / 255; // رشد شدت نور مشابه ضربان قلب
    uint8_t r = ((color >> 16) & 0xFF) * brightness / 255;
    uint8_t g = ((color >> 8) & 0xFF) * brightness / 255;
    uint8_t b = (color & 0xFF) * brightness / 255;
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(r, g, b));
    }
    pixels.show();
    delay(wait);
  }
  for (int i = 255; i > 0; i--) {
    int brightness = (i * i) / 255;
    uint8_t r = ((color >> 16) & 0xFF) * brightness / 255;
    uint8_t g = ((color >> 8) & 0xFF) * brightness / 255;
    uint8_t b = (color & 0xFF) * brightness / 255;
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(r, g, b));
    }
    pixels.show();
    delay(wait);
  }
}
