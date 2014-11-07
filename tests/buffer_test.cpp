#include "gtest/gtest.h"
#include "../buffer.hpp"
#include <algorithm>

TEST(Buffer, init_with_float32_44100_monoral)
{
	using namespace WaveFile;

	std::vector<float> buffer;
	buffer.push_back(0);
	buffer.push_back(0.1);
	buffer.push_back(0.2);
	buffer.push_back(0.3);
	buffer.push_back(0.4);
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

	std::vector<int16_t> buffer;
	buffer.push_back(0);
	buffer.push_back(100);
	buffer.push_back(200);
	buffer.push_back(400);
	buffer.push_back(500);
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
