#include <Wire.h>
#include <BleMouse.h>

#define L_BTN 36
#define R_BTN 34
#define MENU 35

#define I2C_ADDR 0x20  
#define MOUSE_MOVE_SPEED 3    // 鼠标移动速度
#define LONG_PRESS_DELAY 500  // 长按延迟时间
#define MULTI_KEY_DELAY 50    // 多键检测延迟

enum {
  GAMEPAD_INPUT_START = 0,
  GAMEPAD_INPUT_SELECT,
  GAMEPAD_INPUT_UP,
  GAMEPAD_INPUT_DOWN,
  GAMEPAD_INPUT_LEFT,
  GAMEPAD_INPUT_RIGHT,
  GAMEPAD_INPUT_A,
  GAMEPAD_INPUT_B,
  GAMEPAD_INPUT_MENU,
  GAMEPAD_INPUT_L,
  GAMEPAD_INPUT_R,
  GAMEPAD_INPUT_MAX
};

typedef struct {
  uint8_t values[GAMEPAD_INPUT_MAX];
  unsigned long pressTime[GAMEPAD_INPUT_MAX];
  bool wasPressed[GAMEPAD_INPUT_MAX];
} input_gamepad_state;

input_gamepad_state gamepad_state = { 0 };
input_gamepad_state previous_gamepad_state = { 0 };
uint8_t debounce[GAMEPAD_INPUT_MAX];
bool mouseButtonPressed = false;

BleMouse bleMouse("ESP32 Maus", "Espressif", 100);

void gamepad_init() {
  Wire.begin();
  pinMode(L_BTN, INPUT_PULLUP);
  pinMode(R_BTN, INPUT_PULLUP);
  pinMode(MENU, INPUT_PULLUP);

  for (int i = 0; i < GAMEPAD_INPUT_MAX; ++i) {
    debounce[i] = 0xFF;
    gamepad_state.wasPressed[i] = false;
    gamepad_state.pressTime[i] = 0;
  }
}

uint8_t i2c_keypad_read() {
  Wire.requestFrom((uint8_t)I2C_ADDR, (uint8_t)1);
  return Wire.available() ? Wire.read() : 0xFF;
}

input_gamepad_state gamepad_input_read_raw() {
  input_gamepad_state state = { 0 };
  uint8_t i2c_data = i2c_keypad_read();

  for (int i = 0; i < 8; ++i) {
    state.values[i] = ((1 << i) & i2c_data) == 0 ? 1 : 0;
  }

  state.values[GAMEPAD_INPUT_MENU] = !digitalRead(MENU);
  state.values[GAMEPAD_INPUT_L] = !digitalRead(L_BTN);
  state.values[GAMEPAD_INPUT_R] = !digitalRead(R_BTN);

  return state;
}

void setup() {
  Serial.begin(115200);
  gamepad_init();
  bleMouse.begin();
}

void loop() {
  input_gamepad_state state = gamepad_input_read_raw();
  unsigned long currentTime = millis();

  for (int i = 0; i < GAMEPAD_INPUT_MAX; ++i) {
    debounce[i] = (debounce[i] << 1) | (state.values[i] ? 1 : 0);
    uint8_t val = debounce[i] & 0x03;

    gamepad_state.values[i] = (val == 0x03);

    // 处理按键单次触发和长按
    if (gamepad_state.values[i]) {
      if (!gamepad_state.wasPressed[i]) {
        gamepad_state.wasPressed[i] = true;
        gamepad_state.pressTime[i] = currentTime;
        handleButtonPress(i);
      } else if (currentTime - gamepad_state.pressTime[i] > LONG_PRESS_DELAY) {
        handleLongPress(i);
      }
    } else {
      gamepad_state.wasPressed[i] = false;
    }
  }

  // 处理多键功能
  handleMultiKeyFunctions();

  // 处理鼠标移动
  handleMouseMovement();

  delay(10);
}

