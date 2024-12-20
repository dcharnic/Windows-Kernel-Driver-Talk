# Windows-Kernel-Driver-Talk
The files I used in my Windows Kernel Talk

# Building and running the driver
In order to build this driver, you need to install visual studio with spectre mitigated libs, winddk and winsdk. These can all be found on Microsoft's website for Windows kernel debugging. 
https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/getting-started-with-windbg--kernel-mode-

The driver is located in the /kdmapper-master/HelloWorld folder, main.c. 

# Building KdMapper
Open the .sln in visual studio and build it for x64 release. 

# Building PrintLoop
Its a c++ program, you can open the .sln file in Visual Studio and build it or you can just build it directly with cmake, whatever you want as long as it runs.

# Using the demo
In my talk I walked through using the driver to do various things and watching it from WinDbg. If you'd like to do the same feel free to examine the code and watch as the driver runs with WinDbg. 

The kernel driver is not polished and is not designed to be run on anything other than a virtual machine, so don't run it on your host.

I did not feel that IOCTL codes and usermode control of the driver was suitable for this talk, so I left it out. If you want to add it for your own satisfaction or project, go for it!

To control the driver simply change the control variable and recompile it. I know that it's not ideal, but IOCTL codes are miles above what I could explain in 1 hour so this worked for now. 
