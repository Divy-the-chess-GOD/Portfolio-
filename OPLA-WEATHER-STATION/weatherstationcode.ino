#include <Arduino_MKRIoTCarrier.h>

MKRIoTCarrier carrier;

float temperature = 0;
float humidity = 0;
float pressure = 1013;

unsigned long lastUpdate = 0;

// Update every 5 seconds
const unsigned long UPDATE_INTERVAL = 5000;

// ================= COLORS =================

uint16_t tempColor(float T) {

  if (T >= 32) {
    return carrier.display.color565(255, 90, 40);
  }

  if (T <= 24) {
    return carrier.display.color565(70, 170, 255);
  }

  return carrier.display.color565(80, 255, 140);
}

// ================= PANEL =================

void drawPanel(
  int x,
  int y,
  int w,
  int h,
  int radius,
  const char* label,
  String value,
  uint16_t bg,
  uint16_t border,
  uint16_t valueColor
) {

  // PANEL
  carrier.display.fillRoundRect(x, y, w, h, radius, bg);

  // BORDER
  carrier.display.drawRoundRect(x, y, w, h, radius, border);

  // LABEL
  carrier.display.setTextSize(1);
  carrier.display.setTextColor(border);

  carrier.display.setCursor(x + 8, y + 5);
  carrier.display.print(label);

  // VALUE
  carrier.display.setTextSize(2);
  carrier.display.setTextColor(valueColor);

  carrier.display.setCursor(x + 8, y + 18);
  carrier.display.print(value);
}

// ================= DASHBOARD =================

void drawDashboard() {

  carrier.display.fillScreen(ST77XX_BLACK);

  uint16_t tColor = tempColor(temperature);

  uint16_t hColor =
    carrier.display.color565(80, 190, 255);

  uint16_t pColor =
    carrier.display.color565(220, 160, 255);

  uint16_t sColor =
    carrier.display.color565(255, 210, 80);

  // DARK BACKGROUNDS

  uint16_t tempBG =
    carrier.display.color565(25, 8, 5);

  uint16_t humBG =
    carrier.display.color565(5, 16, 28);

  uint16_t pressBG =
    carrier.display.color565(18, 8, 28);

  uint16_t stateBG =
    carrier.display.color565(28, 22, 5);

  // TEXT COLOR

  uint16_t white =
    carrier.display.color565(255, 255, 255);

  // WEATHER STATE

  String state = "STABLE";

  if (humidity > 82 && temperature > 28) {
    state = "RAINY";
  }

  else if (humidity > 78) {
    state = "HUMID";
  }

  else if (temperature >= 32) {
    state = "HOT";
  }

  else if (temperature <= 20) {
    state = "COOL";
  }

  // ================= PANELS =================

  // TEMPERATURE
  drawPanel(
    55, 24,
    130, 36,
    18,

    "TEMPERATURE",

    String(temperature, 1) + "C",

    tempBG,

    tColor,

    white
  );

  // HUMIDITY
  drawPanel(
    28, 66,
    184, 38,
    14,

    "HUMIDITY",

    String(humidity, 0) + "%",

    humBG,

    hColor,

    white
  );

  // PRESSURE
  drawPanel(
    18, 110,
    204, 38,
    14,

    "PRESSURE",

    String(pressure, 0),

    pressBG,

    pColor,

    white
  );

  // WEATHER STATE
  drawPanel(
    28, 154,
    184, 38,
    14,

    "WEATHER",

    state,

    stateBG,

    sColor,

    white
  );
}

// ================= SETUP =================

void setup() {

  CARRIER_CASE = false;

  carrier.begin();

  carrier.display.setRotation(0);

  carrier.display.fillScreen(ST77XX_BLACK);
}

// ================= LOOP =================

void loop() {

  if (millis() - lastUpdate >= UPDATE_INTERVAL) {

    // READ SENSORS

    temperature =
      carrier.Env.readTemperature();

    humidity =
      carrier.Env.readHumidity();

    // If this line errors,
    // replace with:
    // pressure = 1013.0;

    pressure =
      carrier.Pressure.readPressure();

    // UPDATE DISPLAY

    drawDashboard();

    lastUpdate = millis();
  }
}
