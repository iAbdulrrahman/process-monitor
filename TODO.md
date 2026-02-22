# Process Monitor - TODO List

## Project Status: Planning Phase

**Last Updated:** January 28, 2026  
**Priority:** Linux (Required), Windows (Optional)

---

## Phase 1: Project Setup & Environment (Week 1)

### 1.1 Development Environment Setup

- [ ] Install Qt 6.x SDK on development machines
- [ ] Install CMake 3.20+
- [ ] Install GCC 9+ or Clang 10+ (Linux)
- [ ] Set up IDE (Qt Creator or VS Code with C++ extensions)
- [ ] Install Git and configure accounts

### 1.2 Project Initialization

- [x] Create GitHub repository (already done ✓)
- [x] Clone repository to local machines
- [x] Create initial project structure:
  - [x] `src/` directory for source code
  - [ ] `include/` directory for headers
  - [ ] `tests/` directory for unit tests
  - [ ] `docs/` directory for documentation
  - [ ] `resources/` directory for UI resources
- [ ] Create initial CMakeLists.txt file
- [ ] Create README.md with build instructions
- [ ] Set up .gitignore for C++/Qt/CMake

### 1.3 Learning & Research

- [ ] Review Qt documentation (Widgets, Charts, Threading)
- [ ] Study Linux /proc filesystem structure
- [ ] Research /proc/[pid]/stat format
- [ ] Research /proc/[pid]/io format
- [ ] Review process monitoring examples

---

## Phase 2: Backend Development - Linux Core (Weeks 2-4)

### 2.1 Project Structure & Architecture

- [x] Design class hierarchy (ProcessMonitor, Process, SystemInfo)
- [x] Create header files for main classes
- [x] Implement platform abstraction layer interface
- [x] Create Linux implementation files

### 2.2 Process Enumeration (Linux)

- [ ] Implement function to list all PIDs from /proc
  - [ ] Read /proc directory
  - [ ] Filter numeric directories (PIDs)
  - [ ] Return vector of process IDs
- [ ] Create Process class to store process information
- [ ] Implement function to read process name from /proc/[pid]/comm
- [ ] Implement function to read command line from /proc/[pid]/cmdline
- [ ] Test process enumeration with various processes

### 2.3 CPU Monitoring (Linux)

- [ ] Study /proc/stat format (system-wide CPU)
- [ ] Study /proc/[pid]/stat format (per-process CPU)
- [ ] Implement system CPU stats reader
  - [ ] Parse /proc/stat
  - [ ] Calculate total CPU time
  - [ ] Track idle time
- [ ] Implement per-process CPU calculation
  - [ ] Read /proc/[pid]/stat
  - [ ] Extract utime and stime (CPU times)
  - [ ] Calculate CPU percentage over time interval
- [ ] Implement CPU core detection (read /proc/cpuinfo)
- [ ] Test CPU monitoring accuracy

### 2.4 Memory Monitoring (Linux)

- [ ] Implement system memory stats reader
  - [ ] Parse /proc/meminfo
  - [ ] Extract MemTotal, MemAvailable, MemFree
  - [ ] Calculate used memory
- [ ] Implement per-process memory reader
  - [ ] Parse /proc/[pid]/status
  - [ ] Extract VmSize (virtual memory)
  - [ ] Extract VmRSS (resident set size / physical memory)
  - [ ] Extract VmSwap (swap usage)
- [ ] Test memory monitoring accuracy

### 2.5 I/O Monitoring (Linux)

- [ ] Study /proc/[pid]/io format
- [ ] Implement I/O stats reader
  - [ ] Read /proc/[pid]/io
  - [ ] Extract read_bytes and write_bytes
  - [ ] Calculate I/O rates (bytes/second)
- [ ] Implement disk I/O tracking over time
- [ ] Test I/O monitoring with file operations

### 2.6 Additional Process Information

- [ ] Read process state from /proc/[pid]/stat (running, sleeping, etc.)
- [ ] Read process owner (UID) from /proc/[pid]/status
- [ ] Convert UID to username (read /etc/passwd or use getpwuid)
- [ ] Read parent PID (PPID) from /proc/[pid]/stat
- [ ] Read thread count from /proc/[pid]/status
- [ ] Implement uptime calculation from /proc/uptime

### 2.7 Data Collection Manager

