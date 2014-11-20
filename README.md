# WaveFile

Lightweight *.wav file IO library in C++.

# Requirements

Modern C++ compiler.

# Example

```
#include "wavefile/writer.hpp"

int main()
{
    // default format, 2ch 44100Hz 16bps
    WaveFile::Format format;

    WaveFile::Writer writer("out.wav", format);
    std::vector<float> wav(1024);

    // fill wav data into `wav`.

    WaveFile::Buffer buffer(std::move(wav), format.sample_rate());
    writer.write(buffer);
}
```

# License

MIT License