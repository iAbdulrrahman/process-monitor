# Process Monitor with Real-Time CPU, Memory, and I/O Statistics

## Project Plan

---

## 1. Project Overview

A cross-platform system monitoring application that displays real-time statistics about running processes, including CPU usage, memory consumption, and I/O operations. The application will feature a modern graphical user interface and support both Windows and Linux operating systems.

---

## 2. Platform Chosen

- **Primary Target**: Windows 10/11 and Linux (Ubuntu 20.04+, Fedora, Arch)
- **Cross-Platform Development**: Using platform-agnostic code with OS-specific modules
- **Architecture Support**: x86-64 (64-bit systems)

---

## 3. Framework / Tools / SDKs

### Programming Language

- **C++17/C++20** - Core application logic and system calls

### GUI Framework

- **Qt 6.x** (Cross-platform GUI framework)
  - Qt Widgets for UI components
  - Qt Charts for real-time graphs
  - Qt Core for threading and signal/slot mechanism

### Build System

- **CMake 3.20+** - Cross-platform build configuration
- **Compiler**:
  - Windows: MSVC 2019+ or MinGW-w64
  - Linux: GCC 9+ or Clang 10+

### System APIs

- **Windows**:
  - Windows API (Process Status API - PSAPI)
  - Performance Data Helper (PDH) library
  - Windows Management Instrumentation (WMI)
- **Linux**:
  - /proc filesystem parsing
  - sysinfo() system call
  - libprocps or direct /proc parsing

### Development Tools

- **IDE**: Visual Studio Code / Qt Creator / Visual Studio
- **Version Control**: Git & GitHub
- **Documentation**: Doxygen for code documentation

### Additional Libraries

- **Threading**: C++ std::thread or Qt QThread
- **JSON parsing**: nlohmann/json (for configuration files)

---

## 4. Problem Statement

Modern operating systems run numerous processes simultaneously, consuming system resources such as CPU, memory, and performing I/O operations. System administrators, developers, and power users need an efficient way to monitor these resources in real-time to:

- **Identify resource-intensive processes** that may be causing system slowdowns
- **Detect memory leaks** or abnormal memory consumption patterns
- **Monitor I/O operations** to identify disk bottlenecks
- **Track CPU usage** across multiple processes and cores
- **Make informed decisions** about process management (terminating, prioritizing processes)

Existing solutions are often platform-specific (Task Manager on Windows, top/htop on Linux) or lack comprehensive features. There is a need for a **cross-platform, user-friendly process monitoring tool** that provides detailed real-time statistics with an intuitive graphical interface.

---

## 5. Project Objectives

### Primary Objectives

1. **Real-Time Process Monitoring**
   - Display all running processes with PID, name, and user
   - Auto-refresh at configurable intervals (e.g., 1-5 seconds)

2. **CPU Statistics**
   - Per-process CPU usage percentage
   - Overall system CPU utilization
   - Multi-core CPU visualization
   - Historical CPU usage graphs

3. **Memory Statistics**
   - Process memory usage (RAM)
   - Virtual memory/working set size
   - System-wide memory consumption
   - Memory usage trends over time

4. **I/O Statistics**
   - Disk read/write operations per process
   - Network I/O monitoring (bytes sent/received)
   - I/O rate visualization

5. **Cross-Platform Support**
   - Single codebase supporting both Windows and Linux
   - Platform-specific optimizations where necessary

6. **User Interface Features**
   - Sortable columns (by CPU, memory, name, PID)
   - Search/filter functionality
   - Process tree/hierarchy view
   - Real-time graphs and charts
   - Dark/Light theme support

### Secondary Objectives

7. **Process Management**
   - Kill/terminate processes from the GUI
   - Change process priority (nice value on Linux)
   - View detailed process information

8. **Performance Optimization**
   - Efficient system resource querying
   - Minimal overhead on system performance
   - Multi-threaded data collection

9. **User Experience**
   - Intuitive and responsive interface
   - Customizable refresh rates
   - Save/load user preferences

---

## 6. Expected Output

### Application Features

1. **Main Dashboard Window**
   - List view displaying all running processes
   - Columns: PID, Process Name, CPU %, Memory (MB), Disk I/O, Network I/O, Status
   - Color-coded indicators for high resource usage

