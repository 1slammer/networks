import sys
import socket

class Client():

	CHAT_INVALID = 0
	CHAT_READY = 1
	CHAT_WAITING = 2

	def __init__(self, serverName, serverPort, clientPort):
		self.sock = None
		self.setup(serverName, serverPort, clientPort)


	def setup(self, serverName, serverPort, clientPort):
		# Create a TCP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

		# Connect the socket to the port where the server is listening
		server_address = (serverName, serverPort)
		print >> sys.stderr, 'connecting to %s port %s' % server_address
		self.sock.connect(server_address)

	def requestChat(self):
		request = self.formRequest()
		response = self.sendChatRequest(request)
		state = self.parseResponse(response)

		if state == self.CHAT_INVALID:
			pass
		elif state == self.CHAT_WAITING:
			pass
		elif state == self.CHAT_READY:
			pass
		
	def formRequest(self):
		request = None

		return request

	def sendChatRequest(self, request):
		response = None

		return response

	def parseResponse(self, response):
		# if invalid response
		return self.CHAT_INVALID

		# if reponse only magic number
		return self.CHAT_WAITING

		# if response complete
		return self.CHAT_READY




		
  


def main(serverName, serverPort, clientPort):
	client = Client(serverName, serverPort, clientPort)
	client.requestChat()


if __name__ == '__main__':
	if len(sys.argv) != 4:
		print "Proper usage:\n\tTCPClient.py ServerName ServerPort MyPort"
		sys.exit()

	try:
		# Convert portNum argument to int
		serverName = str(sys.argv[1])
		serverPort = int(sys.argv[2])
		clientPort = int(sys.argv[3])
	except ValueError as e:
		print e
		sys.exit()

	main(serverName, serverPort, clientPort)
