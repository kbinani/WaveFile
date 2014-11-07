#include "./buffer.hpp"
#include <memory>
#include <cassert>

namespace WaveFile
{

class Buffer::Impl
{
public:
	Impl()
	{}

	void init(std::vector<float> const& monoral_samples, int sample_rate)
	{
		format_ = Format(1, Format::Type::FLOAT, sample_rate);
		samples_float_.push_back(monoral_samples);
	}

	void init(std::vector<int16_t> const& monoral_samples, int sample_rate)
	{
		format_ = Format(1, Format::Type::PCM, sample_rate);
		samples_int16_.push_back(monoral_samples);
	}

	Format format() const
	{
		return format_;
	}

	std::vector<float> samples_f(int channel) const
	{
		if (format().type() == Format::Type::FLOAT) {
			assert(0 <= channel && channel < samples_float_.size());
			return samples_float_[channel];
		} else {
			assert(0 <= channel && channel < samples_int16_.size());
			std::vector<float> result;
			result.resize(samples_int16_[channel].size());
			for (size_t i = 0; i < samples_int16_[channel].size(); ++i) {
				result[i] = samples_int16_[channel][i] / 32768.0f;
			}
			return result;
		}
	}

	std::vector<int16_t> samples_s16(int channel) const
	{
		if (format().type() == Format::Type::PCM) {
			assert(0 <= channel && channel < samples_int16_.size());
			return samples_int16_[channel];
		} else {
			assert(0 <= channel && channel < samples_float_.size());
			std::vector<int16_t> result;
			result.resize(samples_float_[channel].size());
			for (size_t i = 0; i < samples_float_[channel].size(); ++i) {
				result[i] = (int16_t)(samples_float_[channel][i] * 32768);
			}
			return result;
		}
	}

private:
	std::vector<std::vector<float>> samples_float_;
	std::vector<std::vector<int16_t>> samples_int16_;
	Format format_;
};


Buffer::Buffer(std::vector<float> const& monoral_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	impl_->init(monoral_samples, sample_rate);
}


Buffer::Buffer(std::vector<int16_t> const& monoral_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	impl_->init(monoral_samples, sample_rate);
}


Buffer::~Buffer()
{}


Format Buffer::format() const
{
	return impl_->format();
}


template<>
std::vector<float> Buffer::samples<float>(int channel) const
{
	return impl_->samples_f(channel);
}


template<>
std::vector<int16_t> Buffer::samples<int16_t>(int channel) const
{
	return impl_->samples_s16(channel);
}

}
