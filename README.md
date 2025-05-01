# 📼 μ-Law Audio Compression and Decompression

This project implements **μ-law (mu-law) audio compression and decompression** for WAV files using **C programming**. Designed for embedded systems and optimized for **ARM architecture**, the solution includes efficient software handling of WAV data and explores potential hardware acceleration techniques.

---

## 🎯 Project Goals

- Implement μ-law logarithmic PCM encoding for 16-bit WAV audio.
- Optimize for execution under 0.13s for both compression and decompression.
- Apply both **software-based** and **hardware-oriented** optimizations.
- Evaluate performance on **ARM-based VM** and standard Linux.

---

## 🧠 Theoretical Background

- **μ-law Compression** reduces dynamic range to fit 16-bit PCM into 8-bit samples.
- Exploits **human auditory perception** to retain intelligibility despite lossy compression.
- Binary codewords include:  
  - 1-bit sign  
  - 3-bit segment (chord)  
  - 4-bit quantization level

---

## 🧰 Software Implementation

- **Language**: C  
- **I/O**: WAV file reading, header parsing, and block-based processing  
- **Key Functions**:
  - `codeword_compression()` — converts 16-bit PCM to 8-bit μ-law
  - `codeword_decompression()` — restores 16-bit PCM from μ-law
  - `encode_wave_file()` — orchestrates compression/decompression flow

### 🗂 File Structure

- `read_wav_file.h` – Defines WAV header structure
- `encode_wave_file()` – Main controller
- `compress_wave_data()` / `decompress_wave_data()` – I/O buffers and μ-law logic

---

## 🚀 Optimization Techniques

### ✔ General
- **Loop unrolling** for processing multiple samples per iteration
- **Reduced memory copies** with in-place buffer management
- **Streamlined I/O** using bulk reads and writes

### ✔ Compiler Optimizations
- `-O3` flag used with GCC:
  - Inlining frequently used small functions
  - Auto-vectorization of loops
  - Specialized function calls for specific patterns

### ✔ Performance Gains
| Operation         | Unoptimized | Optimized (-O3) |
|------------------|-------------|-----------------|
| Compression Time | 137.1 ms    | 113.4 ms        |
| Decompression    | 93.2 ms     | 77 ms           |

---

## 🖥 Hardware Considerations

While not implemented, the report outlines how a **hardware solution** (e.g., on FPGA or ARM SoC) could:

- Use **parallel pipelines** for faster throughput
- Minimize latency with **deterministic processing**
- Free up CPU via **dedicated μ-law modules**

---

## 🧪 Testing and Results

- **Input**: 10s WAV voice recording (~928 KB)
- **After Compression**: 5s audio, ~464 KB
- **After Decompression**: 10s restored, minor intelligible noise
- Trade-off: Noticeable quality drop, but retained intelligibility

---

## 💡 Future Improvements

- Explore **custom assembly-level scheduling** for ARM
- Implement **hardware acceleration** for real-time audio streaming
- Add **batch file processing** and real-time command-line interface

---

## 📚 References

- [Texas Instruments – μ-Law & A-Law](https://www.ti.com/lit/an/spra163a/spra163a.pdf)  
- [Infineon AN2095 – μ-Law Companding](https://www.infineon.com/dgdl/Infineon-AN2095_Algorithm_Logarithmic_Signal_Companding_Not_Just_a_Good_Idea_It_Is_-Law-ApplicationNotes-v05_00-EN.pdf)  
- Haykin, *Signals and Systems*  
- UVic SENG 440 Lecture Notes

---
