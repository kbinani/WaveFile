#include "gtest/gtest.h"
#include "../buffer.hpp"
#include <algorithm>

TEST(Buffer, init_with_float32_44100_monoral)
{
	using namespace WaveFile;

	std::vector<float> buffer = {0, 0.1, 0.2, 0.3, 0.4};
	Buffer b(std::move(buffer), 44100);

	Format format = b.format();
	EXPECT_EQ(1, format.channels());
	EXPECT_EQ(44100, format.sample_rate());
	EXPECT_EQ(Format::Type::FLOAT, format.type());

	{
		std::vector<float> a = b.samples<float>();
		EXPECT_EQ(5, a.size());
		EXPECT_EQ(0, a[0]);
		EXPECT_EQ(0.4f, a[4]);
	}
	{
		std::vector<int16_t> a = b.samples<int16_t>();
		EXPECT_EQ(5, a.size());
		EXPECT_EQ(0, a[0]);
		EXPECT_EQ(13107, a[4]);
	}
}


TEST(Buffer, init_with_int16_44100_monoral)
{
	using namespace WaveFile;

	std::vector<int16_t> buffer = {0, 100, 200, 400, 500};
	Buffer b(std::move(buffer), 44100);

	Format format = b.format();
	EXPECT_EQ(1, format.channels());
	EXPECT_EQ(44100, format.sample_rate());
	EXPECT_EQ(Format::Type::PCM, format.type());

	{
		std::vector<float> a = b.samples<float>();
		EXPECT_EQ(5, a.size());
		EXPECT_EQ(0, a[0]);
		EXPECT_EQ(500 / 32768.f, a[4]);
	}
	{
		std::vector<int16_t> a = b.samples<int16_t>();
		EXPECT_EQ(5, a.size());
		EXPECT_EQ(0, a[0]);
		EXPECT_EQ(500, a[4]);
	}
}