void handleButtonPress(int button) {
  if (!bleMouse.isConnected()) return;

  // 检查是否有组合键按下
  bool isComboKeyPressed = gamepad_state.values[GAMEPAD_INPUT_MENU] || 
                           gamepad_state.values[GAMEPAD_INPUT_START];

  if (!isComboKeyPressed) {
    switch (button) {
      case GAMEPAD_INPUT_B: 
        bleMouse.press(MOUSE_LEFT);
        mouseButtonPressed = true;
        break;
      case GAMEPAD_INPUT_A: 
        bleMouse.press(MOUSE_RIGHT);
        mouseButtonPressed = true;
        break;
      case GAMEPAD_INPUT_MENU: 
        bleMouse.press(MOUSE_MIDDLE);
        mouseButtonPressed = true;
        break;
    }
  }
}

void handleMultiKeyFunctions() {
  if (!bleMouse.isConnected()) return;

  // MENU + 方向键滚动
  if (gamepad_state.values[GAMEPAD_INPUT_MENU]) {
    if (gamepad_state.values[GAMEPAD_INPUT_UP]) {
      bleMouse.move(0, 0, 1);  // 向上滚动
      delay(MULTI_KEY_DELAY);
    }
    if (gamepad_state.values[GAMEPAD_INPUT_DOWN]) {
      bleMouse.move(0, 0, -1);  // 向下滚动
      delay(MULTI_KEY_DELAY);
    }
    if (gamepad_state.values[GAMEPAD_INPUT_LEFT]) {
      bleMouse.move(0, 0, 0, -1);  // 向左滚动
      delay(MULTI_KEY_DELAY);
    }
    if (gamepad_state.values[GAMEPAD_INPUT_RIGHT]) {
      bleMouse.move(0, 0, 0, 1);  // 向右滚动
      delay(MULTI_KEY_DELAY);
    }
    return;  // 阻止单独的MENU键触发中键
  }
  
  // SELECT + 方向键浏览
  if (gamepad_state.values[GAMEPAD_INPUT_START]) {
    if (gamepad_state.values[GAMEPAD_INPUT_LEFT]) {
      bleMouse.click(MOUSE_BACK);
      delay(MULTI_KEY_DELAY);
    }
    if (gamepad_state.values[GAMEPAD_INPUT_RIGHT]) {
      bleMouse.click(MOUSE_FORWARD);
      delay(MULTI_KEY_DELAY);
    }
  }
}

void handleLongPress(int button) {
  // 长按处理，可以添加额外的逻辑
}

void handleMouseMovement() {
  if (!bleMouse.isConnected()) return;

  // 检查是否有组合键按下
  bool isComboKeyPressed = gamepad_state.values[GAMEPAD_INPUT_MENU] || 
                           gamepad_state.values[GAMEPAD_INPUT_START];

  if (!isComboKeyPressed) {
    int8_t x = 0, y = 0;
    
    if (gamepad_state.values[GAMEPAD_INPUT_LEFT]) x -= MOUSE_MOVE_SPEED;
    if (gamepad_state.values[GAMEPAD_INPUT_RIGHT]) x += MOUSE_MOVE_SPEED;
    if (gamepad_state.values[GAMEPAD_INPUT_UP]) y -= MOUSE_MOVE_SPEED;
    if (gamepad_state.values[GAMEPAD_INPUT_DOWN]) y += MOUSE_MOVE_SPEED;

    if (x != 0 || y != 0) {
      bleMouse.move(x, y);
    }
  }

  // 松开鼠标按键
  if (mouseButtonPressed && 
      !gamepad_state.values[GAMEPAD_INPUT_B] && 
      !gamepad_state.values[GAMEPAD_INPUT_A] && 
      !gamepad_state.values[GAMEPAD_INPUT_MENU]) {
    bleMouse.release(MOUSE_LEFT);
    bleMouse.release(MOUSE_RIGHT);
    bleMouse.release(MOUSE_MIDDLE);
    mouseButtonPressed = false;
  }
}
