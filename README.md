# Antivirus Project : Epitech HUB

 ## Project :
Creating an antivirus for Windows, with a viral base, and a runtime protection.
This is a very ambitious project.

## Software architecture

### Windows Service - C#

 This is the main part of the program. it runs as a Windows Service, with LOCAL_MACHINE maximum privileges.
 It monitors all applications running on the computer, and watch suspicious locations. It also run runtime memory scans, and hard drive scans.
 In the future, the service will also include a Peer to Peer module, to connect to virus database.

 ### Hook DLL - C++ / x86 ASM
The Hook DLL is injected into every running application to have a control on it from the inside.
The DLL uses inline hooking to edit the lowest level windows API functions, and hook them.
Using this hook, the DLL will be able to filter system calls, and tell the service for application suspicious comportment.
It is also able to kill the file itself from the inside, in case of severe persistance.

### Interprocess DLL - C++
The interprocess DLL is loaded by both **Hook DLL** and **Service**. It makes them able to communicate throught an instant TCP connection between processes.

### Graphical interface - Electron JS
The role of this app is to make the antivirus user-friendly. It has a great look, and let the user see informations about his protection, and manage some settings.
The JS is communicating with the **Service** throught HTTP web requests.

### Notification Module - C# WPF
This very simple GUI app is started by the service when it needs user approval about a suspicious comportment. It should warn the user about this program.
The user has three options :

 1. Allow suspicious action, and continue
 2. Deny suspicious actions, and continue
 3. Deny all, and delete program

### Driver - C++ / Kernel Programming

The driver is at the lowest level possible in Windows. Not any untrusted application (like a virus) can bypass kernel mode monitoring.
Like the DLL, it role is to check applications system calls, and filter them.

## Developement Team

 ### Tom Florentin
 ![
](https://image.noelshack.com/fichiers/2019/12/2/1552955415-tom-florentin.jpg)
 EPITECH Strasbourg
 2nd year student
 tom.florentin@epitech.eu


### Quentin Thomas
![
](https://image.noelshack.com/fichiers/2019/12/2/1552955415-quentin1-thomas.jpg)
 EPITECH Strasbourg
 2nd year student
quentin1.thomas@epitech.eu