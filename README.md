# PulseRTOS

**PulseRTOS** is a deterministic, research-grade **Real-Time Operating System (RTOS)** designed for **hard real-time embedded systems**.
Built from first principles on **RISC-V (RV32)**, PulseRTOS focuses on **precise timing, optimal scheduling, and predictable memory behavior**.

> *Pulse* — the fundamental unit of time.
> PulseRTOS is driven by deadlines, ticks, and determinism.

---

## Design Philosophy

PulseRTOS is built with a single goal:

> **Absolute predictability under all conditions**

The system avoids unnecessary abstractions and feature bloat in favor of **formal correctness**, **bounded latency**, and **analyzable behavior**.

---

## Key Features

* Hard real-time guarantees
* Fully preemptive kernel
* Optimal and analyzable schedulers
* Deterministic memory management
* Bounded interrupt latency
* Minimal and auditable codebase
* Research and education friendly

---

## Architecture Overview

PulseRTOS uses a **single address space, monolithic RTOS architecture** optimized for deeply embedded systems.

```
+----------------------------+
|        Applications        |
+----------------------------+
|   IPC / Synchronization    |
+----------------------------+
|        Scheduler           |
+----------------------------+
|     Memory Management      |
+----------------------------+
|   Trap & Context Switch    |
+----------------------------+
|      RISC-V Hardware       |
+----------------------------+
```

---

## Supported Platform

* **Architecture:** RISC-V RV32
* **ISA:** RV32I + Zicsr
* **Privilege Mode:** Machine mode (M-mode)
* **Target:** Bare-metal / QEMU `virt`
* **Toolchain:** `riscv32-unknown-elf-gcc`

---

## Scheduler Subsystem

PulseRTOS includes multiple **fully preemptive real-time schedulers**, selectable at build time.

### Implemented Scheduling Policies

* **Fixed Priority Preemptive Scheduler**

  * O(1) task selection
  * Priority bitmap and ready queues
* **Rate Monotonic Scheduling (RMS)**
* **Earliest Deadline First (EDF)**
* **Round-Robin (optional time slicing)**

### Scheduler Capabilities

* Deadline tracking and enforcement
* Deadline miss detection
* Overload awareness
* Bounded scheduling latency
* Tick-based and tickless scheduling modes

All scheduling decisions are **deterministic and analyzable**.

---

## Task Management

* Fully preemptive multitasking
* Lightweight Task Control Blocks (TCB)
* Dedicated kernel stack per task
* Stack overflow detection
* Priority-inheritance aware execution

### Task States

* READY
* RUNNING
* BLOCKED
* SLEEPING
* SUSPENDED

---

## Context Switching & Trap Handling

* Full register save/restore
* Clean trap entry and exit paths
* Nested interrupt safe
* ISR-aware rescheduling
* Minimal and bounded context switch latency

PulseRTOS maintains strict separation between **ISR context** and **task context**.

---

## Memory Management

PulseRTOS provides a **strictly deterministic memory subsystem**.

### Memory Model

* Static memory layout
* Single address space
* No virtual memory
* No paging

### Allocators

* Fixed-size block allocator
* Slab allocator
* Object pools for kernel primitives
* O(1) allocation and free
* Compile-time configurable memory pools

### Safety Guarantees

* Zero fragmentation
* Stack guard regions
* Memory boundary validation
* No unbounded allocation paths

---

## Inter-Process Communication (IPC)

Minimal, efficient, and real-time safe IPC primitives:

* Semaphores
* Mutexes

  * Priority inheritance
* Message queues
* Event flags

All IPC operations are scheduler-integrated and priority-safe.

---

## Time Management

* System tick via RISC-V CLINT
* High-resolution timers
* Sleep and timeout APIs
* Deadline timers
* Tickless idle support

---

## Interrupt Handling

* Deterministic interrupt latency
* ISR-safe kernel APIs
* Deferred interrupt processing
* Clear ISR vs task context boundaries

---

## Configuration Model

* Compile-time configuration only
* No runtime configuration overhead
* Feature selection via headers
* Scheduler selection at build time

---

## What PulseRTOS Is Not

PulseRTOS deliberately excludes:

* Filesystems
* Networking stacks
* Device driver frameworks
* POSIX compatibility layers
* User/kernel isolation

These are intentionally omitted to preserve **hard real-time guarantees**.

---

## Intended Use Cases

* Hard real-time control systems
* Robotics and automation
* Safety-critical embedded systems
* RTOS scheduler research
* RISC-V teaching and experimentation

---

PulseRTOS is stable and suitable for research, education, and experimentation.
Most of the features are still under development and present in dev private branches.

---

## License

MIT License

---

## Author

Designed and implemented by **Hari**
Focused on real-time systems, RISC-V, and operating system internals.

---

## Motto

> **Every pulse matters.**
