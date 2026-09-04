# PNW Senior Design 2024

The following project was assigned to a team of four engineering students, including myself, as part of our course work during our senior year of university.

## Stacked Vertical Axis Wind Turbine (VAWT) Prototype Project
The purpose of this project was to explore the viability of mounting multiple vertical axis wind turbines on top of each other as a modular alternative to larger, singular turbines. For the purposes of data collection, we used an Arduino Uno with peripherals to monitor the voltage output of the turbines on a static load, as well as the analog signal from an anemometer to measure local wind speed. This data was automatically measured, tabulated, and exported to an SD card as a .csv file for later analysis.

Additionaly, because wind turbines cannot run optimally under a static load, I attempted to use PWM to dynamicaly load the wind turbine based off the current power output. 

The .ino file included in this repo is the result of my efforts to write a program that would meet these requirements.

Unfortunately, the project as a whole was unsuccessful, due to oversights in our design. Although the data we collected was accurate, the mechanical modifications made to the VAWT's made them unable to operate under any significant load.

<img width="768" height="1024" alt="image0" src="https://github.com/user-attachments/assets/b18ccd6d-fdf4-4d39-9124-7ddf1be64cd7" />
