#include "input.h"
#include <SDL2/SDL.h>
#include <engine/logger/asserts.h>
#include "keys.h"

static void *m_window = nullptr;
static bool m_init = false;
static bool key_repeat = false;

InputSystem& InputSystem::getInstance()
{
	static InputSystem i;
	return i;
}

void InputSystem::init(std::shared_ptr<window> _window)
{
	m_init = true;

	_window->getInstance()->setWindowKeyCallbackPointer(
		[](i32 key, i32 scancode, u8 action, u16 mods, u8 repeat)
	{
		Input::setKeyState(static_cast<Key>(key), static_cast<KeyState>(action));
		
		if (repeat == 0)
			key_repeat = false;
		else
			key_repeat = true;

		// key_Changed = action != repeat
		bool key_Changed = !repeat;
		Input::setKeyChanged(static_cast<Key>(key), key_Changed);
	});
	
	_window->getInstance()->setWindowCursorPosCallbackPointer(
		[](f32 xpos, f32 ypos, f32 xrel, f32 yrel)
	{
		Input::setMousePos(xpos, ypos, xrel, yrel);
	});

	_window->getInstance()->setWindowMouseScrollCallbackPointer(
		[](f32 scrollx, f32 scrolly)
	{
		Input::setMouseScroll(scrollx, scrolly);
	});

	_window->getInstance()->setWindowMouseButtonCallbackPointer(
		[](u8 button, u8 state)
	{
		Input::setMouseButtonState(static_cast<MouseButton>(button), static_cast<ButtonState>(state));

		// button pressed this frame
		Input::setButtonPressedThisFrame(static_cast<MouseButton>(button), static_cast<ButtonState>(state) == ButtonState::ButtonPressed);
	});
}

void InputSystem::cleanup()
{
	m_init = false;
}

bool InputSystem::isKeyPressed(Key keycode)
{
	return getKeyState(keycode) == KeyState::KeyPressed;
}

bool InputSystem::isKeyReleased(Key keycode)
{
	return getKeyState(keycode) == KeyState::KeyReleased;
}

bool InputSystem::isKeyHeld(Key keycode)
{
	KeyState state = getKeyState(keycode);
	return state == key_repeat && state == KeyState::KeyPressed;
}

bool InputSystem::isKeyChanged(Key keycode)
{
	// get changed state
	std::unordered_map<Key, bool>::iterator state = m_keyscahnged.find(keycode);
	bool ret;
	if (state == m_keyscahnged.end())
		ret = false;
	else
		ret = state->second;

	// reset key changed
	InputSystem::setKeyChanged(keycode, false);

	return ret;
}

bool InputSystem::isKeyDown(Key keycode)
{
	return isKeyChanged(keycode) && isKeyPressed(keycode);
}

bool InputSystem::isButtonPressed(MouseButton button)
{
	return getMouseButtonState(button) == ButtonState::ButtonPressed;
}

bool InputSystem::isButtonReleased(MouseButton button)
{
	return getMouseButtonState(button) == ButtonState::ButtonReleased;
}

bool InputSystem::isButtonDown(MouseButton button)
{
	// get pressed this frame
	std::unordered_map<MouseButton, bool>::iterator pressed_this_frame = m_mouseButtonsPressedThisFrame.find(button);
	bool is_pressed_this_frame;
	if (pressed_this_frame == m_mouseButtonsPressedThisFrame.end())
		is_pressed_this_frame = false;
	else
		is_pressed_this_frame = pressed_this_frame->second;

	// reset button pressed this frame
	InputSystem::setButtonPressedThisFrame(button, false);

	return is_pressed_this_frame && isButtonPressed(button);
}

KeyState InputSystem::getKeyState(Key keycode)
{
	std::unordered_map<Key, KeyState>::iterator state = m_keys.find(keycode);
	if (state == m_keys.end())
	{
		return KeyState::KeyReleased;
	}
	return state->second;
}

