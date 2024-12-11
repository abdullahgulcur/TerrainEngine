#pragma once

#include "glm.hpp"

/* Printable keys */
#define KEY_SPACE              (1ULL<<0) | (0 << 8)
#define KEY_APOSTROPHE         (1ULL<<1) | (0 << 8) /* ' */
#define KEY_COMMA              (1ULL<<2) | (0 << 8) /* , */
#define KEY_MINUS              (1ULL<<3) | (0 << 8) /* - */
#define KEY_PERIOD             (1ULL<<4) | (0 << 8) /* . */
#define KEY_SLASH              (1ULL<<5) | (0 << 8) /* / */
#define KEY_0                  (1ULL<<6) | (0 << 8)
#define KEY_1                  (1ULL<<7) | (0 << 8)
#define KEY_2                  (1ULL<<0) | (1 << 8)
#define KEY_3                  (1ULL<<1) | (1 << 8)
#define KEY_4                  (1ULL<<2) | (1 << 8)
#define KEY_5                  (1ULL<<3) | (1 << 8)
#define KEY_6                  (1ULL<<4) | (1 << 8)
#define KEY_7                  (1ULL<<5) | (1 << 8)
#define KEY_8                  (1ULL<<6) | (1 << 8)
#define KEY_9                  (1ULL<<7) | (1 << 8)
#define KEY_SEMICOLON          (1ULL<<0) | (2 << 8) /* ; */
#define KEY_EQUAL              (1ULL<<1) | (2 << 8) /* = */
#define KEY_A                  (1ULL<<2) | (2 << 8)
#define KEY_B                  (1ULL<<3) | (2 << 8)
#define KEY_C                  (1ULL<<4) | (2 << 8)
#define KEY_D                  (1ULL<<5) | (2 << 8)
#define KEY_E                  (1ULL<<6) | (2 << 8)
#define KEY_F                  (1ULL<<7) | (2 << 8)
#define KEY_G                  (1ULL<<0) | (3 << 8)
#define KEY_H                  (1ULL<<1) | (3 << 8)
#define KEY_I                  (1ULL<<2) | (3 << 8)
#define KEY_J                  (1ULL<<3) | (3 << 8)
#define KEY_K                  (1ULL<<4) | (3 << 8)
#define KEY_L                  (1ULL<<5) | (3 << 8)
#define KEY_M                  (1ULL<<6) | (3 << 8)
#define KEY_N                  (1ULL<<7) | (3 << 8)
#define KEY_O                  (1ULL<<0) | (4 << 8)
#define KEY_P                  (1ULL<<1) | (4 << 8)
#define KEY_Q                  (1ULL<<2) | (4 << 8)
#define KEY_R                  (1ULL<<3) | (4 << 8)
#define KEY_S                  (1ULL<<4) | (4 << 8)
#define KEY_T                  (1ULL<<5) | (4 << 8)
#define KEY_U                  (1ULL<<6) | (4 << 8)
#define KEY_V                  (1ULL<<7) | (4 << 8)
#define KEY_W                  (1ULL<<0) | (5 << 8)
#define KEY_X                  (1ULL<<1) | (5 << 8)
#define KEY_Y                  (1ULL<<2) | (5 << 8)
#define KEY_Z                  (1ULL<<3) | (5 << 8)
#define KEY_LEFT_BRACKET       (1ULL<<4) | (5 << 8) /* [ */
#define KEY_BACKSLASH          (1ULL<<5) | (5 << 8) /* \ */
#define KEY_RIGHT_BRACKET      (1ULL<<6) | (5 << 8) /* ] */
#define KEY_GRAVE_ACCENT       (1ULL<<7) | (5 << 8) /* ` */
#define GLFW_KEY_WORLD_1       (1ULL<<0) | (6 << 8) /* non-US #1 */
#define GLFW_KEY_WORLD_2       (1ULL<<1) | (6 << 8) /* non-US #2 */
#define KEY_ESCAPE             (1ULL<<2) | (6 << 8)
#define KEY_ENTER              (1ULL<<3) | (6 << 8)
#define KEY_TAB                (1ULL<<4) | (6 << 8)
#define KEY_BACKSPACE          (1ULL<<5) | (6 << 8)
#define KEY_INSERT             (1ULL<<6) | (6 << 8)
#define KEY_DELETE             (1ULL<<7) | (6 << 8)
#define KEY_RIGHT              (1ULL<<0) | (7 << 8)
#define KEY_LEFT               (1ULL<<1) | (7 << 8)
#define KEY_DOWN               (1ULL<<2) | (7 << 8)
#define KEY_UP                 (1ULL<<3) | (7 << 8)
#define KEY_PAGE_UP            (1ULL<<4) | (7 << 8)
#define KEY_PAGE_DOWN          (1ULL<<5) | (7 << 8)
#define KEY_HOME               (1ULL<<6) | (7 << 8)
#define KEY_END                (1ULL<<7) | (7 << 8)
#define KEY_CAPS_LOCK          (1ULL<<0) | (8 << 8)
#define KEY_SCROLL_LOCK        (1ULL<<1) | (8 << 8)
#define KEY_NUM_LOCK           (1ULL<<2) | (8 << 8)
#define KEY_PRINT_SCREEN       (1ULL<<3) | (8 << 8)
#define KEY_PAUSE              (1ULL<<4) | (8 << 8)
#define KEY_F1                 (1ULL<<5) | (8 << 8)
#define KEY_F2                 (1ULL<<6) | (8 << 8)
#define KEY_F3                 (1ULL<<7) | (8 << 8)
#define KEY_F4                 (1ULL<<0) | (9 << 8)
#define KEY_F5                 (1ULL<<1) | (9 << 8)
#define KEY_F6                 (1ULL<<2) | (9 << 8)
#define KEY_F7                 (1ULL<<3) | (9 << 8)
#define KEY_F8                 (1ULL<<4) | (9 << 8)
#define KEY_F9                 (1ULL<<5) | (9 << 8)
#define KEY_F10                (1ULL<<6) | (9 << 8)
#define KEY_F11                (1ULL<<7) | (9 << 8)
#define KEY_F12                (1ULL<<0) | (10 << 8)
#define KEY_F13                (1ULL<<1) | (10 << 8)
#define KEY_F14                (1ULL<<2) | (10 << 8)
#define KEY_F15                (1ULL<<3) | (10 << 8)
#define KEY_F16                (1ULL<<4) | (10 << 8)
#define KEY_F17                (1ULL<<5) | (10 << 8)
#define KEY_F18                (1ULL<<6) | (10 << 8)
#define KEY_F19                (1ULL<<7) | (10 << 8)
#define KEY_F20                (1ULL<<0) | (11 << 8)
#define KEY_F21                (1ULL<<1) | (11 << 8)
#define KEY_F22                (1ULL<<2) | (11 << 8)
#define KEY_F23                (1ULL<<3) | (11 << 8)
#define KEY_F24                (1ULL<<4) | (11 << 8)
#define KEY_F25                (1ULL<<5) | (11 << 8)
#define KEY_KP_0               (1ULL<<6) | (11 << 8)
#define KEY_KP_1               (1ULL<<7) | (11 << 8)
#define KEY_KP_2               (1ULL<<0) | (12 << 8)
#define KEY_KP_3               (1ULL<<1) | (12 << 8)
#define KEY_KP_4               (1ULL<<2) | (12 << 8)
#define KEY_KP_5               (1ULL<<3) | (12 << 8)
#define KEY_KP_6               (1ULL<<4) | (12 << 8)
#define KEY_KP_7               (1ULL<<5) | (12 << 8)
#define KEY_KP_8               (1ULL<<6) | (12 << 8)
#define KEY_KP_9               (1ULL<<7) | (12 << 8)
#define KEY_KP_DECIMAL         (1ULL<<0) | (13 << 8)
#define KEY_KP_DIVIDE          (1ULL<<1) | (13 << 8)
#define KEY_KP_MULTIPLY        (1ULL<<2) | (13 << 8)
#define KEY_KP_SUBTRACT        (1ULL<<3) | (13 << 8)
#define KEY_KP_ADD             (1ULL<<4) | (13 << 8)
#define KEY_KP_ENTER           (1ULL<<5) | (13 << 8)
#define KEY_KP_EQUAL           (1ULL<<6) | (13 << 8)
#define KEY_LEFT_SHIFT         (1ULL<<7) | (13 << 8)
#define KEY_LEFT_CONTROL       (1ULL<<0) | (14 << 8)
#define KEY_LEFT_ALT           (1ULL<<1) | (14 << 8)
#define KEY_LEFT_SUPER         (1ULL<<2) | (14 << 8)
#define KEY_RIGHT_SHIFT        (1ULL<<3) | (14 << 8)
#define KEY_RIGHT_CONTROL      (1ULL<<4) | (14 << 8)
#define KEY_RIGHT_ALT          (1ULL<<5) | (14 << 8)
#define KEY_RIGHT_SUPER        (1ULL<<6) | (14 << 8)
#define KEY_MENU               (1ULL<<7) | (14 << 8)

