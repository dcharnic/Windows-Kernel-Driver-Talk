
#include <ntddk.h>
//#include <ntdef.h>
//s#include <ntifs.h>

NTSTATUS NTAPI MmCopyVirtualMemory(
	PEPROCESS SourceProcess,
	PVOID SourceAddress,
	PEPROCESS TargetProcess,
	PVOID TargetAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T ReturnSize
);
NTKERNELAPI
NTSTATUS
PsLookupProcessByProcessId(
	_In_ HANDLE ProcessId,
	_Outptr_ PEPROCESS* Process
);
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
		KdPrint(("Address of var: %p\n", &var2));
	}
	if (var2 == 4) {
		KdPrint(("0x54175 about to get real"));
		KeBugCheck(0xEF715BAD);					//Tell me what this is talking about after I'm done and I'll give you a sticker
	}
	if (var2 == 5) {
		KdPrint(("I'm about to hack this dude\n"));
		int Writeval = 666;

		PEPROCESS Process;
		PsLookupProcessByProcessId(13016, &Process); //lookup the process by it's id;

		KeWriteProcessMemory(Process, &Writeval, 0x8BBE35F9E0, sizeof(__int32));

		DbgPrint("Value of int i: %d", Writeval);

		return STATUS_SUCCESS;
	}
	//Return 0, this code is equivalent to NTSTATUS SUCCESS OR 0X0
	return 0;
}
