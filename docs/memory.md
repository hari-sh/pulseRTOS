# pulseRTOS – Memory Subsystem Documentation

This document fully describes the **memory design, rules, layout, and safety mechanisms** used in **pulseRTOS**.
It is intended to be a **single source of truth** for developers, reviewers, and future maintainers.

---

## 1. Memory Philosophy

pulseRTOS follows a **deterministic, RTOS-grade memory model**:

* ❌ No `malloc` / `free`

* ❌ No heap

* ❌ No fragmentation

* ❌ No virtual memory / MMU

* ✅ Fixed-size memory pools

* ✅ O(1) allocation and free

* ✅ Static ownership rules

* ✅ Fail-fast on memory errors

All memory usage is **predictable at build time**.

---

## 2. Runtime Memory Layout

pulseRTOS runs in a **single flat address space** (QEMU `virt`, bare metal).

```
0x80000000  ─────────────────────────────
            │ .text    (kernel code)
            │
            ├────────────────────────────
            │ .rodata  (const strings)
            │
            ├────────────────────────────
            │ .data    (initialized globals)
            │
            ├────────────────────────────
            │ .bss     (zeroed globals)
            │          - task stacks
            │          - memory pools
            │          - TCB pool
            │
            ├────────────────────────────
            │ Kernel memory pools
            │   ├─ pool16
            │   ├─ pool32
            │   ├─ pool64
            │   ├─ pool128
            │   └─ tcb_pool
            │
            ├────────────────────────────
            │ Task stacks (static)
            │   ├─ Task 0 stack
            │   ├─ Task 1 stack
            │   └─ Task N stack
            │
            ├────────────────────────────
            │ Boot / kernel stack
            │
            └────────────────────────────
```

### Key Properties

* Single address space
* No runtime relocation
* No dynamic heap growth
* All memory ownership is static or pool-based

---

## 3. Task Stack Design

Each task owns **one statically allocated stack**.
Stacks are never freed or resized.

### Stack Direction

* Stack grows **downward** (RISC-V ABI)
* Initial SP = `stack_base + stack_size`

---

### Stack Layout

```
Low address
┌──────────────────────────┐
│ STACK_GUARD (0xDEADBEEF) │  ← overflow detection
├──────────────────────────┤
│ 0xA5 pattern             │
│ unused stack region      │
│                          │
├──────────────────────────┤
│ actual stack usage       │  ← grows downward
│                          │
├──────────────────────────┤
│ initial SP               │
└──────────────────────────┘
High address
```

---

### Stack Safety Mechanisms

#### 1. Stack Guard

* Guard word placed at **lowest address** of stack
* Value: `0xDEADBEEF`
* Checked on every context switch

If corrupted → **kernel panic**

---

#### 2. Stack Pattern Fill

* Entire stack filled with `0xA5` at task creation
* Unused stack remains patterned

---

#### 3. High-Water Mark Tracking

* Stack scanned for first non-pattern byte
* Calculates **maximum stack usage ever reached**
* Stored per-task in TCB

Used for:

* stack sizing
* debugging
* safety validation

---

## 4. Kernel Memory Pools

pulseRTOS uses **fixed-size memory pools** for all dynamic kernel objects.

### Available Pools

| Pool     | Block Size  | Typical Usage         |
| -------- | ----------- | --------------------- |
| pool16   | 16 bytes    | tiny kernel objects   |
| pool32   | 32 bytes    | small structs         |
| pool64   | 64 bytes    | scheduler / IPC nodes |
| pool128  | 128 bytes   | larger kernel objects |
| tcb_pool | sizeof(TCB) | task control blocks   |

---

### Pool Characteristics

* Fixed-size blocks
* Singly linked free list
* O(1) allocation and free
* No fragmentation
* No resizing

---

### Memory Pool Statistics

Each pool tracks:

* `total_blocks`
* `free_blocks`
* `min_free_blocks` (peak usage indicator)

These statistics allow:

* memory tuning
* leak detection
* IPC sizing decisions

---

## 5. Allocation Policy (OOM Handling)

### Kernel Allocation Rules

* `kmalloc()` **never returns NULL**
* Out-of-memory is **fatal**
* System halts immediately

Reason:

> RTOS correctness > partial execution

---

### Kernel Panic Behavior

On fatal memory error:

* Message printed via UART
* System enters infinite loop
* No recovery attempt

This ensures:

* no silent corruption
* deterministic failure mode

---

## 6. Object Lifetime Rules

Strict ownership rules prevent memory corruption.

### Task Control Block (TCB)

* Allocated only by `init_task()`
* Freed only by `task_delete()`
* Never freed by scheduler

#### TCB Safety

* Magic value (`TCB_MAGIC`) used
* Detects:

  * double free
  * use-after-free
  * memory overwrite

---

### Task Stacks

* Allocated at boot
* Never freed
* Owned by task for lifetime

---

### Memory Pools

* Initialized once at boot
* Never destroyed
* Accessed only via allocator APIs

---

## 7. Alignment Rules

* All stacks aligned to **16 bytes**
* Pool block size ≥ `sizeof(mem_block_t)`
* Ensures ABI safety and predictable behavior

---

## 8. Linker Script Contract

The linker guarantees:

* `.text` → executable code
* `.data` → writable initialized data
* `.bss` → zero-initialized memory

### RWX Warning

The RWX segment warning is:

* expected for bare-metal
* acceptable without MMU
* documented and intentional

---

## 9. Memory Invariants (Must Never Be Broken)

The following rules define **memory correctness** in pulseRTOS:

* Stack guard must remain intact
* Stack grows downward only
* No allocation outside pools
* No silent allocation failures
* Each object freed exactly once
* Scheduler never manages memory ownership

Violation of any invariant results in **kernel panic**.

---

## 10. Memory Subsystem Status

```
✔ Stack overflow detection
✔ Stack high-water mark
✔ Alignment enforcement
✔ Memory pool statistics
✔ Allocation failure policy
✔ Object lifetime rules
✔ Memory layout documentation
```

🟢 **Memory subsystem is complete and RTOS-grade**

---

This document intentionally freezes the memory design.
All future kernel work (scheduler, IPC, priorities) must obey these rules.
