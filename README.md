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
| Digital Filters | `filter.cpp/h` | Single-pole IIR low-pass and high-pass filters with configurable cutoff and sample rate |
| Time Domain Plot | `timedomainplot.cpp/h` | Interactive QCustomPlot widget — zoom, pan, real-time updates |
| Frequency Domain Plot | `freqdomainplot.cpp/h` | FFT magnitude visualization with interactive frequency axis |
| WAV Reader | `wavreader.cpp/h` | PCM WAV import with time/frequency domain analysis and playback |

---

## Design Notes

**FFT via FFTW:** Rather than implementing a naive O(n²) DFT, the toolbox uses FFTW's Cooley-Tukey algorithm for O(n log n) complexity. For audio-length signals this is the practical choice — the same library used in production audio processing tools.

**IIR filter design:** The filtering module implements single-pole RC low-pass and high-pass filters. A one-pole IIR needs a single multiply-add per sample and one state variable, which keeps it cheap enough to run inside the real-time update loop. The filter object carries its state between blocks, so a stream that arrives in chunks is filtered continuously instead of restarting at every block boundary. The trade-off against a windowed-sinc FIR is a gentle 6 dB/octave roll-off and a non-linear phase response.

**QCustomPlot for visualization:** Qt's built-in chart widgets lack the performance needed for real-time signal rendering. QCustomPlot handles dynamic updates efficiently and supports the zoom/pan interaction model needed for exploratory signal analysis.

**Module separation:** Each DSP component (`fft`, `filter`, `wavegenerator`) is independently testable. This mirrors the modular design pattern used in production DSP pipelines and ML feature extraction systems.

---

## Features

- **Waveform synthesis** — sine, square, triangle, sawtooth, white noise with configurable frequency and amplitude
- **FFT analysis** — magnitude spectrum in dB, interactive frequency domain plot
- **Digital filtering** — single-pole IIR low-pass / high-pass with adjustable cutoff and sample rate, applied to the generated signal
- **WAV support** — import, visualize, and play PCM audio files (8/16/24/32-bit integer and 32/64-bit float, mono or multi-channel)
- **Real-time visualization** — synchronized time and frequency domain plots with zoom/pan

---

## Installation

```bash
git clone https://github.com/umutonuryasar/Signal-Processing-Toolbox.git
```

**Dependencies:** Qt 6 (widgets, printsupport, multimedia) · FFTW3 · QCustomPlot (vendored in this repo)

On Linux, FFTW is picked up through `pkg-config`:

```bash
sudo apt install qt6-base-dev qt6-multimedia-dev libfftw3-dev pkg-config
qmake6 && make
```

On Windows, point qmake at the FFTW folder:

```bash
qmake FFTW_DIR=C:/libs/fftw-3.3.5-dll64 && make
```

`FFTW_DIR` may also be set as an environment variable, and works on Linux too if
you prefer a local FFTW build over the pkg-config one.

---

## License

GPL-3.0 — see [LICENSE](LICENSE)
