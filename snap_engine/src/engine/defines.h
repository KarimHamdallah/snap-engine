#ifndef DEFINES_H
#define DEFINES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

typedef unsigned int gl_id;

typedef struct _color
{
	f32 r;
	f32 g;
	f32 b;
	f32 a;
}color;

#define RGB_COLOR(r,g,b) color{r/255.0f, g/255.0f, b/255.0f, 1.0f}
#define RGBA_COLOR(r,g,b,a) color{r/255.0f, g/255.0f, b/255.0f, a/255.0f}

#define WHITE_FADED color{1.0f, 1.0f, 1.0f, 0.3f}
#define WHITE color{1.0f, 1.0f, 1.0f, 1.0f}
#define RED color{1.0f, 0.0f, 0.0f, 1.0f}
#define GREEN color{0.0f, 1.0f, 0.0f, 1.0f}
#define BLUE color{0.0f, 0.0f, 1.0f, 1.0f}
#define YELLOW color{1.0f, 1.0f, 0.0f, 1.0f}
#define CYAN color{0.8f, 1.0f, 1.0f, 1.0f}

#endif // !DEFINES_H


#ifndef RANDOM_H
#define RANDOM_H

#include <chrono>
#include <random>

class Random {
	std::mt19937 _rng;
public:
	Random() {
		auto duration = std::chrono::system_clock::now().time_since_epoch();
		auto seed = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
		_rng.seed(seed);
	}
	Random(unsigned int seed) { _rng.seed(seed); }

	float operator()(float max = 1.0f) {
		std::uniform_real_distribution<float> dist(0.0f, max);
		return dist(_rng);
	}
	float operator()(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(_rng);
	}

	int operator()(int max = std::mt19937::max()) {
		std::uniform_int_distribution<int> dist(0, max);
		return dist(_rng);
	}
	int operator()(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(_rng);
	}

	static Random& get() { static Random rng; return rng; }
};

#define random (Random::get())

#endif