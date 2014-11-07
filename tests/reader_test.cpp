#include "gtest/gtest.h"
#include "../reader.hpp"
#include <iostream>

TEST(ReaderTest, read)
{
	using namespace WaveFile;
	Reader reader("test.wav");
	Format format = reader.format();
	EXPECT_EQ(2, format.channels());
	EXPECT_EQ(44100, format.sample_rate());

	std::unique_ptr<Buffer> buffer = reader.read(100);
	std::vector<float> left = buffer->samples<float>(0);
	ASSERT_FLOAT_EQ(-0.000213623f, left[0]);
	ASSERT_FLOAT_EQ(-0.10754395f, left[61]);
	ASSERT_FLOAT_EQ(-0.070922852f, left[99]);

	std::vector<float> right = buffer->samples<float>(1);
	ASSERT_FLOAT_EQ(-0.0012207031f, right[1]);
	ASSERT_FLOAT_EQ(0.049835205f, right[39]);
	ASSERT_FLOAT_EQ(-0.026062012f, right[68]);
}
