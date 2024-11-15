// Include our references.
#include <ntddk.h>

// This defines the no-doc MemCopy function in NTAPI
// SourceProcess is the process that is calling the function
// SourceAddress is the address we want to copy from
// TargetAddress is the address we are going to place our copied memory into
// BufferSize is the size of memory to copy
// PreviousMode is the mode we are calling from, we are calling from kernel Here. 
// ReturnSize is the size of the bytes that were successfully copied. If this doesn't match BufferSize, something broke.

// MmCopyVirtualMemory can also be used to read memory addresses. See if you can create a new function that reads memory at an address. Use KeWriteProcessMemory as a template. It shouldn't be that different. 
NTSTATUS NTAPI MmCopyVirtualMemory(
	PEPROCESS SourceProcess,
	PVOID SourceAddress,
	PEPROCESS TargetProcess,
	PVOID TargetAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T ReturnSize
);

// Just defining another function
// This one places a PEPROCESS* into the the value that is passed in as Process when we give the function a PID.
NTKERNELAPI
NTSTATUS
PsLookupProcessByProcessId(
	_In_ HANDLE ProcessId,
	_Outptr_ PEPROCESS* Process
);

// This is a simplified version of our no-doc MmCopyVirtualMemory function. 
// Some things can be inferred / hardcoded so we only ask for what we actually need. 
NTSTATUS KeWriteProcessMemory(
	PEPROCESS Process,
	PVOID SourceAddress,
	PVOID TargetAddress,
	SIZE_T Size
) {
	PEPROCESS SourceProcess = PsGetCurrentProcess();
	PEPROCESS TargetProcess = Process;
	SIZE_T Result;
	NTSTATUS status = MmCopyVirtualMemory(
		SourceProcess,
		SourceAddress,
		TargetProcess,
		TargetAddress,
		Size,
		KernelMode,
		&Result
	);

	if (NT_SUCCESS(status))
		return STATUS_SUCCESS;  // Operation was successful
	else
		return STATUS_ACCESS_DENIED;
}
// This is the code that gets run when the driver gets loaded, like Main()
NTSTATUS CustomDriverEntry(
	_In_ PDRIVER_OBJECT  kdmapperParam1,
	_In_ PUNICODE_STRING kdmapperParam2
)
{
	//These are parameters passed to entry by kdMapper, we dont need them so we'll mark them as unreferenced
	UNREFERENCED_PARAMETER(kdmapperParam1);
	UNREFERENCED_PARAMETER(kdmapperParam2);
	
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
		KdPrint(("I'm about to hack this dude\n"));
		// Value that will get written into the program we attack
		int Writeval = 0xACDC;
		// Create a new PEPROCESS type to store the pointer to the process we are attacking
		PEPROCESS Process;
		PsLookupProcessByProcessId(13016, &Process); //lookup the process by it's id;
		// Write "Writeval" to the process in "Process" at the memory address pointed to by the 3rd argument. We are going to be writing a 32 bit int, so our buffer must be that big.
		KeWriteProcessMemory(Process, &Writeval, 0x8BBE35F9E0, sizeof(__int32));

		// If everything worked, the program will print out the new value. 
		DbgPrint("Value of int i: %d", Writeval);

		return STATUS_SUCCESS;
	}
	//Return 0, this code is equivalent to NTSTATUS SUCCESS OR 0X0
	return 0;
}