2. **Real-Time Graphs**
   - System-wide CPU usage graph (line chart)
   - Memory usage over time (area chart)
   - Per-process resource consumption charts
   - Update frequency: 1-2 seconds

3. **Process Details Panel**
   - Detailed information when a process is selected
   - Command line arguments
   - Parent-child process relationships
   - Thread count
   - File handles and network connections

4. **System Overview**
   - Total CPU cores and usage
   - Total/available/used RAM
   - Disk I/O statistics
   - Network interface statistics
   - System uptime

5. **Interactive Features**
   - Right-click context menu for process operations
   - Search bar to filter processes by name or PID
   - Sorting by any column (ascending/descending)
   - Process tree view toggle

### Technical Deliverables

1. **Source Code**
   - Well-structured C++ codebase with clear separation of concerns
   - Platform abstraction layer for OS-specific code
   - Comprehensive inline comments and documentation

2. **Build System**
   - CMakeLists.txt for cross-platform compilation
   - Clear build instructions for Windows and Linux

3. **Documentation**
   - README.md with installation and usage instructions
   - Developer documentation (architecture, API reference)
   - User manual with screenshots

4. **Executable Application**
   - Standalone executable for Windows (.exe)
   - Binary package for Linux (or AppImage)
   - Installer packages (optional: MSI for Windows, DEB/RPM for Linux)

### Performance Metrics

- Application startup time: < 2 seconds
- Memory footprint: < 50 MB
- CPU overhead: < 2% on idle
- Refresh latency: < 100ms for data updates

---

## 7. Project Timeline

### Phase 1: Planning & Setup (Week 1)

- Finalize project requirements
- Set up development environment
- Initialize Git repository
- Create project structure

### Phase 2: Core Development (Weeks 2-4)

- Implement platform abstraction layer
- Develop process enumeration for Windows
- Develop process enumeration for Linux
- Implement CPU monitoring module
- Implement memory monitoring module

### Phase 3: I/O & GUI (Weeks 5-6)

- Implement I/O statistics collection
- Design and implement Qt GUI
- Integrate backend with frontend
- Implement real-time charts

### Phase 4: Features & Polish (Week 7)

- Add process management features
- Implement search and filter
- Add configuration and preferences
- Optimize performance

### Phase 5: Testing & Documentation (Week 8)

- Cross-platform testing
- Bug fixes and refinements
- Write documentation
- Prepare final deliverables

---

## 8. Team Responsibilities

### Developer 1: Backend & Windows Implementation

- Windows system APIs integration
- Process enumeration and monitoring
- CPU/Memory statistics collection
- Performance optimization

### Developer 2: GUI & Linux Implementation

- Qt GUI design and implementation
- Linux /proc parsing and monitoring
- Charts and visualization
- User interaction features

### Shared Responsibilities

- Code review and testing
- Documentation
- Cross-platform integration
- Bug fixing

---

## 9. Risks & Mitigation

### Technical Risks

- **Risk**: Platform-specific API differences
  - _Mitigation_: Design robust abstraction layer early

- **Risk**: Performance overhead from monitoring
  - _Mitigation_: Use efficient algorithms, async data collection

- **Risk**: GUI responsiveness with real-time updates
  - _Mitigation_: Multi-threading, Qt signals/slots

### Project Risks

- **Risk**: Time constraints
  - _Mitigation_: Prioritize core features, modular development

- **Risk**: Learning curve for Qt/system APIs
  - _Mitigation_: Early prototyping, use official documentation

---

## 10. Success Criteria

The project will be considered successful if:

1. Application runs on both Windows and Linux without crashes
2. Accurately displays real-time CPU, memory, and I/O statistics
3. GUI is responsive and updates smoothly
4. Users can interact with processes (view details, terminate)
5. Code is well-documented and maintainable
6. Application has minimal performance impact on the system

---

## References

- Windows PSAPI: https://docs.microsoft.com/windows/win32/psapi
- Linux /proc filesystem: https://man7.org/linux/man-pages/man5/proc.5.html
- Qt Documentation: https://doc.qt.io/
- CMake Documentation: https://cmake.org/documentation/
