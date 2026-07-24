# Fair Share CPU Scheduling Simulator

​	This project provides a CPU scheduling simulator using algorithms taken from the fair share CPU scheduling family. the two algorithms I chose are Weighted Fair Queueing and Virtual Round Robin.

## Project Status

​	The Weighted Fair Queue simulator is complete, but I ran into problems with the Virtual Round Robin algotithm when it came to varrying the number of processes. So as a consequence the VVR simulator only uses 3 processes

| Algorithm                    | Code files                      | Notes                                                        |
| ---------------------------- | ------------------------------- | ------------------------------------------------------------ |
| Weighted Fair Queueing (WFQ) | Index.js, index.html, index.css | the html, castcading style sheets, and java script to load into the webpage. |
| Virtual Round Robin (VRR)    | Index.js, index.html, index.css | the html, castcading style sheets, and java script to load into the webpage. |

## Requirements

- Web server
- Web browser
- Python3

## How to run

1. Open a Terminal on your computer

2. Create a new directory for the 3 code files

3. Place the 3 code files in your newly created directory on your computer.

4. cd (change directory) to the directory you just created

5. Use the following command to start a simple python web server:

   ​	**Python3    -m    http.server 8000**

6. Open a web browser and enter the following web address:

   ​	**localhost:8000**

   ## Usage

   1. After opening the web page, by default you'll be presented with the Weighted Fair Queueing simulator. There is a additional button for the Virtual Round Robin.

      ### Using the Weighted Fair Queueing simulator(WFQ)

      1. Enter the number of processes

      2. Enter the total usage seconds

      3. The results will display how the CPU prioritizes processes

         ### Using the Virtual Round Robin (VRR)

      1. Enter the Base Time Quantum 
      2. Enter the Max Burst
      3. The number of processes is automatically set at 3 and cannot be changed since i didn't want to give the false impression that its working.
      4. The results shows how the CPU schedules processes in a FIFO queue.

