# DronePositioningV2
A training system for UAV operators using Augmented Reality technologies

## Architecture overview
### General
 ![General scheme](docs\DronePositioningGeneralScheme.drawio.png)

The scheme above is the general depiction of the system, which consists of three separate components:
- UAV: sends telemetry information: GPS&angular position, velocity and acceleration vectors through radio signals.
- Windows desktop application: serves a role of both client listening for incoming UAV telemetry and server that sends position data to HL2 through UDP. It calculates accuracy metrics for flight trajcetory corectness evaluation and generates a final report after the session terminates.
- Microsoft HoloLens2: runs Unity application that receives UAV position as a UDP client and updates the scene. It is responsible for displaying of visual guidance tips for operator as well as visual feeback regarding colision with obstacles.

### Windows desktop application
TODO

### Microsoft HoloLens2
TODO

### UAV
TODO