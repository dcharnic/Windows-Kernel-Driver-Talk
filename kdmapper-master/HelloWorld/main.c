#include <ntddk.h>

// This defines the no-doc MemCopy function in NTAPI
// SourceP is the process that is calling the function
// SourceA is the address we want to copy from
// TargetA is the address we are going to place our copied memory into
// BufferS is the size of memory to copy
// Mode is the mode we are calling from, we are calling from kernel Here. 
// Size is the size of the bytes that were successfully copied. If this doesn't match BufferSize, something broke.

// MmCopyVirtualMemory can also be used to read memory addresses. See if you can create a new function that reads memory at an address. Use KeWriteProcessMemory as a template. It wont be very different. 
NTSTATUS NTAPI MmCopyVirtualMemory(PEPROCESS sourceP, PVOID sourceA, PEPROCESS targetP, PVOID targetA, SIZE_T BufferS, KPROCESSOR_MODE mode, PSIZE_T Size);

// Just defining another function
// This one places a PEPROCESS* into the the value that is passed in as Process when we give the function a PID.
NTKERNELAPI NTSTATUS PsLookupProcessByProcessId(_In_ HANDLE pid, _Outptr_ PEPROCESS* proc);

// This is a simplified version of our no-doc MmCopyVirtualMemory function. 
// Some things can be inferred / hardcoded so we only ask for what we actually need. 
NTSTATUS KeWriteProcessMemory(PEPROCESS P, PVOID SourceA, PVOID TargetA, SIZE_T Size) {
	SIZE_T ptr;
	PEPROCESS SourceP = PsGetCurrentProcess();
	PEPROCESS TargetP = P;
	NTSTATUS exit = MmCopyVirtualMemory(SourceP, SourceA, TargetP, TargetA, Size, 0, &ptr);
	return exit;
}
// This is the code that gets run when the driver gets loaded, like Main()
NTSTATUS CustomDriverEntry(_In_ PDRIVER_OBJECT  kdmapperParam1, _In_ PUNICODE_STRING kdmapperParam2) {
	//Defines an int3 breakpoint, will be caught by the debugger.
	DbgBreakPoint();

	//Print something
	KdPrint(("Hello world!"));
	int var2 = 5;
	if (var2 == 3) {
		// Prints the actual physical memory address of var2
		KdPrint(("Address of var: %p\n", &var2));
	}
	if (var2 == 4) {
		KdPrint(("0x54175 about to get real"));
		KeBugCheck(0xEF715BAD);					//Tell me what this is talking about after I'm done and I'll give you a sticker
	}
	if (var2 == 5) {
		KdPrint(("I'm about to hack this dude"));
		// Create a new PEPROCESS to store the process we are attacking
		PEPROCESS proc;
		// Value that will get written into the program we attack
		int num = 0xACDC;
		// Place the pid of PrintLoop here
		int pid = 1912;
		// Place the address output by PrintLoop here
		PVOID addr = 0x000000536B58F890;
		PsLookupProcessByProcessId(pid, &proc); //lookup the process by it's id;
		// Write "num" to the process in "Process" at the memory address pointed to by "addr". We are going to be writing a 32 bit int, so our buffer must be that big.
		NTSTATUS Status = KeWriteProcessMemory(proc, &num, addr, sizeof(__int32));
		return Status;
	}
	//Return 0, this code is equivalent to NTSTATUS SUCCESS OR 0X0
	return 0;
}
