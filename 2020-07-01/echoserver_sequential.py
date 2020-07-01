import socket

def handle(conn, addr):
  print('handling connection from', addr)
  while True:
    data = conn.recv(1024)
    if not data: break
    conn.sendall(data)
  conn.close()
  print('connection closed', addr)

def serve(host, port):
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.bind((host, port))
  print('listening on port', port)
  s.listen()
  
  while True:
    conn, addr = s.accept()
    handle(conn, addr)


serve("localhost", 4223)

