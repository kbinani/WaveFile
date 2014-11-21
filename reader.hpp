#pragma once

#include "./buffer.hpp"
#include <memory>

namespace WaveFile
{

class Reader
{
public:
	explicit Reader(std::string const& path);

	~Reader();

	Format format() const;

	Buffer read(int sample_count);

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

}
