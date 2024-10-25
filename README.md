# Signal Processing Toolbox

A versatile Qt-based toolkit offering essential signal processing functionalities, including waveform generation, time-domain visualization, Fourier analysis, and filtering.

## Features

- **Waveform Generation**: Create various waveforms to simulate different signals:
  - **Sine Wave**: Represents a pure frequency with no harmonics, ideal for testing linear systems.
  - **Square Wave**: Contains odd harmonics, commonly used in digital signal applications.
  - **Triangle Wave**: Linear rise and fall, often used to test the linearity of systems.
  - **Sawtooth Wave**: Contains both even and odd harmonics, suitable for sound synthesis and analysis.
  - **White Noise**: Random signal with a flat frequency spectrum, useful for testing system response.

- **Time-Domain Visualization**: Interactive plots to view signals over time using QCustomPlot. Features include:
  - **Zoom and Pan**: Easily zoom in on specific portions of the signal and pan across the timeline.
  - **Dynamic Updates**: Real-time plotting capabilities to visualize signals as they are generated.

- **Frequency-Domain Analysis**: Fast Fourier Transform (FFT) visualization with magnitude plotting:
  - **FFT Calculation**: Computes the frequency spectrum of the input signal to analyze its frequency components.
  - **Magnitude Plotting**: Visualizes the magnitude of different frequency components in decibels (dB).
  - **Interactive Frequency Plots**: Zoom and pan across the frequency domain for detailed analysis.

- **Filtering**: Apply digital filters to signals for real-time signal processing:
  - **Low Pass Filter**: Removes high-frequency components, allowing low-frequency components to pass through.
  - **High Pass Filter**: Removes low-frequency components, allowing high-frequency components to pass through.
  - **Adjustable Parameters**: Customize cutoff frequency and sampling frequency to achieve desired filtering effects.

- **WAV File Support**: Load and analyze WAV files to visualize and process audio signals:
  - **File Import**: Import WAV files for playback and signal analysis.
  - **Graphical Analysis**: Plot the time-domain waveform and apply FFT to visualize the frequency components.
  - **Playback Controls**: Play, pause, and stop audio files with real-time graphical updates.

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/umutonuryasar/Signal-Processing-Toolbox.git
   ```
2. Install dependencies (Qt, FFTW, QCustomPlot).
   - **Qt**: Install Qt Creator or the Qt libraries.
   - **FFTW**: Install the FFTW library for performing Fourier transforms.
   - **QCustomPlot**: Include QCustomPlot for plotting functionalities.
3. Build and run using Qt Creator or a preferred IDE.

## Usage

- **Generate Signals**: Create different waveforms (sine, square, triangle, sawtooth, white noise) and visualize them in the time domain.
- **Visualize Time Domain**: View generated signals in the time domain with interactive zoom and pan features.
- **Analyze Frequency Domain**: Compute the FFT of generated or imported signals to view their frequency components.
- **Apply Filters**: Use low-pass or high-pass filters to modify the generated signals and observe their real-time effects.
- **Load WAV Files**: Import WAV files to visualize and analyze their time and frequency domain representations.
- **Playback Audio**: Play loaded WAV files with synchronized time-domain and frequency-domain plots.

## Dependencies

- **Qt**: GUI framework for building the user interface and managing interactions.
- **FFTW**: Fast Fourier Transform library for efficient computation of FFT.
- **QCustomPlot**: Plotting library for interactive time-domain and frequency-domain graphs.

## License

Licensed under the [GNU GPL-3.0 License](LICENSE).
