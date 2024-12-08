#include "pch.h"
#include "input.h"
#include "core.h"
#include "glfw_context.h"
//#include "Core/allocator.h"

namespace Engine {

	Input* Input::create() {
		
		return new Input();
	}

	Input::Input() {

	}

	Input::~Input() {

	}

	void Input::init() {

		cursorPos = Core::getGlfwContext()->getCursorPos();
		mouseDelta = glm::vec2(0);

		for (int i = 0; i < 16; i++)
			keyPressed[i] = 0;
	}

	void Input::update() {

		for (int i = 0; i < 16; i++) {
			keyDown[i] = 0U;
			keyUp[i] = 0U;
		}

		mouseDown = 0U;
		mouseUp = 0U;
		scrollDelta = 0.f;

		glm::i16vec2 newCursorPos = Core::getGlfwContext()->getCursorPos();
		mouseDelta = newCursorPos - cursorPos;
		cursorPos = newCursorPos;
	}

	void Input::pressMouseButton(UINT64 button) {

		mouseDown = bit_set(static_cast<UINT64>(mouseDown), button);
		mousePressed = bit_set(static_cast<UINT64>(mousePressed), button);
	}

	void Input::releaseMouseButton(UINT64 button) {

		mouseUp = bit_set(static_cast<UINT64>(mouseUp), button);
		mousePressed = bit_clear(static_cast<UINT64>(mousePressed), button);
	}

	void Input::pressButton(UINT64 key) {

		UINT64 index = key >> 8;
		constexpr UINT64 divisor = 1ULL << 8;
		UINT64 remainder = key % divisor;
		keyDown[index] |= remainder;
		keyPressed[index] |= remainder;
	}

	void Input::releaseButton(UINT64 key) {

		UINT64 index = key >> 8;
		constexpr UINT64 divisor = 1ULL << 8;
		UINT64 remainder = key % divisor;
		keyUp[index] |= remainder;
		keyPressed[index] &= ~remainder;
	}

	bool Input::getKeyPressed(UINT64 key) {

		UINT64 index = key >> 8;
		constexpr UINT64 divisor = 1ULL << 8;
		UINT64 remainder = key % divisor;
		return keyPressed[index] & remainder;
	}

	bool Input::getKeyUp(UINT64 key) {

		UINT64 index = key >> 8;
		constexpr UINT64 divisor = 1ULL << 8;
		UINT64 remainder = key % divisor;
		return keyUp[index] & remainder;
	}

	bool Input::getKeyDown(UINT64 key) {

		UINT64 index = key >> 8;
		constexpr UINT64 divisor = 1ULL << 8;
		UINT64 remainder = key % divisor;
		return keyDown[index] & remainder;
	}

	bool Input::getMousePressed(UINT64 button) {

		return static_cast<bool>(bit_check(static_cast<UINT64>(mousePressed), button));
	}

	bool Input::getMouseUp(UINT64 button) {

		return static_cast<bool>(bit_check(static_cast<UINT64>(mouseUp), button));
	}

	bool Input::getMouseDown(UINT64 button) {

		return static_cast<bool>(bit_check(static_cast<UINT64>(mouseDown), button));
	}

	float Input::getMouseScrollDelta() {

		return scrollDelta;
	}

	glm::vec2 Input::getMouseDelta() {

		return mouseDelta;
	}

	void Input::handleMouseScrollInput(float yOffset) {

		scrollDelta = yOffset;
	}

