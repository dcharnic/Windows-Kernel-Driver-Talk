# Windows-Kernel-Driver-Talk
The files I used in my Windows Kernel Talk

# Building and running the driver
In order to build this driver, you need to install visual studio with spectre mitigated libs, winddk and winsdk. These can all be found on Microsoft's website for Windows kernel debugging. 
https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/getting-started-with-windbg--kernel-mode-

The driver is located in the /kdmapper-master/HelloWorld folder, main.c. 

# Building KdMapper
Open the .sln in visual studio and build it for x64 release. 

# Building PrintLoop
Its a c++ program, you can open the .sln file in Visual Studio and build it or you can just build it directly with cmake, whatever you want. Just get it to run
s
