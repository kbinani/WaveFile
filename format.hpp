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

private:
	int channels_;
	Type type_;
	int sample_rate_;
};

} // namespace WaveFile
