<span style="font-size:18px; font-family: Consolas,serif" >

# Parallel Programming Course Projects

This repository contains the projects and assignments during the **Parallel Programming** course at Tehran University. The projects cover various domains of parallel and concurrent programming, including **SIMD**, **OpenMP**, **POSIX threads**, **Intel Parallel Studio**, and **CUDA**. Each project demonstrates practical implementations of parallel computing concepts.

---

## Concepts and Domains

### **SIMD (Single Instruction, Multiple Data)**
- **Concepts**: SIMD programming leverages hardware-level parallelism to perform the same operation on multiple data points simultaneously. This is particularly useful for tasks like image processing, matrix operations, and numerical computations.
- **Applications**:
  - Image filtering and transformation (e.g., grayscale conversion, edge detection).
  - Matrix multiplication and other linear algebra operations.
  - Performance comparison between serial and parallel implementations.

---

### **OpenMP**
- **Concepts**: OpenMP is an API for shared-memory parallel programming. It simplifies the process of writing multi-threaded applications by providing high-level directives for parallelizing loops and tasks.
- **Applications**:
  - Parallel image processing (e.g., applying filters to images).
  - Parallel matrix operations (e.g., addition, multiplication).
  - Performance analysis and optimization of parallel algorithms.

---

### **POSIX Threads**
- **Concepts**: POSIX threads (pthreads) enable the creation and management of threads in C/C++. This project focused on thread synchronization and concurrency control using mechanisms like mutexes and condition variables.
- **Applications**:
  - **Bakery Simulation**: A multi-threaded application simulating a bakery system with bakers, customers, ovens, and delivery spaces. Threads represent different entities in the bakery, and synchronization ensures proper handling of shared resources (e.g., ovens, delivery queues).
  - Key challenges included avoiding race conditions, deadlocks, and ensuring thread-safe access to shared data.

---

### **Intel Parallel Studio**
- **Concepts**: Intel Parallel Studio provides tools for optimizing and parallelizing code for Intel processors. It includes features for performance profiling, vectorization, and threading.
- **Applications**:
  - Optimizing existing code for better performance using Intel's tools.
  - Analyzing and improving the efficiency of parallel algorithms.
  - Fixing memory management Bugs.

---

### **CUDA**
- **Concepts**: CUDA is a parallel computing platform and programming model for NVIDIA GPUs. It enables massively parallel computations by offloading work to the GPU.
- **Applications**:
  - **Image Processing**: Implemented GPU-accelerated image processing tasks such as grayscale conversion, edge detection, and filtering.
  - **Ray Tracing**: Developed a basic ray tracer using CUDA to simulate light interactions with 3D objects. Extended the ray tracer to include camera support for more advanced rendering.

---

## Tools and Technologies
- **Programming Languages**: C, C++, Python, CUDA.
- **Libraries and Frameworks**: OpenMP, POSIX threads, Intel Parallel Studio, CUDA.
- **Tools**: CMake, Visual Studio Code, Jupyter Notebook.
