#pragma once

namespace WaveFile
{

class Format
{
public:
	enum class Type
	{
		PCM,
		FLOAT,
	};

public:
	explicit Format(int channels = 2, Type type = Type::PCM, int sample_rate = 44100)
		: channels_(channels)
		, type_(type)
		, sample_rate_(sample_rate)
		, bit_per_sample_(16)
	{}

	int channels() const
	{
		return channels_;
	}

	Type type() const
	{
		return type_;
	}

	int sample_rate() const
	{
		return sample_rate_;
	}

	int bit_per_sample() const
	{
		return bit_per_sample_;
	}

	int data_rate() const
	{
		return channels_ * sample_rate_ * bit_per_sample_ / 8;
	}

	int block_size() const
	{
		return channels_ * bit_per_sample_ / 8;
	}

private:
	int channels_;
	Type type_;
	int sample_rate_;
	int bit_per_sample_;
};

} // namespace WaveFile
