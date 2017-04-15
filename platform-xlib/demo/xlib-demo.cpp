
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, mtezych
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef __gnu_linux__

#include <xlib/Display.h>
#include <xlib/Pixmap.h>
#include <xlib/Window.h>
#include <xlib/Image.h>

#include <chrono>
#include <thread>
#include <cassert>

int main()
{
	const auto status = Status { XInitThreads() };
	assert(status != False);

	const auto display = xlib::Display { };

	const auto data = std::vector<uint8_t> // BGRA
	{
		0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
		0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
		0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00,
	};
	auto image = xlib::Image { display, { 3, 3 }, data };

	auto pixmap = xlib::Pixmap { display, { 512, 512 } };
	auto savedPixmapImage = xlib::Image { pixmap };
	pixmap.Draw(image);

	auto window = xlib::Window { display, { 512, 512 } };
	const auto render = [&window, &image]()
	{
		window.Clear(util::vec4 { 0.0f, 0.5f, 0.0f, 0.0f });
		window.Draw(image);

		const auto savedWindowImage = xlib::Image { window };

		std::this_thread::sleep_for(std::chrono::milliseconds { 16 });
	};
	window.RecieveMessages(render);

	pixmap.Draw(window);

	return 0;
}

#else

int main()
{
	return 0;
}

#endif
