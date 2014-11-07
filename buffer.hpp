#pragma once

#include "./format.hpp"
#include <vector>
#include <memory>

namespace WaveFile
{

class Buffer
{
public:
	Buffer(std::vector<float> && monoral_samples, int sample_rate);
	Buffer(std::vector<int16_t> && monoral_samples, int sample_rate);
	Buffer(std::vector<std::vector<float>> && samples, int sample_rate);
	Buffer(std::vector<std::vector<int16_t>> && samples, int sample_rate);

	~Buffer();

	Format format() const;

	template<class T>
	std::vector<T> samples(int channel = 0) const;

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

}
