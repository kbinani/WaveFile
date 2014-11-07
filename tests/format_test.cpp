#include "gtest/gtest.h"
#include "../format.hpp"


TEST(FormatTest, default)
{
	using namespace WaveFile;
	Format format;
	EXPECT_EQ(2, format.channels());
	EXPECT_EQ(Format::Type::PCM, format.type());
	EXPECT_EQ(44100, format.sample_rate());
}


TEST(FormatTest, constructor)
{
	using namespace WaveFile;
	Format format(3, Format::Type::FLOAT, 48000);
	EXPECT_EQ(3, format.channels());
	EXPECT_EQ(Format::Type::FLOAT, format.type());
	EXPECT_EQ(48000, format.sample_rate());
}
