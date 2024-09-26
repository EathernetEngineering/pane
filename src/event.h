#ifndef PANE_EVENT_H_
#define PANE_EVENT_H_

#include <cstdint>

namespace pane {
enum EventClass {
	EVENT_CLASS_APPLICATION = 1 << 0,
	EVENT_CLASS_WINDOW      = 1 << 1,
	EVENT_CLASS_KEYBOARD    = 1 << 2,
	EVENT_CLASS_MOUSE       = 1 << 3
};

enum class EventType : uint8_t {
	EVENT_TYPE_APPLICATION_TICK = 0, EVENT_TYPE_APPLICATION_EXIT,
	EVENT_TYPE_WINDOW_RESIZE, EVENT_TYPE_WINDOW_POSITION, EVENT_TYPE_WINDOW_CLOSE, EVENT_TYPE_WINDOW_FOCUS, EVENT_TYPE_WINDOW_UNFOCUS,
	EVENT_TYPE_KEYBOARD_KEY_DOWN, EVENT_TYPE_KEYBOARD_KEY_UP, EVENT_TYPE_KEYBOARD_KEY_REPEAT,
	EVENT_TYPE_MOUSE_MOVE, EVENT_TYPE_MOUSE_BUTTON_DOWN, EVENT_TYPE_MOUSE_BUTTON_UP, EVENT_TYPE_MOUSE_SCROLL
};

class Event {
public:
	virtual EventClass GetClass() = 0;
	virtual EventType GetType() = 0;
};

class ApplicationTickEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_APPLICATION; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_APPLICATION_TICK; };
};

class ApplicationExitEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_APPLICATION; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_APPLICATION_EXIT; };
};

class WindowResizeEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_WINDOW; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_WINDOW_RESIZE; };

public:
	WindowResizeEvent(uint32_t nWidth, uint32_t nHeight)
	 : m_nWidth(nWidth), m_nHeight(nHeight)
	{ }

	uint32_t GetWidth() const { return m_nWidth; }
	uint32_t GetHeight() const { return m_nHeight; }

protected:
	uint32_t m_nWidth, m_nHeight;
};

class WindowPositionEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_WINDOW; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_WINDOW_POSITION; };

public:
	WindowPositionEvent(int32_t nX, int32_t nY)
	 : m_nX(nX), m_nY(nY)
	{ }
 
	int32_t GetX() const { return m_nX; }
	int32_t GetY() const { return m_nY; }

protected:
	int32_t m_nX, m_nY;
};

class WindowCloseEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_WINDOW; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_WINDOW_CLOSE; };
};

class WindowFocusEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_WINDOW; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_WINDOW_FOCUS; };
};

class WindowUnfocusEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_WINDOW; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_WINDOW_UNFOCUS; };
};

class KeyDownEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_KEYBOARD; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_KEYBOARD_KEY_DOWN; };

public:
	KeyDownEvent(uint32_t uKeycode)
	 : m_uKeycode(uKeycode)
	{ }

	uint32_t GetKeycode() const { return m_uKeycode; }

protected:
	uint32_t m_uKeycode;
};

class KeyUpEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_KEYBOARD; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_KEYBOARD_KEY_UP; };

public:
	KeyUpEvent(uint32_t uKeycode)
	 : m_uKeycode(uKeycode)
	{ }

	uint32_t GetKeycode() const { return m_uKeycode; }

protected:
	uint32_t m_uKeycode;
};

class KeyRepeatEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_KEYBOARD; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_KEYBOARD_KEY_REPEAT; };

public:
	KeyRepeatEvent(uint32_t uKeycode)
	 : m_uKeycode(uKeycode)
	{ }

	uint32_t GetKeycode() const { return m_uKeycode; }

protected:
	uint32_t m_uKeycode;
};

class MouseMoveEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_MOUSE; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_MOUSE_MOVE; };

public:
	MouseMoveEvent(double nX, double nY)
	 : m_nX(nX), m_nY(nY)
	{ }

	double GetX() const { return m_nX; }
	double GetY() const { return m_nY; }

protected:
	double m_nX, m_nY;
};

class ButtonDownEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_MOUSE; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_MOUSE_BUTTON_DOWN; };

public:
	ButtonDownEvent(uint32_t uMousecode)
	 : m_uMousecode(uMousecode)
	{ }

	uint32_t GetMousecode() const { return m_uMousecode; }

protected:
	uint32_t m_uMousecode;
};

class ButtonUpEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_MOUSE; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_MOUSE_BUTTON_UP; };

public:
	ButtonUpEvent(uint32_t uMousecode)
	 : m_uMousecode(uMousecode)
	{ }

	uint32_t GetMousecode() const { return m_uMousecode; }

protected:
	uint32_t m_uMousecode;
};

class ScrollEvent : public Event {
public:
	virtual EventClass GetClass() { return EVENT_CLASS_MOUSE; };
	virtual EventType GetType() { return EventType::EVENT_TYPE_MOUSE_SCROLL; };

public:
	ScrollEvent(double nX, double nY)
	 : m_nX(nX), m_nY(nY)
	{ }

	double GetX() const { return m_nX; }
	double GetY() const { return m_nY; }

protected:
	double m_nX, m_nY;
};
}

#endif

