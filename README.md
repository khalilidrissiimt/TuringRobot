# TuringRobot
This project encompasses the development of an autonomous robot designed to simulate the universal turing machine. It leverages the MeMegaPi board, making it compatible with a wide range of modules and sensors for diverse robotic applications.
This repository hosts a Python script and a C++ program designed to work together to control a robot using a Finite State Machine (FSM) described in a CSV file.

## Description

The project includes:

- A Python script (`turing.py`) that reads a CSV file representing FSM transitions and prepares the data for use in the C++ program.
- A C++ application (`robotCode.cpp`) that implements the FSM to control the robot's movements and actions based on its current state and inputs from its environment.

## Features

The robot operates continuously, cycling through a loop where it reads sensor data, processes this information to determine its current state, and executes corresponding actions. These actions include object manipulation, movement, and reactive adjustments based on ultrasonic sensor readings.

![Robot in Action](images/robot.jpg)

## Python CSV Reader for FSM Data

The Python script parses a CSV file with columns for `state`, `current_value`, `value_to_write`, `displacement`, and `next_state`. Each line in the CSV represents a state transition in the FSM.

### Usage

To run the script and parse the FSM data:

```bash
python turing.py fsm_data.csv
``` 

## C++ Robot Control System using FSM Data

The `robot_control.cpp` file implements a robot control system using FSM data provided by the output of the Python script. The C++ program defines several arrays corresponding to each column in the CSV file: `states`, `current_values`, `values_to_write`, `displacements`, and `next_states`. These arrays drive the robot's behavior as dictated by the FSM.

### C++ Array Initialization from FSM Data

The FSM data is represented as arrays in C++:

```cpp
const int numberOfEntries = 6; // Adjust the number of entries based on the CSV data
std::string states[numberOfEntries] = {"init", "e1", "e1", "e1", "e2", "e2"};
std::string current_values[numberOfEntries] = {"vide", "1", "0", "vide", "vide", "0"};
std::string values_to_write[numberOfEntries] = {"vide", "0", "0", "1", "1", "0"};
std::string displacements[numberOfEntries] = {"droite", "droite", "droite", "gauche", "droite", "gauche"};
std::string next_states[numberOfEntries] = {"e1", "e1", "e1", "e2", "fin", "e2"};
```

This program is expected to run in a robotic control system, responding to sensor inputs and controlling actuators as per the FSM logic.



