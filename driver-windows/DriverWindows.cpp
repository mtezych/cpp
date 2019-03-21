
/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2019, mtezych
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

#include <wdm.h>
#include <wdf.h>

//
// Windows Driver Kit Documentation
// ~ https://docs.microsoft.com/en-us/windows-hardware/drivers
//
// Writing a Simple WDF Driver
// ~ https://docs.microsoft.com/en-us/windows-hardware/drivers/wdf/writing-a-simple-kmdf-driver
//
// Write a Very Small KMDF Driver
// ~ https://docs.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/writing-a-very-small-kmdf--driver
//

EVT_WDF_DRIVER_UNLOAD EvtDriverUnload;

VOID EvtDriverUnload ([[maybe_unused]] WDFDRIVER driver)
{
	DbgPrint("Driver Unload \n");
}

extern "C"
{
	DRIVER_INITIALIZE DriverEntry;

	NTSTATUS DriverEntry (PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath)
	{
		DbgPrint("Driver Entry \n");

		WDF_DRIVER_CONFIG driverConfig;
		WDF_DRIVER_CONFIG_INIT(&driverConfig, nullptr);
		driverConfig.EvtDriverUnload = EvtDriverUnload;

		NTSTATUS status = WdfDriverCreate
		(
			driverObject,
			registryPath,
			WDF_NO_OBJECT_ATTRIBUTES,
			&driverConfig,
			WDF_NO_HANDLE
		);

		if (!NT_SUCCESS(status))
		{
			return status;
		}

		return STATUS_SUCCESS;
	}
}
