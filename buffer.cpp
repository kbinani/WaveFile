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
		unsigned int const channels = static_cast<unsigned int>(samples.size());
		format_ = Format(channels, Format::Type::FLOAT, sample_rate);
		samples_float_ = std::move(samples);
	}

	void init(std::vector<std::vector<std::int16_t>> && samples, int sample_rate)
	{
		unsigned int const channels = static_cast<unsigned int>(samples.size());
		format_ = Format(channels, Format::Type::PCM, sample_rate);
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
			for (std::size_t i = 0; i < samples_int16_[channel].size(); ++i) {
				result[i] = samples_int16_[channel][i] / kFloatToInt16;
			}
			return std::move(result);
		}
	}

	std::vector<std::int16_t> samples_s16(int channel) const
	{
		if (format().type() == Format::Type::PCM) {
			assert(0 <= channel && channel < samples_int16_.size());
			return samples_int16_[channel];
		} else {
			assert(0 <= channel && channel < samples_float_.size());
			std::vector<std::int16_t> result;
			result.resize(samples_float_[channel].size());
			for (std::size_t i = 0; i < samples_float_[channel].size(); ++i) {
				float const v = std::max(-1.0f, std::min(1.0f, samples_float_[channel][i]));
				result[i] = (std::int16_t)(v * kFloatToInt16);
			}
			return std::move(result);
		}
	}

	std::size_t size() const
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
	std::vector<std::vector<std::int16_t>> samples_int16_;
	Format format_;

private:
	static float const kFloatToInt16;
};

	
float const Buffer::Impl::kFloatToInt16 = 32768.0f;


Buffer::Buffer(std::vector<float> && monoral_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<float>> buffer;
	buffer.push_back(monoral_samples);
	impl_->init(std::move(buffer), sample_rate);
}


Buffer::Buffer(std::vector<std::int16_t> && monoral_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<std::int16_t>> buffer;
	buffer.push_back(std::move(monoral_samples));
	impl_->init(std::move(buffer), sample_rate);
}


Buffer::Buffer(std::vector<std::vector<float>> && samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	impl_->init(std::move(samples), sample_rate);
}


Buffer::Buffer(std::vector<std::vector<std::int16_t>> && samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	impl_->init(std::move(samples), sample_rate);
}

	
Buffer::Buffer(std::vector<float> const& monoral_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<float>> buffer;
	buffer.push_back(monoral_samples);
	impl_->init(std::move(buffer), sample_rate);
}

	
Buffer::Buffer(std::vector<std::int16_t> const& monoral_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<std::int16_t>> buffer;
	buffer.push_back(monoral_samples);
	impl_->init(std::move(buffer), sample_rate);
}

	
Buffer::Buffer(std::vector<std::vector<float>> const& samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<float>> copy = samples;
	impl_->init(std::move(copy), sample_rate);
}

	
Buffer::Buffer(std::vector<std::vector<std::int16_t>> const& samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<std::int16_t>> copy = samples;
	impl_->init(std::move(copy), sample_rate);
}

	
Buffer::Buffer(float const* monoral_samples, size_t num_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<float>> buffer;
	buffer.push_back(std::vector<float>());

	assert(monoral_samples);
	if (monoral_samples) {
		buffer[0].assign(monoral_samples, monoral_samples + num_samples);
	}

	impl_->init(std::move(buffer), sample_rate);
}


Buffer::Buffer(std::int16_t const* monoral_samples, size_t num_samples, int sample_rate)
	: impl_(std::make_unique<Impl>())
{
	std::vector<std::vector<std::int16_t>> buffer;
	buffer.push_back(std::vector<std::int16_t>());

	assert(monoral_samples);
	if (monoral_samples) {
		buffer[0].assign(monoral_samples, monoral_samples + num_samples);
	}

	impl_->init(std::move(buffer), sample_rate);
}


Buffer::Buffer(Buffer && other)
{
	impl_.swap(other.impl_);
}


Buffer & Buffer::operator = (Buffer && other)
{
	impl_.swap(other.impl_);
	return *this;
}


Buffer::~Buffer()
{}


Format Buffer::format() const
{
	return impl_->format();
}


template<>
std::vector<float>
Buffer::samples<float>(int channel) const
{
	return std::move(impl_->samples_f(channel));
}


template<>
std::vector<std::int16_t>
Buffer::samples<std::int16_t>(int channel) const
{
	return std::move(impl_->samples_s16(channel));
}


size_t Buffer::size() const
{
	return impl_->size();
}
	
} // namespace WaveFile
