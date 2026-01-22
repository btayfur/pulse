# Pulse

An Accelerometer system for Operational Modal Analysis

## Project Overview

This project allows you to connect and read data from four ADXL355 accelerometers simultaneously (Almost. But for most structure it is more than enough.) using a microcontroller and I2C multiplexer. The system offers two implementations:

1. **Text-based output** (`I2c_switcher`): Sends accelerometer data in human-readable, tab-separated format
2. **Binary protocol** (): This part of project is ready but I couldnt optimize the system. When it is complete, it should be much more faster than the original version.

## Hardware

### Components
- A microcontroller:
Most microcontroller can be used but for high frequencies, 80 MHz and higher clock speeds are needed. I used Arduino Due (84 MHz) in first version but upgraded to ESP32 ESP-32S (160 MHz) later. I'll upgrade it to Teensy 4.x eventually. If you check the board scheme microcontroller part can be replaced easily. Only 4 jumper should be connected (3.3v, GND, SDA, SCL).
- TCA9548A I2C multiplexer
- Four ADXL355 accelerometers
- Supporting circuitry as shown in the schematic (see `board/scheme.JPG`), I'll add fritzing scheme as soon as possible.
- Cat6 or higher cables between circuit and accelerometers.
- A lot of RJ45

Using cat cables can be not the best solution and it could be an overkill. I need to explain this section. Im not a professional on this subject, I spend serious time for this project. In forums people says SPI is more suitable for long cabling. But frequency was a bottleneck for my specific example. I2C offers high sampling rates but shorter cables are recommended. So I used cat cables to overcome this situation and it worked well.

## Directory Structure

```
├── I2c_switcher/               # Text-based implementation
│   └── I2c_switcher.ino        # Arduino code for text output
├── binary_i2c_switcher/        # Binary protocol implementation (Not ready yet)
│   ├── binary_i2c_switcher.ino # Arduino code for binary output
│   ├── read_binary_data.py     # Python script to read binary data (There will be a C# software with an interface also)
│   └── requirements.txt        # Python dependencies
└── board/                      # Hardware design files
    ├── scheme.JPG              # Circuit schematic
    ├── board.pdf               # PCB layout
    └── devre.pdsprj            # Proteus project file
```

## Getting Started

### Hardware Setup

1. Assemble the hardware according to the schematic in `board/scheme.JPG`. You can use .pdsprj file to create your own board.
2. Connect the microcontroller to your computer via USB

### Text-based Implementation

1. Open `I2c_switcher/I2c_switcher.ino` in the Arduino IDE
2. Upload the sketch to your Arduino
3. Open the Serial Monitor at 500000 baud (Higher baudrates can be used but needs to test first.)
4. Data will be displayed in tab-separated format: `X1 Y1 Z1 X2 Y2 Z2 X3 Y3 Z3 X4 Y4 Z4` You need to use your own timestamp. Arduino IDE works well.

Project's current bottleneck is SerialPort for now. In most setups, you can get easily 200-300 Hz sampling rates.


## License

This project is open-source. Please include appropriate attribution if you use or modify this work.

## Contact

For questions or support, please open an issue on GitHub. If you'd like to contribute to this project, pull requests are welcome! Feel free to improve the code, documentation, or suggest new features.

## Citation

If you use this project in your research, please cite the following paper:

```
DOI: 10.1016/j.istruc.2025.110190
```
