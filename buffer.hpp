#pragma once

#include "./format.hpp"
#include <vector>
#include <memory>

namespace WaveFile
{

class Buffer
{
public:
	Buffer(std::vector<float> const& monoral_samples, int sample_rate);
	Buffer(std::vector<int16_t> const& monoral_samples, int sample_rate);

	~Buffer();

	Format format() const;

	template<class T>
	std::vector<T> samples(int channel = 0) const;

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

}
