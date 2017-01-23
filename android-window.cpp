
#ifdef __ANDROID__

#include <cassert>

#include <EGL/egl.h>

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

// https://github.com/waynewolf/waynewolf.github.io
// https://source.android.com/devices/graphics/arch-sf-hwc.html

class Window
{
	android::sp<android::SurfaceComposerClient> surfaceComposerClient;
	android::sp<android::SurfaceControl>        surfaceControl;
	android::sp<ANativeWindow>                  androidNativeWindow;

public:
	Window()
	:
		surfaceComposerClient { nullptr },
		surfaceControl        { nullptr },
		androidNativeWindow   { nullptr }
	{
		auto status = android::status_t { android::UNKNOWN_ERROR };

		surfaceComposerClient = new android::SurfaceComposerClient { };
		assert(surfaceComposerClient != nullptr);

		status = surfaceComposerClient->initCheck();
		assert(status == android::NO_ERROR);

		surfaceControl = surfaceComposerClient->createSurface
		(
			android::String8 { "Android Surface Title" },
			512, 512,
			android::PIXEL_FORMAT_RGB_888
		);
		assert(surfaceControl != nullptr);

		const auto validity = surfaceControl->isValid();
		assert(validity);

		android::SurfaceComposerClient::openGlobalTransaction();
		{
			// setting layer establishes Z order of surfaces
			// larger values bring surfaces to front
			status = surfaceControl->setLayer(0x7FFFFFFF);
			assert(status == android::NO_ERROR);

			status = surfaceControl->show();
			assert(status == android::NO_ERROR);
		}
		android::SurfaceComposerClient::closeGlobalTransaction();

		androidNativeWindow = surfaceControl->getSurface();
	}

	EGLNativeWindowType get()
	{
		return androidNativeWindow.get();
	}
};

int main()
{
	auto window = Window { };

	const auto eglNativeWindow = window.get();
	assert(eglNativeWindow != nullptr);

	return 0;
}

#else

int main()
{
	return 0;
}

#endif