ButtonState InputSystem::getMouseButtonState(MouseButton button)
{
	std::unordered_map<MouseButton, ButtonState>::iterator state = m_mouseButtons.find(button);
	if (state == m_mouseButtons.end())
	{
		return ButtonState::ButtonReleased;
	}
	return state->second;
}

glm::vec2 InputSystem::getMousePos()
{
	return glm::vec2(m_mouseX, m_mouseY);
}

glm::vec2 InputSystem::getMouseScroll()
{
	return glm::vec2(m_scrollX, m_scrollY);
}

glm::vec2 InputSystem::getMouseRelativePos()
{
	return glm::vec2(m_mouse_relativeX, m_mouse_relativeY);
}

void InputSystem::setKeyState(Key keycode, KeyState state)
{
	m_keys.insert_or_assign(keycode, state);
}

void InputSystem::setMouseButtonState(MouseButton button, ButtonState state)
{
	m_mouseButtons.insert_or_assign(button, state);
}

void InputSystem::setKeyChanged(Key keycode, bool changed)
{
	m_keyscahnged.insert_or_assign(keycode, changed);
}
void InputSystem::setButtonPressedThisFrame(MouseButton button, bool pressed_this_frame)
{
	m_mouseButtonsPressedThisFrame.insert_or_assign(button, pressed_this_frame);
}

void InputSystem::setMousePos(f32 x_pos, f32 y_pos, f32 x_rel, f32 y_rel)
{
	m_mouseX = x_pos;
	m_mouseY = y_pos;
	m_mouse_relativeX = x_rel;
	m_mouse_relativeY = y_rel;
}

void InputSystem::setMouseScroll(f32 x, f32 y)
{
	m_scrollX = x;
	m_scrollY = y;
}

bool Input::isKeyPressed(Key keycode)
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().isKeyPressed(keycode);
}

bool Input::isKeyReleased(Key keycode)
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().isKeyReleased(keycode);
}

bool Input::isKeyHeld(Key keycode)
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().isKeyHeld(keycode);
}

bool Input::isKeyChanged(Key keycode)
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().isKeyChanged(keycode);
}

bool Input::isKeyDown(Key keycode)
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().isKeyDown(keycode);
}

bool Input::isButtonPressed(MouseButton button)
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().isButtonPressed(button);
}

bool Input::isButtonReleased(MouseButton button)
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().isButtonReleased(button);
}

bool Input::isButtonDown(MouseButton button)
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().isButtonDown(button);
}

void Input::setKeyState(Key keycode, KeyState state)
{
	InputSystem::getInstance().setKeyState(keycode, state);
}

void Input::setKeyChanged(Key keycode, bool changed)
{
	InputSystem::getInstance().setKeyChanged(keycode, changed);
}

void Input::setMouseButtonState(MouseButton button, ButtonState state)
{
	InputSystem::getInstance().setMouseButtonState(button, state);
}
void Input::setButtonPressedThisFrame(MouseButton button, bool pressed_this_frame)
{
	InputSystem::getInstance().setButtonPressedThisFrame(button, pressed_this_frame);
}

glm::vec2 Input::getMousePos()
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().getMousePos();
}

glm::vec2 Input::getMouseScroll()
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().getMouseScroll();
}

glm::vec2 Input::getMouseRelativePos()
{
	LOG_ASSERT_MSG(m_init, "input utility is called before initialization of input system");
	return InputSystem::getInstance().getMouseRelativePos();
}

void Input::show_cursor(bool show)
{
	SDL_ShowCursor(show);
}

void Input::setMousePos(f32 x_pos, f32 y_pos, f32 x_rel, f32 y_rel)
{
	InputSystem::getInstance().setMousePos(x_pos, y_pos, x_rel, y_rel);
}

void Input::setMouseScroll(f32 x, f32 y)
{
	InputSystem::getInstance().setMouseScroll(x, y);
}