#pragma once

#include <cstdint>
#include <array>
#include <list>
#include <memory>

#include <glm/glm.hpp>

// The width and height we are going to work at.  ESCAPI automatically scales
// larger images down to this.
enum { WIDTH = 1024, HEIGHT = 768 };

// Represent a pixel as a quad in rgba order.  Note that this is not the layout
// of the video input data
struct rgb_byte { uint8_t r, g, b, a; };

class frame {
	typedef std::array<uint32_t, WIDTH * HEIGHT> buffer_data_t;
	typedef std::unique_ptr<buffer_data_t> buffer_handle_t;
	buffer_handle_t buffer;

public:
	frame() 
		: buffer(new buffer_data_t()) { }

	frame(const frame& f) 
		: buffer(new buffer_data_t(*f.buffer)) { }

	frame& operator=(const frame& f) {
		buffer.reset(new buffer_data_t(*f.buffer));
		return *this;
	}

	// Returns a pointer to the beginning of a pixel in memory.
	// Pixels are laid out in BGRA format.  A is not defined.
	uint8_t* get_pixel(int x, int y) {
		return reinterpret_cast<uint8_t*>(&(*buffer)[WIDTH * (HEIGHT - y - 1) + x]);
	}

	// Allow const methods to get a read-only copy of the data
	const uint8_t* get_pixel(int x, int y) const {
		return const_cast<frame*>(this)->get_pixel(x, y);
	}

	// Return a pointer to the underlying data.  Be careful with this!
	void* get_buffer_data() {
		return &(*buffer)[0];
	}
};

// represents a sliding window of points that we weight so that the most
// recent is the most important
class sliding_window {
	enum { SIZE = 10 };
	std::list<glm::ivec2> history;

public:
	void push_value(glm::ivec2 p) {
		history.push_back(p);
		if(history.size() > SIZE) {
			history.pop_front();
		}
	}

	glm::ivec2 value() const {
		glm::vec2 tmp(0);
		int factor = 1;
		for(auto& p : history) {
			tmp += glm::vec2(p) / pow(2.0f, factor++);
		}

		return glm::ivec2(tmp);
	}
};


inline bool is_black(const uint8_t* p) {
	return !(p[0] || p[1] || p[2]);
}

inline void blacken(uint8_t* p) {
	p[0] = p[1] = p[2] = 0;
}