#define MOUSE_BUTTON_1         (1ULL<<0)
#define MOUSE_BUTTON_2         (1ULL<<1)
#define MOUSE_BUTTON_3         (1ULL<<2)
#define MOUSE_BUTTON_4         (1ULL<<3)
#define MOUSE_BUTTON_5         (1ULL<<4)
#define MOUSE_BUTTON_6         (1ULL<<5)
#define MOUSE_BUTTON_7         (1ULL<<6)
#define MOUSE_BUTTON_8         (1ULL<<7)

namespace Engine {

	class Input {
	private:

		UINT8 keyDown[16];
		UINT8 keyUp[16];
		UINT8 keyPressed[16];

		UINT8 mouseDown = 0;
		UINT8 mouseUp = 0;
		UINT8 mousePressed = 0;
		float scrollDelta = 0.f;

		glm::i16vec2 mouseDelta;
		glm::i16vec2 cursorPos;

		Input();
		~Input();

		void pressMouseButton(UINT64 button);
		void releaseMouseButton(UINT64 button);
		void pressButton(UINT64 key);
		void releaseButton(UINT64 key);

	public:

		static Input* create();
		
		void init();
		void update();
		
		bool getKeyPressed(UINT64 key);
		bool getKeyUp(UINT64 key);
		bool getKeyDown(UINT64 key);
		bool getMousePressed(UINT64 button);
		bool getMouseUp(UINT64 button);
		bool getMouseDown(UINT64 button);
		float getMouseScrollDelta();
		glm::vec2 getMouseDelta();

		void handleKeyboardInput(int key, int action);
		void handleMouseInput(int button, int action);
		void handleMouseScrollInput(float yOffset);

		// -------- BIT MANIPULATION --------
		// ref: https://stackoverflow.com/questions/47981/how-to-set-clear-and-toggle-a-single-bit

		UINT64 bit_set(UINT64 number, UINT64 n) {
			return number | ((UINT64)1 << n);
		}

		UINT64 bit_clear(UINT64 number, UINT64 n) {
			return number & ~((UINT64)1 << n);
		}

		UINT64 bit_toggle(UINT64 number, UINT64 n) {
			return number ^ ((UINT64)1 << n);
		}

		UINT64 bit_check(UINT64 number, UINT64 n) {
			return (number >> n) & (UINT64)1;
		}

		UINT64 bit_set_to(UINT64 number, UINT64 n) {
			return number | ((UINT64)1 << n);
		}
	};
}