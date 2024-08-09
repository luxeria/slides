import asyncio

async def handle(reader, writer):
  addr = writer.get_extra_info('peername')
  print('handling connection from', addr)
  while True:
    data = await reader.read(1024)
    if not data:
        break
    writer.write(data) # in the background

  writer.close()
  print('connection closed', addr)

async def serve(host, port):
  server = await asyncio.start_server(handle, host, port)
  print('listening on port', port)
  async with server:
    await server.serve_forever()

asyncio.run(serve("localhost", 4223))

