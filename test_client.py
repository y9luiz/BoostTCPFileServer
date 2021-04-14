import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('localhost', 13)
print(sys.stderr, 'connecting to %s port %s' % server_address)
sock.connect(server_address)
try:
    # Send data
    header = ""
    header = sock.recv(4)
    received = 0
    body = ""
    print(header)
    while(received < int(header)):
        body  = sock.recv(int(header))
        received += len(body) 
    print(body)

finally:
    print("closing socket")
    sock.close()