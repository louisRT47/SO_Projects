# SO_Projects
Three C programming projects focusing on server managementa and UNIX system, developed during college using Eclipse IDE.

**Current Status:** Done ✅

## Project 1

**Goal:** Familiarization with the UNIX/LINUX environment; Design of programs involving process creation, program execution, pipe usage, and I/O redirection; Consolidation of system-level programming.

## Project 2

**Goal:** Familiarization with the UNIX/LINUX environment; Design of programs based on the client/server paradigm using sockets as a mechanism of communication between processes; Design of concurrent programs based on multitasking; Consolidation of system-level programming.

## Methodology
Implement a server process that provides the service of converting PDF files to text format.
The client process connects to the server and sends a PDF file, expecting the server to return the corresponding file in text format.
The files sent for conversion are temporarily stored on disk in a folder associated with the conversion service.
The server process converts the PDF files using the external application pdftotext. The execution of this application will be performed using the functions from the Process module developed during the first practical work.

The conversion server will be a concurrent process based on multiple tasks:

1. The accept task (Accept thread) is responsible for handling connections from client processes, receiving the PDF file, converting the received file, and sending the file in text format to the client.
2. The menu task (Menu thread) is responsible for processing the interface (text menu in the console) with the user. This interface allows the user to observe the server's status by presenting information about the number of processed files, average size of converted files, etc. The purpose of this menu is to assist in validating the solution, allowing additional options to be added as needed.

## Project 3

**Goal:** Familiarization with the UNIX/LINUX environment; Design of programs based on the client/server paradigm using sockets as a communication mechanism between processes; Design of concurrent programs based on multitasking; Synchronization between multiple tasks in POSIX; Use of UNIX signals; Construction of dynamic libraries for the UNIX system; Consolidation of system-level programming.

## Methodology

Consider the following requirements to be added to the prototype developed in the previous work:

1. Accept Thread - Task responsible for handling connections from client processes. This task starts depositing each new connection into a connection queue (Producer/Consumer). These connections will be processed by a set of tasks (Receive Threads) previously created and responsible for receiving files to be converted from client processes;
2. Receive Threads - After completing the reception of a file, these tasks place an indication of the existence of a new file in another file queue for conversion (Producer/Consumer) and wait for the existence of a new connection to receive a new file from a client;
3. Convert Threads - These tasks obtain a file from their respective queue (Producer/Consumer) and proceed to convert it to text format. After conversion is completed, they send the file to the client and wait for a new file in the queue of files to be converted.
4. Menu thread - Task responsible for processing the user interface. The server should present a management interface (text menu in the console) that allows:

Viewing the number of files waiting to be converted, total number of processed files, average size of converted files, etc.;

Shutting down the server: No longer accepting requests and waiting for ongoing conversions to finish.