- [ ] Create ProcessMonitor manager class
- [ ] Implement periodic data collection (timer-based)
- [ ] Implement thread-safe data structure for process list
- [ ] Create update mechanism (collect stats every N seconds)
- [ ] Implement caching to reduce /proc reads
- [ ] Add error handling for missing/terminated processes

---

## Phase 3: GUI Development (Weeks 5-6)

### 3.1 Qt Project Setup

- [ ] Create Qt Widgets application structure
- [ ] Set up main window class
- [ ] Configure CMake for Qt integration
- [ ] Test basic Qt window creation

### 3.2 Main Window UI

- [ ] Design main window layout (sketches/wireframes)
- [ ] Create main window with menu bar
- [ ] Add File menu (Exit, Preferences)
- [ ] Add View menu (Refresh Rate, Theme)
- [ ] Add Help menu (About)
- [ ] Create central widget with splitter layout

### 3.3 Process Table View

- [ ] Create QTableWidget for process list
- [ ] Add columns: PID, Name, CPU%, Memory, I/O Read, I/O Write, Status
- [ ] Implement column headers
- [ ] Enable column sorting (click header to sort)
- [ ] Implement row selection
- [ ] Add alternating row colors
- [ ] Implement color coding for high resource usage

### 3.4 Search and Filter

- [ ] Add search bar (QLineEdit) above table
- [ ] Implement real-time search/filter by process name
- [ ] Add filter by PID option
- [ ] Implement "Show only user processes" checkbox
- [ ] Test search performance with many processes

### 3.5 Process Details Panel

- [ ] Create details panel widget (QWidget)
- [ ] Display selected process information:
  - [ ] PID, Name, User
  - [ ] Command line
  - [ ] Parent process (PPID)
  - [ ] State, Thread count
  - [ ] Memory details (Virtual, Resident, Swap)
  - [ ] CPU time (User, System)
  - [ ] I/O statistics
- [ ] Update details when selection changes

### 3.6 System Overview Panel

- [ ] Create system stats widget
- [ ] Display CPU core count
- [ ] Display total/used/available RAM
- [ ] Display system uptime
- [ ] Add visual progress bars for resource usage

### 3.7 Real-Time Charts (Qt Charts)

- [ ] Add Qt Charts module to CMake
- [ ] Create CPU usage chart (QLineSeries)
- [ ] Create memory usage chart (QAreaSeries)
- [ ] Implement time-based X-axis (last 60 seconds)
- [ ] Implement percentage/MB Y-axis
- [ ] Update charts in real-time
- [ ] Add chart legends

### 3.8 Context Menu & Actions

- [ ] Implement right-click context menu on table
- [ ] Add "Kill Process" action
- [ ] Add "View Details" action
- [ ] Add "Copy PID" action
- [ ] Implement process termination (kill system call)
- [ ] Add confirmation dialog for kill action

### 3.9 Threading & Performance

- [ ] Move data collection to background thread (QThread)
- [ ] Implement signal/slot for data updates
- [ ] Ensure UI remains responsive during updates
- [ ] Optimize table refresh (only update changed values)
- [ ] Test with high refresh rates

### 3.10 Settings & Preferences

- [ ] Create settings dialog
- [ ] Add refresh rate selector (1s, 2s, 5s)
- [ ] Add theme selector (Light, Dark)
- [ ] Implement QSettings for persistent preferences
- [ ] Apply settings on startup

---

## Phase 4: Integration & Features (Week 7)

### 4.1 Backend-Frontend Integration

- [ ] Connect ProcessMonitor to GUI
- [ ] Implement data update signals
- [ ] Populate table with process data
- [ ] Update charts with system stats
- [ ] Test data flow from backend to UI

### 4.2 Process Tree View (Optional Feature)

- [ ] Create tree view widget (QTreeWidget)
- [ ] Build parent-child process relationships
- [ ] Display processes in hierarchical structure
- [ ] Add toggle between list and tree view

### 4.3 Additional Features

- [ ] Implement process priority display
- [ ] Add "Refresh Now" button
- [ ] Add export data to CSV functionality
- [ ] Implement keyboard shortcuts (F5 = refresh, Ctrl+F = search)
- [ ] Add icons for different process states

### 4.4 Error Handling & Robustness

- [ ] Handle permission errors (some /proc files need root)
- [ ] Handle processes that terminate during read
- [ ] Add try-catch blocks around file operations
- [ ] Display user-friendly error messages
- [ ] Log errors to file for debugging

