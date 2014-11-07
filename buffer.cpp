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

	~Impl()
	{}

	void init(std::vector<std::vector<float>> && samples, int sample_rate)
	{
		format_ = Format(samples.size(), Format::Type::FLOAT, sample_rate);
		samples_float_ = std::move(samples);
	}

	void init(std::vector<std::vector<int16_t>> && samples, int sample_rate)
	{
		format_ = Format(samples.size(), Format::Type::PCM, sample_rate);
		samples_int16_ = std::move(samples);
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

	size_t size() const
	{
		if (format_.type() == Format::Type::PCM) {
			return samples_int16_[0].size();
		} else if (format_.type() == Format::Type::FLOAT) {
			return samples_float_[0].size();
		}
		return 0;
	}

private:
	std::vector<std::vector<float>> samples_float_;
	std::vector<std::vector<int16_t>> samples_int16_;
	Format format_;
};


Buffer::Buffer(std::vector<float> && monoral_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<float>> buffer;
	buffer.push_back(monoral_samples);
	impl_->init(std::move(buffer), sample_rate);
}


Buffer::Buffer(std::vector<int16_t> && monoral_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<int16_t>> buffer;
	buffer.push_back(std::move(monoral_samples));
	impl_->init(std::move(buffer), sample_rate);
}


Buffer::Buffer(std::vector<std::vector<float>> && samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	impl_->init(std::move(samples), sample_rate);
}


Buffer::Buffer(std::vector<std::vector<int16_t>> && samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	impl_->init(std::move(samples), sample_rate);
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


size_t Buffer::size() const
{
	return impl_->size();
}
}
