# PNW Senior Design 2024

A team of four engineering students, including me, was assigned the following project as part of our senior-year coursework.

## Stacked Vertical Axis Wind Turbine (VAWT) Prototype Project
This project explored the viability of mounting multiple vertical axis wind turbines on top of each other as a modular alternative to larger, single turbines. For the purposes of data collection, we used an Arduino Uno with peripherals to monitor the voltage output of the turbines on a static load, as well as the analog signal from an anemometer to measure local wind speed. This data was automatically measured, tabulated, and exported to an SD card as a .csv file for later analysis.

Additionally, because wind turbines cannot run optimally under a static load, I attempted to use PWM to dynamically load the wind turbine based on the current power output.

The .ino file included in this repo is the result of my efforts to write a program that would meet these requirements.

Unfortunately, due to oversights in our design, the project was unsuccessful. Although the data we collected was accurate, the mechanical modifications made to the VAWT's resulted in them being unable to operate under any significant load.

<img width="768" height="1024" alt="image0" src="https://github.com/user-attachments/assets/b18ccd6d-fdf4-4d39-9124-7ddf1be64cd7" />
