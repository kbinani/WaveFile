#include "./reader.hpp"
#include "./buffer.hpp"

#include <cstdio>
#include <string>

namespace WaveFile
{

class Reader::Impl
{
	enum class FormatCode : uint16_t
	{
		Unknown = 0,
		PCM = 1,
	};

public:
	Impl(std::string const& path)
		: file_(nullptr)
		, total_samples_(0)
		, valid_(false)
	{
		file_ = fopen(path.c_str(), "rb");
		if (file_) {
			if (read_header()) {
				valid_ = true;
			} else {
				close();
			}
		}
	}

	~Impl()
	{
		close();
	}

	void close()
	{
		if (file_) {
			fclose(file_);
			file_ = nullptr;
		}
		valid_ = false;
	}

	bool read_header()
	{
		// RIFF header
		char riff[5];
		riff[4] = 0;
		if (fread(riff, sizeof(char), 4, file_) != 4) {
			return false;
		}
		if (std::string(riff) != std::string("RIFF")) {
			return false;
		}

		// file size
		uint32_t file_size;
		if (fread(&file_size, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}

		// "WAVE" header
		char wave[5];
		wave[4] = 0;
		if (fread(wave, sizeof(char), 4, file_) != 4) {
			return false;
		}
		if (std::string(wave) != std::string("WAVE")) {
			return false;
		}

		// fmt header
		char fmt[5];
		fmt[4] = 0;
		if (fread(fmt, sizeof(char), 4, file_) != 4) {
			return false;
		}
		if (std::string(fmt) != std::string("fmt ")) {
			return false;
		}

		// "fmt" chunk size
		uint32_t fmt_chunk_size;
		if (fread(&fmt_chunk_size, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}
		long const fmt_chunk_end_pos = ftell(file_) + fmt_chunk_size;

		// format ID
		uint16_t format_id;
		if (fread(&format_id, sizeof(uint16_t), 1, file_) != 1) {
			return false;
		}
		if (format_id != static_cast<uint16_t>(FormatCode::PCM)) {
			return false;
		}

		uint16_t channels;
		if (fread(&channels, sizeof(uint16_t), 1, file_) != 1) {
			return false;
		}

		// sample rate
		uint32_t sample_rate;
		if (fread(&sample_rate, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}

		uint32_t data_rate;
		if (fread(&data_rate, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}

		uint16_t block_size;
		if (fread(&block_size, sizeof(uint16_t), 1, file_) != 1) {
			return false;
		}

		uint16_t bit_per_sample;
		if (fread(&bit_per_sample, sizeof(uint16_t), 1, file_) != 1) {
			return false;
		}

		// seek to the end of "fmt " chunk
		if (ftell(file_) != fmt_chunk_end_pos) {
			if (fseek(file_, fmt_chunk_end_pos, SEEK_SET) != 0) {
				return false;
			}
		}

		// "data" header
		uint32_t size_of_data_chunk = 0;
		while (true) {
			char data[5];
			data[4] = 0;
			if (fread(data, sizeof(char), 4, file_) != 4) {
				return false;
			}
			uint32_t chunk_size = 0;
			if (fread(&chunk_size, sizeof(uint32_t), 1, file_) != 1) {
				return false;
			}
			if (std::string(data) == std::string("data")) {
				size_of_data_chunk = chunk_size;
				break;
			}
			if (fseek(file_, chunk_size, SEEK_CUR) != 0) {
				return false;
			}
		}

		format_ = Format(channels, Format::Type::PCM, sample_rate);
		int const kBitPerByte = 8;
		total_samples_ = size_of_data_chunk / channels * kBitPerByte / bit_per_sample;

		return true;
	}

	Buffer read(int sample_frames)
	{
		std::vector<std::vector<int16_t>> result;
		if (!file_ || sample_frames <= 0) {
			Buffer b(std::move(result), format_.sample_rate());
			return std::move(b);
		}
		for (int i = 0; i < format_.channels(); ++i) {
			result.push_back(std::vector<int16_t>(sample_frames));
		}
		for (int i = 0; i < sample_frames; ++i) {
			for (int ch = 0; ch < format_.channels(); ++ch) {
				int16_t v;
				if (fread(&v, sizeof(int16_t), 1, file_) != 1) {
					Buffer b(std::move(result), format_.sample_rate());
					return std::move(b);
				}
				result[ch][i] = v;
			}
		}
		Buffer b(std::move(result), format_.sample_rate());
		return std::move(b);
	}

	Format format() const
	{
		return format_;
	}

	bool is_valid() const
	{
		return file_ != nullptr;
	}

	size_t total_samples() const
	{
		return total_samples_;
	}

private:
	Format format_;
	FILE *file_;
	size_t total_samples_;
	bool valid_;
};


Reader::Reader(std::string const& path)
	: impl_(new Impl(path))
{}


Reader::~Reader()
{}


Format Reader::format() const
{
	return impl_->format();
}


Buffer Reader::read(int sample_frames)
{
	return impl_->read(sample_frames);
}


bool Reader::is_valid() const
{
	return impl_->is_valid();
}


size_t Reader::total_samples() const
{
	return impl_->total_samples();
}

}
