import sys
import socket

class Client():

	RESPONSE_INVALID = 0
	RESPONSE_READY = 1
	RESPONSE_WAITING = 2

	def __init__(self, serverName, serverPort, clientPort):
		self.sock = None
		self.clientPort = clientPort
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

		if state == self.RESPONSE_INVALID:
			self.displayInvalidResponseMessage(response)
		elif state == self.RESPONSE_WAITING:
			self.waitForChatPartner()
		elif state == self.RESPONSE_READY:
			self.connectToChatServer()
		
	def formRequest(self):
		request = None

		return request

	def sendChatRequest(self, request):
		response = None

		return response

	def parseResponse(self, response):
		# if invalid response
		return self.RESPONSE_INVALID

		# if reponse only magic number
		return self.RESPONSE_WAITING

		# if response complete
		return self.RESPONSE_READY



	def displayInvalidResponseMessage(self, response):
		print "Invalid response from server"
		print response

	def waitForChatPartner(self):
		print "waiting for a partner to connect"
		self.setUpChatServer()

	def setUpChatServer(self):
		# Create a TCP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

		# Bind the socket to the port
		server_address = ('localhost', self.clientPort)
		print "starting up on %s port %s" % server_address
		self.sock.bind(server_address)

		# Listen for incoming connections
		sock.listen(1)

		while True:
			# Wait for a connection
			print "Waiting for a connection"
			connection, client_address = sock.accept()

			try:
				print "connection from %s" % client_address

				# Recieve data in a single chunk
				data = connection.recv(256)
				print "received %s" % data

			finally:
				connection.close()


	def connectToChatServer(self):
		pass

		


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
