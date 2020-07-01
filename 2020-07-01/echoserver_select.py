import socket
import select

def serve(host, port):
  server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  server_socket.bind((host, port))
  print('listening on port', port)
  server_socket.listen()
  
  read_waitlist = [server_socket]
  write_waitlist = []
  data = {}
  while True:
    readable, writable, errored = select.select(read_waitlist, write_waitlist, [])

    for s in readable:
        if s is server_socket:
            conn, addr = server_socket.accept()
            read_waitlist.append(conn)
            print('accepted connection', addr)
        else:
            print('reading from', s.getpeername())
            read_waitlist.remove(s)
            buf = s.recv(1024)
            if not buf:
                print('closing', s.getpeername())
                s.close()
                continue
            data[s] = buf
            write_waitlist.append(s)

    for s in writable:
        write_waitlist.remove(s)
        print('writing to', s.getpeername())
        buf = data.pop(s)
        s.sendall(buf)
        read_waitlist.append(s)

serve("localhost", 4223)