	void Input::handleMouseInput(int button, int action) {

		if (action == GLFW_PRESS) {
			switch (button) {
			case GLFW_MOUSE_BUTTON_1:
				pressMouseButton(MOUSE_BUTTON_1);
				break;
			case GLFW_MOUSE_BUTTON_2:
				pressMouseButton(MOUSE_BUTTON_2);
				break;
			case GLFW_MOUSE_BUTTON_3:
				pressMouseButton(MOUSE_BUTTON_3);
				break;
			case GLFW_MOUSE_BUTTON_4:
				pressMouseButton(MOUSE_BUTTON_4);
				break;
			case GLFW_MOUSE_BUTTON_5:
				pressMouseButton(MOUSE_BUTTON_5);
				break;
			case GLFW_MOUSE_BUTTON_6:
				pressMouseButton(MOUSE_BUTTON_6);
				break;
			case GLFW_MOUSE_BUTTON_7:
				pressMouseButton(MOUSE_BUTTON_7);
				break;
			case GLFW_MOUSE_BUTTON_8:
				pressMouseButton(MOUSE_BUTTON_8);
				break;
			}
		}

		if (action == GLFW_RELEASE) {
			switch (button) {
			case GLFW_MOUSE_BUTTON_1:
				releaseMouseButton(MOUSE_BUTTON_1);
				break;
			case GLFW_MOUSE_BUTTON_2:
				releaseMouseButton(MOUSE_BUTTON_2);
				break;
			case GLFW_MOUSE_BUTTON_3:
				releaseMouseButton(MOUSE_BUTTON_3);
				break;
			case GLFW_MOUSE_BUTTON_4:
				releaseMouseButton(MOUSE_BUTTON_4);
				break;
			case GLFW_MOUSE_BUTTON_5:
				releaseMouseButton(MOUSE_BUTTON_5);
				break;
			case GLFW_MOUSE_BUTTON_6:
				releaseMouseButton(MOUSE_BUTTON_6);
				break;
			case GLFW_MOUSE_BUTTON_7:
				releaseMouseButton(MOUSE_BUTTON_7);
				break;
			case GLFW_MOUSE_BUTTON_8:
				releaseMouseButton(MOUSE_BUTTON_8);
				break;
			}
		}
	}

