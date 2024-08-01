"""
The following code implements UDP server which receives data
from remote client. This script aims to test TelemetrySender implmenetation.
"""
import socket


UDP_IP = "0.0.0.0"  
UDP_PORT = 1337     

if __name__ == "__main__":

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  
    sock.bind((UDP_IP, UDP_PORT))

    print(f"Telemetry receiver listening on {UDP_IP}:{UDP_PORT}")

    while True:
        data, addr = sock.recvfrom(1024)  # Buffer size of 1024 bytes
        telemetry_str = data.decode('utf-8')

        # Parse the telemetry string
        try:
            telemetry_values = [float(x) for x in telemetry_str.split()] 
            print(f"Received telemetry: {telemetry_values} from {addr}")

        except ValueError:
            print(f"Invalid telemetry format: {telemetry_str}")