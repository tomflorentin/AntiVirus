# Hooked WinAPI calls



## NtOpenProcess

```
__kernel_entry NTSTATUS WINAPI NtOpenProcess(
  OUT PHANDLE             ProcessHandle,
  IN ACCESS_MASK          AccessMask,
  IN POBJECT_ATTRIBUTES   ObjectAttributes,
  IN PCLIENT_ID           ClientId );
```

- Disallow programs to get an handle of any process of Alpha Defender



## NtCreateFile

```
__kernel_entry NTSTATUS WINAPI NtCreateFile(
	OUT PHANDLE           FileHandle,
	IN ACCESS_MASK        DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK  IoStatusBlock,
	IN PLARGE_INTEGER     AllocationSize,
	IN ULONG              FileAttributes,
	IN ULONG              ShareAccess,
	IN ULONG              CreateDisposition,
	IN ULONG              CreateOptions,
	IN PVOID              EaBuffer,
	IN ULONG              EaLength
);
```

- Disallow programs to open Antivirus files

- Ask confirmation if it try to access the following folders :
	- C:\Users\Username\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
	- C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp
  

## NtOpenKey

```
__kernel_entry NTSTATUS WINAPI NtOpenKey(
  OUT PHANDLE             pKeyHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes );
```

- Ask a confirmation if open one of theses registers : 
	- [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunServices]
	- [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunServicesOnce]
	- [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run]
	- [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunOnce]
	- [HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run]
	- [HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunOnce]
	- [HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunServices]

## NtUnmapViewOfSection

```
__kernel_entry NTSTATUS WINAPI NtUnmapViewOfSection(
  IN HANDLE               ProcessHandle,
  IN PVOID                BaseAddress );
```

- Log this action in the RunPE detector


## NtWriteVirtualMemory

```
__kernel_entry NTSTATUS WINAPI NtWriteVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN PVOID                BaseAddress,
  IN PVOID                Buffer,
  IN ULONG                NumberOfBytesToWrite,
  OUT PULONG              NumberOfBytesWritten OPTIONAL );
```

- Log this action in the RunPE detector


## NtGetContextThread

```
__kernel_entry NTSTATUS WINAPI NtGetContextThread(
  IN HANDLE               ThreadHandle,
  OUT PCONTEXT            pContext );
```

- Log this action in the RunPE detector

## NtSetContextThread

```
__kernel_entry NTSTATUS WINAPI NtSetContextThread(
  IN HANDLE               ThreadHandle,
  IN PCONTEXT             Context );
```

- Log this action in the RunPE detector


