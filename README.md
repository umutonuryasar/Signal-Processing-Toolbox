# Signal Processing Toolbox

A desktop DSP toolkit built in C++ with Qt — waveform generation, time/frequency domain visualization, digital filtering, and WAV file analysis. Designed around the same signal processing primitives that underpin audio ML, embedded sensing, and feature extraction pipelines.

**Stack:** C++ · Qt · FFTW · QCustomPlot

---

## Overview

This toolbox implements core DSP operations from scratch, providing an interactive environment for signal analysis. The architecture separates signal generation, transformation, and visualization into independent modules — making the codebase straightforward to extend.

**Key modules:**

| Module | File(s) | Description |
| :--- | :--- | :--- |
| Waveform Generator | `wavegenerator.cpp/h` | Parametric synthesis of sine, square, triangle, sawtooth, and white noise |
| FFT Engine | `fft.cpp/h` | Discrete Fourier Transform via FFTW — outputs magnitude spectrum in dB |
| Digital Filters | `filter.cpp/h` | FIR low-pass and high-pass filters with configurable cutoff and sample rate |
| Time Domain Plot | `timedomainplot.cpp/h` | Interactive QCustomPlot widget — zoom, pan, real-time updates |
| Frequency Domain Plot | `freqdomainplot.cpp/h` | FFT magnitude visualization with interactive frequency axis |
| WAV Reader | `wavreader.cpp/h` | PCM WAV import with time/frequency domain analysis and playback |

---

## Design Notes

**FFT via FFTW:** Rather than implementing a naive O(n²) DFT, the toolbox uses FFTW's Cooley-Tukey algorithm for O(n log n) complexity. For audio-length signals this is the practical choice — the same library used in production audio processing tools.

**FIR filter design:** The filtering module implements a windowed-sinc FIR design. FIR filters are unconditionally stable (no feedback path) and have linear phase response — both properties matter for signal analysis where phase distortion would compromise frequency domain interpretation.

**QCustomPlot for visualization:** Qt's built-in chart widgets lack the performance needed for real-time signal rendering. QCustomPlot handles dynamic updates efficiently and supports the zoom/pan interaction model needed for exploratory signal analysis.

**Module separation:** Each DSP component (`fft`, `filter`, `wavegenerator`) is independently testable. This mirrors the modular design pattern used in production DSP pipelines and ML feature extraction systems.

---

## Features

- **Waveform synthesis** — sine, square, triangle, sawtooth, white noise with configurable frequency and amplitude
- **FFT analysis** — magnitude spectrum in dB, interactive frequency domain plot
- **Digital filtering** — low-pass / high-pass FIR with adjustable cutoff and sample rate
- **WAV support** — import, visualize, filter, and play PCM audio files
- **Real-time visualization** — synchronized time and frequency domain plots with zoom/pan

---

## Installation

```bash
git clone https://github.com/umutonuryasar/Signal-Processing-Toolbox.git
```

**Dependencies:** Qt (Qt Creator or Qt libraries) · FFTW · QCustomPlot

Build and run using Qt Creator or `qmake` + `make`.

---

## License

GPL-3.0 — see [LICENSE](LICENSE)
