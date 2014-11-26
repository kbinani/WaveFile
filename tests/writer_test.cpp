#include "gtest/gtest.h"
#include "../writer.hpp"
#include "../buffer.hpp"
#include <memory>
#include <cmath>

TEST(WriterTest, write)
{
	using namespace WaveFile;

	std::string output("a.wav");
	int const sample_rate = 44100;
	Format format(2, Format::Type::PCM, sample_rate);
	Writer writer(output, format);
	std::vector<float> data;
	double const kHz = 440;
	double const kPI = 4 * std::atan(1.0);
	for (int i = 0; i < 10 * sample_rate; ++i) {
		double t = i / (double)sample_rate;
		double v = 0.2 * std::sin(t * 2 * kPI * kHz);
		data.push_back((float)v);
	}
	Buffer buffer(std::move(data), format.sample_rate());
	writer.write(buffer);
}
