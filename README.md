# CS-596-Project

## Circuit Diagram Connections

This table outlines the connections for the components in the circuit diagram.

| Component        | Connection to TTGO Board | Other Connections                      | Notes                                      |
|-----------------|-------------------------|---------------------------------------|--------------------------------------------|
| **TTGO** |                         |                                       | Main microcontroller board                 |
| Power (+)       | 3V                      |                                       | Positive voltage supply                    |
| Power (-)       | G                       | 2nd Breadboard (-)                      | Ground connection                          |
| Green Button    | 15                      | Button                                | Input for the green button                 |
| Red Button      | 2                       | Button                                | Input for the red button                   |
| LED 1           | 26                      | Long leg of LED                       |                                            |
| LED 2           | 25                      | Long leg of LED                       |                                            |
| LED 3           | 33                      | Long leg of LED                       |                                            |
| LED 4           | 32                      | Long leg of LED                       |                                            |
| Photoresistor 1 | 39                      | Photoresistor & Resistor              | Analog input for light sensing             |
| Photoresistor 2 | 38                      | Photoresistor & Resistor              | Analog input for light sensing             |
| Photoresistor 3 | 37                      | Photoresistor & Resistor              | Analog input for light sensing             |
| Photoresistor 4 | 36                      | Photoresistor & Resistor              | Analog input for light sensing             |
| **LED** |                         |                                       | Light Emitting Diode                       |
| Anode (+)       | Pins 25, 26, 32, 33     | Long leg                              | Connect to the specified TTGO pins       |
| Cathode (-)     | 2nd G                   | Short leg                             | Connect to the ground on the 2nd breadboard |
| **Photoresistor**|                         |                                       | Light-dependent resistor                   |
| Pin 1           | Pins 36, 37, 38, 39     | Resistor                              | Connected to TTGO and a resistor           |
| Pin 2           | +                       | Positive power supply                   | Connected to the positive voltage          |
| **Button** |                         |                                       | Momentary push button                      |
| Pin 1           | 15                      | Green Button                          |                                            |
| Pin 2           | 2                       | Red Button                            |                                            |
| Common Ground   | G                       |                                       | Connect to ground                            |
| **Resistor** |                         |                                       | Current-limiting resistor for photoresistor |
| Pin 1           | Pins 36, 37, 38, 39     | Photoresistor                         | Connected to TTGO and the photoresistor    |
| Pin 2           | G                       | Ground (-)                            | Connected to ground                          |
| **Breadboard** |                         |                                       | Prototyping board                          |
| Ground (-)      | G (from TTGO)           | Short legs of LEDs, Resistors, Button | Common ground rail                         |