### 4.5 Performance Optimization

- [ ] Profile application CPU usage
- [ ] Optimize /proc file parsing (minimize reads)
- [ ] Implement intelligent refresh (only changed processes)
- [ ] Reduce memory allocations
- [ ] Test with 500+ processes

---

## Phase 5: Testing & Documentation (Week 8)

### 5.1 Testing

- [ ] Test on Ubuntu 20.04+
- [ ] Test on Fedora
- [ ] Test on Arch Linux
- [ ] Test with normal user privileges
- [ ] Test with root privileges
- [ ] Test CPU monitoring accuracy (compare with `top`)
- [ ] Test memory monitoring accuracy (compare with `free`)
- [ ] Test I/O monitoring accuracy
- [ ] Test process termination functionality
- [ ] Stress test with high refresh rates
- [ ] Test UI responsiveness with 1000+ processes
- [ ] Memory leak testing (run for extended period)

### 5.2 Bug Fixes

- [ ] Fix identified bugs from testing
- [ ] Handle edge cases
- [ ] Improve error messages
- [ ] Fix UI glitches

### 5.3 Code Quality

- [ ] Add code comments and documentation
- [ ] Ensure consistent coding style
- [ ] Remove debug/test code
- [ ] Run static analysis (cppcheck)
- [ ] Check for memory leaks (valgrind)

### 5.4 Documentation

- [ ] Write comprehensive README.md
  - [ ] Project description
  - [ ] Features list
  - [ ] Screenshots
  - [ ] Requirements
  - [ ] Build instructions for Linux
  - [ ] Usage instructions
- [ ] Create BUILDING.md with detailed compilation steps
- [ ] Write developer documentation (ARCHITECTURE.md)
  - [ ] Class diagrams
  - [ ] Code structure explanation
  - [ ] How to extend the project
- [ ] Add inline code comments (Doxygen style)
- [ ] Create user manual (USER_GUIDE.md)
- [ ] Document known limitations

### 5.5 Build & Packaging

- [ ] Ensure clean build from scratch
- [ ] Create install target in CMake
- [ ] Create desktop entry file (.desktop)
- [ ] Test installation on clean system
- [ ] Create AppImage (optional)
- [ ] Create DEB package (optional)
- [ ] Create RPM package (optional)

---

## Optional: Windows Support (Stretch Goal)

### Windows Implementation (If Time Permits)

- [ ] Set up Windows development environment (MSVC/MinGW)
- [ ] Implement Windows process enumeration (CreateToolhelp32Snapshot)
- [ ] Implement Windows CPU monitoring (PDH library)
- [ ] Implement Windows memory monitoring (PSAPI)
- [ ] Implement Windows I/O monitoring (WMI or GetProcessIoCounters)
- [ ] Add preprocessor directives for Windows/Linux code
- [ ] Test on Windows 10/11
- [ ] Update documentation for Windows build

---

## Continuous Tasks (Throughout Project)

- [ ] Regular Git commits with meaningful messages
- [ ] Code reviews between team members
- [ ] Weekly progress meetings
- [ ] Update TODO list with completed tasks
- [ ] Keep PLAN.md synchronized with changes
- [ ] Backup code regularly
- [ ] Test incrementally after each feature

---

## Milestones

- **Milestone 1** (End of Week 2): Process enumeration working, basic CPU/memory stats
- **Milestone 2** (End of Week 4): Complete backend with I/O stats, data collection manager
- **Milestone 3** (End of Week 6): Working GUI with process table and charts
- **Milestone 4** (End of Week 7): Full integration, all features working
- **Milestone 5** (End of Week 8): Testing complete, documented, ready for submission

---

## Notes

- Focus on Linux implementation first (required)
- Windows support is optional and lower priority
- Test frequently to catch issues early
- Ask for help when stuck (use documentation, forums, StackOverflow)
- Keep code modular and well-organized
- Document as you go, don't leave it for the end

---

## Quick Reference Commands

### Build Project

```bash
mkdir build && cd build
cmake ..
make
```

### Run Application

```bash
./process-monitor
```

### Test Process Info

```bash
cat /proc/[pid]/stat
cat /proc/[pid]/status
cat /proc/[pid]/io
```

### Check Running Processes

```bash
ls /proc | grep -E '^[0-9]+$'
```
