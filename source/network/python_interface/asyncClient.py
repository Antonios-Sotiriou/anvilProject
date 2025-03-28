'''
Asyncio Testing Client
'''
import asyncio

class EchoClientProtocol(asyncio.Protocol):

    def __init__(self, message, on_con_lost):
        print('__init__(self)')
        self.message = message
        self.on_con_lost = on_con_lost

    def connection_made(self, transport):
        peername = transport.get_extra_info('peername')
        print('Connecting to {}'.format(peername))
        transport.write(self.message.encode())
        self.transport = transport

    def data_received(self, data):
        print("Client Received Response: {")
        print("{:s}".format(data.decode()))
        print("}")

    def connection_lost(self, exc):
        print("Client Connection Lost\n\n")
        self.on_con_lost.set_result(True)
        
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

    on_con_lost = loop.create_future()
    message = "GET /test_request"

    transport, protocol = await loop.create_connection(
        lambda: EchoClientProtocol(message, on_con_lost),
        "127.0.0.1", 8887)

    # Wait until the protocol signals that the connection
    # is lost and close the transport.
    try:
        await on_con_lost
    finally:
        transport.close()

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except ConnectionRefusedError:
        print("Connection refused. Server may not be running")


