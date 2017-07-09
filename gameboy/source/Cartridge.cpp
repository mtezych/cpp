
#include <gameboy/Cartridge.h>

#include <cassert>
#include <fstream>

namespace gb
{
	namespace
	{
		auto readBytesFromFile(const std::string& filePath)
		{
			/*
			auto file = std::ifstream
			{
				filePath,
				std::ios::in | std::ios::binary | std::ios::ate
			};
			assert(file);

			const auto fileSize = file.tellg();
			assert(fileSize > 0);

			file.seekg(0, std::ios::beg);

			auto fileBytes = std::vector<uint8_t>(static_cast<size_t>(fileSize), '\0');

			file.read(reinterpret_cast<char*>(fileBytes.data()), fileBytes.size());

			return fileBytes;
			*/

			auto file = std::ifstream
			{
				filePath,
				std::ios::in | std::ios::binary
			};
			assert(file);

			return std::vector<uint8_t>
			{
				std::istreambuf_iterator<char> { file },
				std::istreambuf_iterator<char> { }
			};
		}
	}

	Cartridge::Cartridge(const std::string& romPath)
	:
		rom { readBytesFromFile(romPath) }
	{
	}
}
