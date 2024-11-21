# Local Network Chat Application

## Overview

This project is a **local network chat application** implemented in C++. It enables communication between multiple independent processes via **POSIX-based inter-process communication (IPC)** methods. The application consists of a **host** and multiple **clients**, each running as independent processes. The chat features include:

- **Global Chat**: Messages visible to all connected participants.
- **Private Chat**: Direct communication between the host and individual clients.
- **Message Order**: All messages are displayed in chronological order.
- **Inactivity Handling**: Clients that remain idle for over a minute are terminated using the `SIGKILL` signal (except the host).

## Features

1. **Host**:
   - Can send both global and private messages.
   - Relays messages from clients to the global chat or as private messages to specific clients.
   - Handles each client in a separate thread for efficient message dispatching.
   - GUI implemented with **Qt6**.

2. **Client**:
   - Can send and receive messages in the global chat.
   - Can exchange private messages with the host.
   - GUI implemented with **Qt6**.

3. **IPC Mechanisms**:
   - **Message Queues**: Used for managing message delivery with proper queuing.
   - **Named Pipes (FIFO)**: Supports bi-directional communication between processes.
   - **Shared Files**: Facilitates temporary data exchange.
   - **Signal-Based Communication**: Used for synchronization and notifications.

## Directory Structure

- `connections/`: Contains files implementing the communication mechanisms:
  - **Message Queues**: Created and managed using `msgget`, `msgsnd`, and `msgrcv`.
  - **Named Pipes**: Created using `mknod` or `mkfifo` system calls.
  - **Shared Files**: Provides auxiliary storage for IPC.

- `client/`: Contains the client-side implementation:
  - GUI for interacting with the host and other clients.
  - Client logic for global and private chats.

- `host/`: Contains the host-side implementation:
  - GUI for managing the chat application.
  - Host logic for managing clients and routing messages.
  - `ClientInfo` class: Represents connected clients and handles their communication.

## Implementation Details

### Communication Methods

1. **Message Queues**:
   - Created with `msgget` using a unique key.
   - Messages are sent with `msgsnd` and retrieved with `msgrcv`.

2. **Named Pipes (FIFO)**:
   - Created with `mknod` or `mkfifo`.
   - Enables bi-directional communication without needing multiple channels.
   
3. **Shared Files (`shm_open` + `mmap`)**  
   - This method creates a shared memory object using `shm_open` and maps it with `mmap`. Unlike anonymous mapping, each process opens and maps the shared memory object independently, treating them as separate processes.  
   - Example:
     ```c++
     int fd = shm_open("/shm_name", O_CREAT | O_RDWR, 0666);
     ftruncate(fd, size);
     void* shared_mem = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
     ```
   - This approach is suitable for processes without a parent-child relationship, ensuring modular interaction through the shared memory object.

4. **Signal-Based Notification**:
   - Used for initial synchronization when a client connects and the host creates communication channels.
   - Client synchronously waits for a notification signal after sending its first message.

### Scalability
The application supports an **unlimited number of clients** (theoretically) due to the thread-based design and robust IPC mechanisms.

### Build Instructions
1. Install required dependencies:
   - **Qt6** for GUI components.
   - Standard development tools (`g++`, `make`, etc.).
2. Clone the repository and navigate to the project directory:
   ```bash
   git clone https://github.com/eagerbeaver04/operating_system
   cd operating_system
   ```
3. The project uses **CMake** for building, ensuring a flexible and cross-platform setup. A `build.sh` script is included to simplify the build process. To build the project, execute the following command in the root directory:
```bash
./build.sh
```

### Usage

To run the application, execute the appropriate host and client executable files based on the chosen communication method:

- For **Named Pipes (FIFO)** communication:
  - Start the host:  
    ```bash
    ./host_fifo
    ```
  - Start the client:  
    ```bash
    ./client_fifo
    ```

- For **Message Queues (MQ)** communication:
  - Start the host:  
    ```bash
    ./host_mq
    ```
  - Start the client:  
    ```bash
    ./client_mq
    ```

- For **Shared Memory (SHM)** communication:
  - Start the host:  
    ```bash
    ./host_shm
    ```
  - Start the client:  
    ```bash
    ./client_shm
    ```

These executables will initialize the respective communication mechanism and allow interaction between the host and the client in the chat application.

