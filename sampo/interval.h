//schedaler/schemas/interval -
#ifndef INTERVAL_H
#define INTERVAL_H

#include <utility>
#include <vector>
#include <random>
#include <cassert>
#include <stdlib.h>

using namespace std;

//float EPS = 0.00001;
//float INF = 3.4e38;
//float MINUS_INF = -3.4e38;

struct PRNG {
	std::mt19937 engine;
};

class IntervalUniform {
private:
	float EPS = 0.00001;
	float INF = 3.4e38;
	float MINUS_INF = -3.4e38;

	float min_val = MINUS_INF;
	float max_val = INF;
	float rand;
public:
	IntervalUniform(float min_val, float max_val) {
		rand = 0;
		this->min_val = min_val;
		this->max_val = max_val;
	}
	float get_min_val() {
		return min_val;
	}
	float get_max_val() {
		return max_val;
	}
	/*void set_min_val(float value) {
		min_val = value;
	}
	void set_max_val(float value) {
		max_val = value;
	}*/
	float rand_float()								//íàäî óëó÷øèòü
	{
		//srand(time(NULL));
		PRNG generator;
		std::random_device device;
		generator.engine.seed(device());
		//std::mt19937 rnd(time(NULL));

		assert(min_val < max_val);

		std::uniform_real_distribution<float> distribution(min_val, max_val);

		return distribution(generator.engine);
	}
	unsigned rand_int()										//удобнее не трогать объект
	{
		PRNG generator;
		std::random_device device;
		generator.engine.seed(device());
		//std::mt19937 rnd(2);

		assert(min_val < max_val);

		std::uniform_int_distribution<unsigned> distribution(min_val, max_val);
		int value = distribution(generator.engine);

		if (value < int(min_val - EPS))
			value = int(min_val - EPS);
		if (value > int(max_val + EPS))
			value = int(max_val + EPS);

		return value;
	}
};

class IntervalGaussian {
//private:
public:
    constexpr static const double EPS = 1e5;

    random_device rd{};
    mt19937 gen{ rd() };
    normal_distribution<> d;

    float min_val;
    float max_val;
	float mean;
	float sigma;
public:
    explicit IntervalGaussian(float mean, float sigma, float min_val, float max_val)
        : min_val(min_val), max_val(max_val), d(normal_distribution<>{mean, sigma}) {}

    IntervalGaussian(const IntervalGaussian& other)
        : IntervalGaussian((float)other.d.mean(), (float)other.d.stddev(), other.min_val, other.max_val) {}

    float randFloat() {
        return (float)d(gen);
    }

    int randInt() {
        int value = (int)round(randFloat());
        value = max(value, int(min_val - EPS));
        value = min(value, int(max_val + EPS));
        return value;
    }
};

#endif