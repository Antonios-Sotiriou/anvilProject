'''
Asyncio Testing Server
'''
import asyncio

class EchoServerProtocol(asyncio.Protocol):

    def __init__(self):
        super().__init__()
        print('__init__(self)')

    def connection_made(self, transport):
        peername = transport.get_extra_info('peername')
        print('Connection from {}'.format(peername))
        self.transport = transport

    def data_received(self, data):
        message = data.decode()
        print("Server received Request: {")
        print("{:s}".format(message))
        print("}")

        self.transport.write(data)

        print("Close the client socket")
        self.transport.close()

    def connection_lost(self, exc):
        print("Server Connection Lost\n\n")
        self.transport.close()

    def eof_received(self):
        print("Eof received!")
        self.transport.close()
    
    def pause_writing(self):
        pass

    def resume_writing(self):
        pass

async def main():
    # Get a reference to the event loop as we plan to use
    # low-level APIs.
    loop = asyncio.get_running_loop()

    server = await loop.create_server(
        lambda: EchoServerProtocol(),
        "127.0.0.1", 8887)

    print("Server running...\n")
    async with server:
        await server.serve_forever()

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("Server terminated")