	void Input::handleKeyboardInput(int key, int action) {

		if (action == GLFW_PRESS) {
			switch (key) {
			case GLFW_KEY_SPACE:
				pressButton(KEY_SPACE);
				break;
			case GLFW_KEY_APOSTROPHE:
				pressButton(KEY_APOSTROPHE);
				break;
			case GLFW_KEY_COMMA:
				pressButton(KEY_COMMA);
				break;
			case GLFW_KEY_MINUS:
				pressButton(KEY_MINUS);
				break;
			case GLFW_KEY_PERIOD:
				pressButton(KEY_PERIOD);
				break;
			case GLFW_KEY_SLASH:
				pressButton(KEY_SLASH);
				break;
			case GLFW_KEY_0:
				pressButton(KEY_0);
				break;
			case GLFW_KEY_1:
				pressButton(KEY_1);
				break;
			case GLFW_KEY_2:
				pressButton(KEY_2);
				break;
			case GLFW_KEY_3:
				pressButton(KEY_3);
				break;
			case GLFW_KEY_4:
				pressButton(KEY_4);
				break;
			case GLFW_KEY_5:
				pressButton(KEY_5);
				break;
			case GLFW_KEY_6:
				pressButton(KEY_6);
				break;
			case GLFW_KEY_7:
				pressButton(KEY_7);
				break;
			case GLFW_KEY_8:
				pressButton(KEY_8);
				break;
			case GLFW_KEY_9:
				pressButton(KEY_9);
				break;
			case GLFW_KEY_SEMICOLON:
				pressButton(KEY_SEMICOLON);
				break;
			case GLFW_KEY_EQUAL:
				pressButton(KEY_EQUAL);
				break;
			case GLFW_KEY_A:
				pressButton(KEY_A);
				break;
			case GLFW_KEY_B:
				pressButton(KEY_B);
				break;
			case GLFW_KEY_C:
				pressButton(KEY_C);
				break;
			case GLFW_KEY_D:
				pressButton(KEY_D);
				break;
			case GLFW_KEY_E:
				pressButton(KEY_E);
				break;
			case GLFW_KEY_F:
				pressButton(KEY_F);
				break;
			case GLFW_KEY_G:
				pressButton(KEY_G);
				break;
			case GLFW_KEY_H:
				pressButton(KEY_H);
				break;
			case GLFW_KEY_I:
				pressButton(KEY_I);
				break;
			case GLFW_KEY_J:
				pressButton(KEY_J);
				break;
			case GLFW_KEY_K:
				pressButton(KEY_K);
				break;
			case GLFW_KEY_L:
				pressButton(KEY_L);
				break;
			case GLFW_KEY_M:
				pressButton(KEY_M);
				break;
			case GLFW_KEY_N:
				pressButton(KEY_N);
				break;
			case GLFW_KEY_O:
				pressButton(KEY_O);
				break;
			case GLFW_KEY_P:
				pressButton(KEY_P);
				break;
			case GLFW_KEY_Q:
				pressButton(KEY_Q);
				break;
			case GLFW_KEY_R:
				pressButton(KEY_R);
				break;
			case GLFW_KEY_S:
				pressButton(KEY_S);
				break;
			case GLFW_KEY_T:
				pressButton(KEY_T);
				break;
			case GLFW_KEY_U:
				pressButton(KEY_U);
				break;
			case GLFW_KEY_V:
				pressButton(KEY_V);
				break;
			case GLFW_KEY_W:
				pressButton(KEY_W);
				break;
			case GLFW_KEY_X:
				pressButton(KEY_X);
				break;
			case GLFW_KEY_Y:
				pressButton(KEY_Y);
				break;
			case GLFW_KEY_Z:
				pressButton(KEY_Z);
				break;
			case GLFW_KEY_LEFT_BRACKET:
				pressButton(KEY_LEFT_BRACKET);
				break;
			case GLFW_KEY_BACKSLASH:
				pressButton(KEY_BACKSLASH);
				break;
			case GLFW_KEY_RIGHT_BRACKET:
				pressButton(KEY_RIGHT_BRACKET);
				break;
			case GLFW_KEY_GRAVE_ACCENT:
				pressButton(KEY_GRAVE_ACCENT);
				break;
			case GLFW_KEY_WORLD_1:
				pressButton(GLFW_KEY_WORLD_1);
				break;
			case GLFW_KEY_WORLD_2:
				pressButton(GLFW_KEY_WORLD_2);
				break;
			case GLFW_KEY_ESCAPE:
				pressButton(KEY_ESCAPE);
				break;
			case GLFW_KEY_ENTER:
				pressButton(KEY_ENTER);
				break;
			case GLFW_KEY_TAB:
				pressButton(KEY_TAB);
				break;
			case GLFW_KEY_BACKSPACE:
				pressButton(KEY_BACKSPACE);
				break;
			case GLFW_KEY_INSERT:
				pressButton(KEY_INSERT);
				break;
			case GLFW_KEY_DELETE:
				pressButton(KEY_DELETE);
				break;
			case GLFW_KEY_RIGHT:
				pressButton(KEY_RIGHT);
				break;
			case GLFW_KEY_LEFT:
				pressButton(KEY_LEFT);
				break;
			case GLFW_KEY_DOWN:
				pressButton(KEY_DOWN);
				break;
			case GLFW_KEY_UP:
				pressButton(KEY_UP);
				break;
			case GLFW_KEY_PAGE_UP:
				pressButton(KEY_PAGE_UP);
				break;
			case GLFW_KEY_PAGE_DOWN:
				pressButton(KEY_PAGE_DOWN);
				break;
			case GLFW_KEY_HOME:
				pressButton(KEY_HOME);
				break;
			case GLFW_KEY_END:
				pressButton(KEY_END);
				break;
			case GLFW_KEY_CAPS_LOCK:
				pressButton(KEY_CAPS_LOCK);
				break;
			case GLFW_KEY_SCROLL_LOCK:
				pressButton(KEY_SCROLL_LOCK);
				break;
			case GLFW_KEY_NUM_LOCK:
				pressButton(KEY_NUM_LOCK);
				break;
			case GLFW_KEY_PRINT_SCREEN:
				pressButton(KEY_PRINT_SCREEN);
				break;
			case GLFW_KEY_PAUSE:
				pressButton(KEY_PAUSE);
				break;
			case GLFW_KEY_F1:
				pressButton(KEY_F1);
				break;
			case GLFW_KEY_F2:
				pressButton(KEY_F2);
				break;
			case GLFW_KEY_F3:
				pressButton(KEY_F3);
				break;
			case GLFW_KEY_F4:
				pressButton(KEY_F4);
				break;
			case GLFW_KEY_F5:
				pressButton(KEY_F5);
				break;
			case GLFW_KEY_F6:
				pressButton(KEY_F6);
				break;
			case GLFW_KEY_F7:
				pressButton(KEY_F7);
				break;
			case GLFW_KEY_F8:
				pressButton(KEY_F8);
				break;
			case GLFW_KEY_F9:
				pressButton(KEY_F9);
				break;
			case GLFW_KEY_F10:
				pressButton(KEY_F10);
				break;
			case GLFW_KEY_F11:
				pressButton(KEY_F11);
				break;
			case GLFW_KEY_F12:
				pressButton(KEY_F12);
				break;
			case GLFW_KEY_F13:
				pressButton(KEY_F13);
				break;
			case GLFW_KEY_F14:
				pressButton(KEY_F14);
				break;
			case GLFW_KEY_F15:
				pressButton(KEY_F15);
				break;
			case GLFW_KEY_F16:
				pressButton(KEY_F16);
				break;
			case GLFW_KEY_F17:
				pressButton(KEY_F17);
				break;
			case GLFW_KEY_F18:
				pressButton(KEY_F18);
				break;
			case GLFW_KEY_F19:
				pressButton(KEY_F19);
				break;
			case GLFW_KEY_F20:
				pressButton(KEY_F20);
				break;
			case GLFW_KEY_F21:
				pressButton(KEY_F21);
				break;
			case GLFW_KEY_F22:
				pressButton(KEY_F22);
				break;
			case GLFW_KEY_F23:
				pressButton(KEY_F23);
				break;
			case GLFW_KEY_F24:
				pressButton(KEY_F24);
				break;
			case GLFW_KEY_F25:
				pressButton(KEY_F25);
				break;
			case GLFW_KEY_KP_0:
				pressButton(KEY_KP_0);
				break;
			case GLFW_KEY_KP_1:
				pressButton(KEY_KP_1);
				break;
			case GLFW_KEY_KP_2:
				pressButton(KEY_KP_2);
				break;
			case GLFW_KEY_KP_3:
				pressButton(KEY_KP_3);
				break;
			case GLFW_KEY_KP_4:
				pressButton(KEY_KP_4);
				break;
			case GLFW_KEY_KP_5:
				pressButton(KEY_KP_5);
				break;
			case GLFW_KEY_KP_6:
				pressButton(KEY_KP_6);
				break;
			case GLFW_KEY_KP_7:
				pressButton(KEY_KP_7);
				break;
			case GLFW_KEY_KP_8:
				pressButton(KEY_KP_8);
				break;
			case GLFW_KEY_KP_9:
				pressButton(KEY_KP_9);
				break;
			case GLFW_KEY_KP_DECIMAL:
				pressButton(KEY_KP_DECIMAL);
				break;
			case GLFW_KEY_KP_DIVIDE:
				pressButton(KEY_KP_DIVIDE);
				break;
			case GLFW_KEY_KP_MULTIPLY:
				pressButton(KEY_KP_MULTIPLY);
				break;
			case GLFW_KEY_KP_SUBTRACT:
				pressButton(KEY_KP_SUBTRACT);
				break;
			case GLFW_KEY_KP_ADD:
				pressButton(KEY_KP_ADD);
				break;
			case GLFW_KEY_KP_ENTER:
				pressButton(KEY_KP_ENTER);
				break;
			case GLFW_KEY_KP_EQUAL:
				pressButton(KEY_KP_EQUAL);
				break;
			case GLFW_KEY_LEFT_SHIFT:
				pressButton(KEY_LEFT_SHIFT);
				break;
			case GLFW_KEY_LEFT_CONTROL:
				pressButton(KEY_LEFT_CONTROL);
				break;
			case GLFW_KEY_LEFT_ALT:
				pressButton(KEY_LEFT_ALT);
				break;
			case GLFW_KEY_LEFT_SUPER:
				pressButton(KEY_LEFT_SUPER);
				break;
			case GLFW_KEY_RIGHT_SHIFT:
				pressButton(KEY_RIGHT_SHIFT);
				break;
			case GLFW_KEY_RIGHT_CONTROL:
				pressButton(KEY_RIGHT_CONTROL);
				break;
			case GLFW_KEY_RIGHT_ALT:
				pressButton(KEY_RIGHT_ALT);
				break;
			case GLFW_KEY_RIGHT_SUPER:
				pressButton(KEY_RIGHT_SUPER);
				break;
			case GLFW_KEY_MENU:
				pressButton(KEY_MENU);
				break;
			default: break;
			}
		}

		if (action == GLFW_RELEASE) {
			switch (key) {
			case GLFW_KEY_SPACE:
				releaseButton(KEY_SPACE);
				break;
			case GLFW_KEY_APOSTROPHE:
				releaseButton(KEY_APOSTROPHE);
				break;
			case GLFW_KEY_COMMA:
				releaseButton(KEY_COMMA);
				break;
			case GLFW_KEY_MINUS:
				releaseButton(KEY_MINUS);
				break;
			case GLFW_KEY_PERIOD:
				releaseButton(KEY_PERIOD);
				break;
			case GLFW_KEY_SLASH:
				releaseButton(KEY_SLASH);
				break;
			case GLFW_KEY_0:
				releaseButton(KEY_0);
				break;
			case GLFW_KEY_1:
				releaseButton(KEY_1);
				break;
			case GLFW_KEY_2:
				releaseButton(KEY_2);
				break;
			case GLFW_KEY_3:
				releaseButton(KEY_3);
				break;
			case GLFW_KEY_4:
				releaseButton(KEY_4);
				break;
			case GLFW_KEY_5:
				releaseButton(KEY_5);
				break;
			case GLFW_KEY_6:
				releaseButton(KEY_6);
				break;
			case GLFW_KEY_7:
				releaseButton(KEY_7);
				break;
			case GLFW_KEY_8:
				releaseButton(KEY_8);
				break;
			case GLFW_KEY_9:
				releaseButton(KEY_9);
				break;
			case GLFW_KEY_SEMICOLON:
				releaseButton(KEY_SEMICOLON);
				break;
			case GLFW_KEY_EQUAL:
				releaseButton(KEY_EQUAL);
				break;
			case GLFW_KEY_A:
				releaseButton(KEY_A);
				break;
			case GLFW_KEY_B:
				releaseButton(KEY_B);
				break;
			case GLFW_KEY_C:
				releaseButton(KEY_C);
				break;
			case GLFW_KEY_D:
				releaseButton(KEY_D);
				break;
			case GLFW_KEY_E:
				releaseButton(KEY_E);
				break;
			case GLFW_KEY_F:
				releaseButton(KEY_F);
				break;
			case GLFW_KEY_G:
				releaseButton(KEY_G);
				break;
			case GLFW_KEY_H:
				releaseButton(KEY_H);
				break;
			case GLFW_KEY_I:
				releaseButton(KEY_I);
				break;
			case GLFW_KEY_J:
				releaseButton(KEY_J);
				break;
			case GLFW_KEY_K:
				releaseButton(KEY_K);
				break;
			case GLFW_KEY_L:
				releaseButton(KEY_L);
				break;
			case GLFW_KEY_M:
				releaseButton(KEY_M);
				break;
			case GLFW_KEY_N:
				releaseButton(KEY_N);
				break;
			case GLFW_KEY_O:
				releaseButton(KEY_O);
				break;
			case GLFW_KEY_P:
				releaseButton(KEY_P);
				break;
			case GLFW_KEY_Q:
				releaseButton(KEY_Q);
				break;
			case GLFW_KEY_R:
				releaseButton(KEY_R);
				break;
			case GLFW_KEY_S:
				releaseButton(KEY_S);
				break;
			case GLFW_KEY_T:
				releaseButton(KEY_T);
				break;
			case GLFW_KEY_U:
				releaseButton(KEY_U);
				break;
			case GLFW_KEY_V:
				releaseButton(KEY_V);
				break;
			case GLFW_KEY_W:
				releaseButton(KEY_W);
				break;
			case GLFW_KEY_X:
				releaseButton(KEY_X);
				break;
			case GLFW_KEY_Y:
				releaseButton(KEY_Y);
				break;
			case GLFW_KEY_Z:
				releaseButton(KEY_Z);
				break;
			case GLFW_KEY_LEFT_BRACKET:
				releaseButton(KEY_LEFT_BRACKET);
				break;
			case GLFW_KEY_BACKSLASH:
				releaseButton(KEY_BACKSLASH);
				break;
			case GLFW_KEY_RIGHT_BRACKET:
				releaseButton(KEY_RIGHT_BRACKET);
				break;
			case GLFW_KEY_GRAVE_ACCENT:
				releaseButton(KEY_GRAVE_ACCENT);
				break;
			case GLFW_KEY_WORLD_1:
				releaseButton(GLFW_KEY_WORLD_1);
				break;
			case GLFW_KEY_WORLD_2:
				releaseButton(GLFW_KEY_WORLD_2);
				break;
			case GLFW_KEY_ESCAPE:
				releaseButton(KEY_ESCAPE);
				break;
			case GLFW_KEY_ENTER:
				releaseButton(KEY_ENTER);
				break;
			case GLFW_KEY_TAB:
				releaseButton(KEY_TAB);
				break;
			case GLFW_KEY_BACKSPACE:
				releaseButton(KEY_BACKSPACE);
				break;
			case GLFW_KEY_INSERT:
				releaseButton(KEY_INSERT);
				break;
			case GLFW_KEY_DELETE:
				releaseButton(KEY_DELETE);
				break;
			case GLFW_KEY_RIGHT:
				releaseButton(KEY_RIGHT);
				break;
			case GLFW_KEY_LEFT:
				releaseButton(KEY_LEFT);
				break;
			case GLFW_KEY_DOWN:
				releaseButton(KEY_DOWN);
				break;
			case GLFW_KEY_UP:
				releaseButton(KEY_UP);
				break;
			case GLFW_KEY_PAGE_UP:
				releaseButton(KEY_PAGE_UP);
				break;
			case GLFW_KEY_PAGE_DOWN:
				releaseButton(KEY_PAGE_DOWN);
				break;
			case GLFW_KEY_HOME:
				releaseButton(KEY_HOME);
				break;
			case GLFW_KEY_END:
				releaseButton(KEY_END);
				break;
			case GLFW_KEY_CAPS_LOCK:
				releaseButton(KEY_CAPS_LOCK);
				break;
			case GLFW_KEY_SCROLL_LOCK:
				releaseButton(KEY_SCROLL_LOCK);
				break;
			case GLFW_KEY_NUM_LOCK:
				releaseButton(KEY_NUM_LOCK);
				break;
			case GLFW_KEY_PRINT_SCREEN:
				releaseButton(KEY_PRINT_SCREEN);
				break;
			case GLFW_KEY_PAUSE:
				releaseButton(KEY_PAUSE);
				break;
			case GLFW_KEY_F1:
				releaseButton(KEY_F1);
				break;
			case GLFW_KEY_F2:
				releaseButton(KEY_F2);
				break;
			case GLFW_KEY_F3:
				releaseButton(KEY_F3);
				break;
			case GLFW_KEY_F4:
				releaseButton(KEY_F4);
				break;
			case GLFW_KEY_F5:
				releaseButton(KEY_F5);
				break;
			case GLFW_KEY_F6:
				releaseButton(KEY_F6);
				break;
			case GLFW_KEY_F7:
				releaseButton(KEY_F7);
				break;
			case GLFW_KEY_F8:
				releaseButton(KEY_F8);
				break;
			case GLFW_KEY_F9:
				releaseButton(KEY_F9);
				break;
			case GLFW_KEY_F10:
				releaseButton(KEY_F10);
				break;
			case GLFW_KEY_F11:
				releaseButton(KEY_F11);
				break;
			case GLFW_KEY_F12:
				releaseButton(KEY_F12);
				break;
			case GLFW_KEY_F13:
				releaseButton(KEY_F13);
				break;
			case GLFW_KEY_F14:
				releaseButton(KEY_F14);
				break;
			case GLFW_KEY_F15:
				releaseButton(KEY_F15);
				break;
			case GLFW_KEY_F16:
				releaseButton(KEY_F16);
				break;
			case GLFW_KEY_F17:
				releaseButton(KEY_F17);
				break;
			case GLFW_KEY_F18:
				releaseButton(KEY_F18);
				break;
			case GLFW_KEY_F19:
				releaseButton(KEY_F19);
				break;
			case GLFW_KEY_F20:
				releaseButton(KEY_F20);
				break;
			case GLFW_KEY_F21:
				releaseButton(KEY_F21);
				break;
			case GLFW_KEY_F22:
				releaseButton(KEY_F22);
				break;
			case GLFW_KEY_F23:
				releaseButton(KEY_F23);
				break;
			case GLFW_KEY_F24:
				releaseButton(KEY_F24);
				break;
			case GLFW_KEY_F25:
				releaseButton(KEY_F25);
				break;
			case GLFW_KEY_KP_0:
				releaseButton(KEY_KP_0);
				break;
			case GLFW_KEY_KP_1:
				releaseButton(KEY_KP_1);
				break;
			case GLFW_KEY_KP_2:
				releaseButton(KEY_KP_2);
				break;
			case GLFW_KEY_KP_3:
				releaseButton(KEY_KP_3);
				break;
			case GLFW_KEY_KP_4:
				releaseButton(KEY_KP_4);
				break;
			case GLFW_KEY_KP_5:
				releaseButton(KEY_KP_5);
				break;
			case GLFW_KEY_KP_6:
				releaseButton(KEY_KP_6);
				break;
			case GLFW_KEY_KP_7:
				releaseButton(KEY_KP_7);
				break;
			case GLFW_KEY_KP_8:
				releaseButton(KEY_KP_8);
				break;
			case GLFW_KEY_KP_9:
				releaseButton(KEY_KP_9);
				break;
			case GLFW_KEY_KP_DECIMAL:
				releaseButton(KEY_KP_DECIMAL);
				break;
			case GLFW_KEY_KP_DIVIDE:
				releaseButton(KEY_KP_DIVIDE);
				break;
			case GLFW_KEY_KP_MULTIPLY:
				releaseButton(KEY_KP_MULTIPLY);
				break;
			case GLFW_KEY_KP_SUBTRACT:
				releaseButton(KEY_KP_SUBTRACT);
				break;
			case GLFW_KEY_KP_ADD:
				releaseButton(KEY_KP_ADD);
				break;
			case GLFW_KEY_KP_ENTER:
				releaseButton(KEY_KP_ENTER);
				break;
			case GLFW_KEY_KP_EQUAL:
				releaseButton(KEY_KP_EQUAL);
				break;
			case GLFW_KEY_LEFT_SHIFT:
				releaseButton(KEY_LEFT_SHIFT);
				break;
			case GLFW_KEY_LEFT_CONTROL:
				releaseButton(KEY_LEFT_CONTROL);
				break;
			case GLFW_KEY_LEFT_ALT:
				releaseButton(KEY_LEFT_ALT);
				break;
			case GLFW_KEY_LEFT_SUPER:
				releaseButton(KEY_LEFT_SUPER);
				break;
			case GLFW_KEY_RIGHT_SHIFT:
				releaseButton(KEY_RIGHT_SHIFT);
				break;
			case GLFW_KEY_RIGHT_CONTROL:
				releaseButton(KEY_RIGHT_CONTROL);
				break;
			case GLFW_KEY_RIGHT_ALT:
				releaseButton(KEY_RIGHT_ALT);
				break;
			case GLFW_KEY_RIGHT_SUPER:
				releaseButton(KEY_RIGHT_SUPER);
				break;
			case GLFW_KEY_MENU:
				releaseButton(KEY_MENU);
				break;
			default: break;
			}
		}
	}
}