# Signal-Processing-Toolbox

The Signal Processing Application is a Qt-based project that provides tools for signal generation, time-domain signal visualization, frequency-domain analysis, audio processing, and filtering. It utilizes the Qt framework and the FFTW library for signal processing.

## Table of Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [Dependencies](#dependencies)
- [Project Structure](#project-structure)
- [Upcoming Features](#upcoming-features)
- [Contributing](#contributing)
- [License](#license)

## Introduction

The Signal Processing Application is designed to assist users in generating, analyzing, and processing various types of signals. Whether you are an engineer, researcher, or hobbyist, this application can help you with tasks such as waveform generation, time-domain visualization, frequency-domain analysis, audio processing, and filtering.

## Installation

To run the Signal Processing Application on your local machine, follow these steps:

1. Clone this repository to your local machine:

   ```bash
   git clone https://github.com/umutonuryasar/Signal-Processing-Toolbox.git

2. Install the necessary dependencies. This project relies on the Qt framework, FFTW, and QCustomPlot for its functionality. You should install these dependencies before building the project.

- Qt
- FFTW
- QCustomPlot

   Depending on your platform, you can install these dependencies using package managers, binary distributions, or by building from source.

3. Build and run the application using Qt Creator or your preferred IDE.

## Usage

The Signal Processing Application offers the following features:

Waveform Generation: You can create various types of waveforms, including Sine, Square, Triangle, and Sawtooth, by specifying amplitude, frequency, and sampling frequency.

Time-Domain Visualization: Visualize generated signals in the time domain using interactive plots. The Time Domain Graph provides insights into signal behavior over time.

Frequency-Domain Analysis: Analyze signals in the frequency domain using Fast Fourier Transform (FFT). The Frequency Domain Graph displays signal magnitudes in decibels (dB) across different frequency components.

Audio Processing (Upcoming): Perform audio processing tasks such as filtering, noise reduction, and audio synthesis. This feature is currently under development.

Filtering (Upcoming): Apply various digital filters to signals, allowing for noise reduction and signal enhancement. This feature is currently under development.

Detailed usage instructions and examples can be found in the application's user interface.

## Dependencies

The Signal Processing Application relies on the following dependencies:

Qt: A powerful C++ framework used for developing cross-platform applications.

FFTW (Fastest Fourier Transform in the West): A library for efficient computation of the discrete Fourier transform (DFT).

QCustomPlot: A Qt-based plotting library for creating interactive graphs and charts.

Make sure to install these dependencies as described in the Installation section before building the application.

## Project Structure

TimeDomainPlot.h
This header file defines the TimeDomainPlot class responsible for plotting time domain graphs.

WaveGenerator.h
This header file defines the WaveGenerator class responsible for generating different types of waveforms.

FFT.h
This header file defines the FFT class, which encapsulates the Fast Fourier Transform (FFT) computation.

MainWindow.h
The MainWindow class represents the main application window and handles user interactions.

Additional source files and forms contribute to the project's functionality, which can be explored further in the source code.

## Upcoming Features

Audio Processing (Under Development)
The Signal Processing Application is working on introducing audio processing capabilities. Users will be able to perform tasks like audio filtering, noise reduction, and audio synthesis.

Filtering (Under Development)
Filtering functionality is being added to the application. Users will have the ability to apply various digital filters to signals, enabling noise reduction and signal enhancement.

## Contributing

Contributions to the Signal Processing Application are welcome. To contribute:

Fork the repository.
Create a new branch for your feature or bug fix: git checkout -b feature/my-feature or git checkout -b bug/fix-my-bug.
Make your changes and commit them: git commit -m 'Add my feature'.
Push to your branch: git push origin feature/my-feature.
Create a pull request.
Please follow the Contributing Guidelines for more details.

## License

This project is licensed under the GNU GPL-3.0 License.
