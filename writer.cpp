#include "./writer.hpp"
#include <string>
#include <cassert>

namespace WaveFile
{

class Writer::Impl
{
public:
	Impl(std::string const& path, Format format)
		: path_(path)
		, format_(format)
		, file_(nullptr)
		, data_chunk_offset_(0)
		, riff_chunk_offset_(0)
		, total_samples_(0)
	{
		file_ = fopen(path.c_str(), "wb");
		if (!write_header()) {
			close();
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
	}


	void write(Buffer const& buffer)
	{
		write(buffer, 0, buffer.size());
	}

	void write(Buffer const& buffer, int offset, size_t num_samples)
	{
		assert(offset >= 0);
		assert(offset + num_samples <= buffer.size());

		std::vector<std::vector<std::int16_t>> samples;
		for (int ch = 0; ch < format_.channels(); ++ch) {
			if (buffer.format().channels() <= ch) {
				break;
			}
			auto s = buffer.samples<std::int16_t>(ch);
			if (offset > 0) {
				s.erase(s.begin(), s.begin() + offset);
			}
			if (num_samples < s.size()) {
				s.erase(s.begin() + num_samples, s.end());
			}
			samples.push_back(s);
		}
		size_t size = buffer.size();
		for (int i = 0; i < size; ++i) {
			for (int ch = 0; ch < format_.channels(); ++ch) {
				int actual_channel = ch < buffer.format().channels() ? ch : 0;
				std::int16_t v = samples[actual_channel][i];
				if (fwrite(&v, sizeof(std::int16_t), 1, file_) != 1) {
					close();
					return;
				}
			}
		}
		total_samples_ += size;
		if (!update_header()) {
			close();
		}
	}

	bool write_header()
	{
		if (!file_) {
			return false;
		}
		// RIFF header
		char riff[] = "RIFF";
		if (fwrite(riff, sizeof(char), 4, file_) != 4) {
			return false;
		}

		// file size
		uint32_t file_size;
		if (fwrite(&file_size, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}
		riff_chunk_offset_ = ftell(file_);

		// "WAVE" header
		char wave[] = "WAVE";
		if (fwrite(wave, sizeof(char), 4, file_) != 4) {
			return false;
		}

		// fmt header
		char fmt[] = "fmt ";
		if (fwrite(fmt, sizeof(char), 4, file_) != 4) {
			return false;
		}

		// "fmt" chunk size
		uint32_t const fmt_chunk_size = 16;
		if (fwrite(&fmt_chunk_size, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}

		// format ID
		uint16_t const format_id = 1;
		if (fwrite(&format_id, sizeof(uint16_t), 1, file_) != 1) {
			return false;
		}

		uint16_t const channels = format_.channels();
		if (fwrite(&channels, sizeof(uint16_t), 1, file_) != 1) {
			return false;
		}

		// sample rate
		uint32_t const sample_rate = format_.sample_rate();
		if (fwrite(&sample_rate, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}

		uint32_t data_rate = format_.data_rate();
		if (fwrite(&data_rate, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}

		uint16_t block_size = format_.block_size();
		if (fwrite(&block_size, sizeof(uint16_t), 1, file_) != 1) {
			return false;
		}

		uint16_t bit_per_sample = format_.bit_per_sample();
		if (fwrite(&bit_per_sample, sizeof(uint16_t), 1, file_) != 1) {
			return false;
		}

		// "data" header
		char data[] = "data";
		if (fwrite(data, sizeof(char), 4, file_) != 4) {
			return false;
		}

		uint32_t size_of_data_chunk = 0;
		if (fwrite(&size_of_data_chunk, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}
		data_chunk_offset_ = ftell(file_);

		return true;
	}

	bool update_header()
	{
		if (!file_) {
			return false;
		}
		long last = ftell(file_);
		if (fseek(file_, riff_chunk_offset_ - sizeof(uint32_t), SEEK_SET) != 0) {
			return false;
		}

		uint32_t data_chunk_size = static_cast<uint32_t>(total_samples_ * format_.block_size());
		uint32_t riff_chunk_size = static_cast<uint32_t>((data_chunk_offset_ - 8) + data_chunk_size);
		if (fwrite(&riff_chunk_size, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}
		if (fseek(file_, data_chunk_offset_ - sizeof(uint32_t), SEEK_SET) != 0) {
			return false;
		}
		if (fwrite(&data_chunk_size, sizeof(uint32_t), 1, file_) != 1) {
			return false;
		}
		if (fseek(file_, last, SEEK_SET) != 0) {
			return false;
		}
		return true;
	}

private:
	std::string const path_;
	Format const format_;
	FILE* file_;
	size_t data_chunk_offset_;
	size_t riff_chunk_offset_;
	size_t total_samples_;
};

Writer::Writer(std::string const& path, Format format)
	: impl_(std::make_unique<Impl>(path, format))
{}


Writer::~Writer()
{}


void Writer::write(Buffer const& buffer)
{
	impl_->write(buffer);
}


void Writer::write(Buffer const& buffer, int offset, int samples)
{
	impl_->write(buffer, offset, samples);
}

}
