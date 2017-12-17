
#include <windows/Bitmap.h>

#include <cassert>
#include <cstring>

namespace windows
{
	namespace
	{
		template <typename ObjectHandleType>
		struct ObjectInfo;

		template <>
		struct ObjectInfo<HBITMAP > { using type = BITMAP;   };
		template <>
		struct ObjectInfo<HPALETTE> { using type = WORD;     };
		template <>
		struct ObjectInfo<HBRUSH  > { using type = LOGBRUSH; };
		template <>
		struct ObjectInfo<HFONT   > { using type = LOGFONT;  };

		template <typename ObjectHandleType>
		using ObjectInfoType = typename ObjectInfo<ObjectHandleType>::type;

		template <typename ObjectHandleType>
		auto GetObjectInfo (ObjectHandleType objectHandle)
		{
			auto objectInfo = ObjectInfoType<ObjectHandleType> { };

			const auto result = GetObject
			(
				objectHandle, sizeof(objectInfo), &objectInfo
			);
			assert(result > 0);

			return objectInfo;
		}
	}

	Bitmap::Bitmap (const util::uvec2& size)
	:
		bitmapHandle        { nullptr },
		deviceContextHandle { nullptr }
	{
		deviceContextHandle = CreateCompatibleDC(nullptr);
		assert(deviceContextHandle != nullptr);

		const auto bitmapInfo = BITMAPINFO
		{
			BITMAPINFOHEADER
			{
				sizeof(BITMAPINFOHEADER),        // size
				+static_cast<LONG>(size.width),  // width
				-static_cast<LONG>(size.height), // height
				1,                               // planes
				32,                              // bit count
				BI_RGB,                          // compression
			}
		};

		void* bitValues = nullptr;

		bitmapHandle = CreateDIBSection
		(
			deviceContextHandle,
			&bitmapInfo,
			DIB_RGB_COLORS,
			&bitValues, // pointer to bitmap's storage
			nullptr, 0  // allocate memory for the DIB
		);
		assert((bitmapHandle != nullptr) && (bitValues != nullptr));

		const auto pixelSize = bitmapInfo.bmiHeader.biBitCount / CHAR_BIT;
		std::memset(bitValues, 0xAA, size.width * size.height * pixelSize);
	}

	Bitmap::~Bitmap ()
	{
		auto result = DeleteObject(bitmapHandle);
		assert(result != 0);

		result = DeleteDC(deviceContextHandle);
		assert(result != 0);
	}

	Bitmap::Bitmap (Bitmap&& bitmap)
	:
		bitmapHandle        { bitmap.bitmapHandle        },
		deviceContextHandle { bitmap.deviceContextHandle }
	{
		bitmap.bitmapHandle        = nullptr;
		bitmap.deviceContextHandle = nullptr;
	}

	Bitmap& Bitmap::operator = (Bitmap&& bitmap)
	{
		bitmapHandle        = bitmap.bitmapHandle;
		deviceContextHandle = bitmap.deviceContextHandle;

		bitmap.bitmapHandle        = nullptr;
		bitmap.deviceContextHandle = nullptr;

		return *this;
	}

	util::uvec2 Bitmap::Size () const
	{
		const auto bitmapInfo = BITMAP { GetObjectInfo(bitmapHandle) };

		return util::uvec2
		{
			static_cast<unsigned int>(bitmapInfo.bmWidth),
			static_cast<unsigned int>(bitmapInfo.bmHeight),
		};
	}
}
