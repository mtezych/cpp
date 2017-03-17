
#ifndef PCI
#define PCI

#include <cstdint>

namespace pci
{
	// https://www.khronos.org/conformance/adopters/conformant-companies#vulkan
	// http://pcisig.com/membership/member-companies
	constexpr const char* vendor(uint32_t pciID)
	{
		switch (pciID)
		{
			case 0x10DE:
				return "NVIDIA Corporation";
			case 0x1022:
				return "Advanced Micro Devices";
			case 0x8086:
				return "Intel Corporation";
			case 0x144D:
				return "Samsung";
			case 0x5143:
				return "Qualcomm";
			case 0x13B5:
				return "ARM";
			case 0x1AEE:
				return "Imagination Technologies";
			default:
				return "Unknown Vendor";
		}
	}
}

#endif
