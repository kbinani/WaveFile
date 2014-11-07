#pragma once

#include "./format.hpp"
#include "./buffer.hpp"
#include <memory>

namespace WaveFile
{

class Writer
{
public:
	Writer(std::string const& path, Format format);

	~Writer();

	void write(Buffer const& buffer);

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

}
