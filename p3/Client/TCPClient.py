import sys
import socket

class ChatClient(object):
	def __init__(self, serverIP, serverPort):
		# Cpreate a TCP/IP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

		# Connect the socket to the port where the server is listening
		server_address = (serverIP, serverPort)
		print >>sys.stderr, 'connecting to %s port %s' % server_address
		self.sock.connect(server_address)

	def run(self):
		finished = False
		while not finished:
			socket_list = [sys.stdin, s]

			# Get list of sockets which are readable
			read_sockets, write_sockets, error_sockets = select.select(socket_list , [], [])

			for sock in read_sockets:
				# Incoming message from remote server
				if sock == s:
					data = sock.recv(4096)
					if not data:
						print "what happened?"
					else:
						print data
						self.prompt()

				else: # user entered a message
					msg = sys.stdin.readline()
					if msg == "Bye Bye Birdie":
						finished = False
						self.finish()
						break
					packet = formPacket(msg)
					sendPacket(packet)
					self.prompt()

	def prompt():
		print "enter message"
		sys.stdout.flush()

	def formPacket(msg):
		# I think we just send the msg, no header or anything fancy..
		packet = msg

		return packet

	def sendPacket(packet):
		self.sock.sendall(packet)



	def finish(self):
		self.sock.close()

class ChatServer(object):
	def __init__(self, clientPort):
		# Create a TCP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

		# Bind the socket to the port
		server_address = ('localhost', clientPort)
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

class Client(object):

	RESPONSE_INVALID = 0
	RESPONSE_READY = 1
	RESPONSE_WAITING = 2

	def __init__(self, serverName, serverPort, clientPort):
		self.sock = None
		self.clientPort = clientPort
		self.setup(serverName, serverPort, clientPort)


	def setup(self, serverName, serverPort, clientPort):
		# Create a UDP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.server_address = (serverName, serverPort)

		print >> sys.stderr, 'connecting to %s port %s' % self.server_address

	def requestChat(self):
		request = self.formRequest()
		response = self.sendChatRequest(request)
		print
		print response
		print
		state = self.parseResponse(response)

		if state == self.RESPONSE_INVALID:
			self.displayInvalidResponseMessage(response)
		elif state == self.RESPONSE_WAITING:
			self.setupChatServer()
		elif state == self.RESPONSE_READY:
			self.connectToChatServer()
		
	def formRequest(self):
		request = bytearray(5)
		request[0] = 0xA5					# first part of magic number
		request[1] = 0xA5					# second part of magic number
		request[2] = self.clientPort >> 8	# port MSB
		request[3] = self.clientPort & 0xFF	# port LSB
		request[4] = 0x09					# Group ID

		return request

	def sendChatRequest(self, request):
		try:
			sent = self.sock.sendto(request, self.server_address)
			print request
			print len(request)
			print sent
		except Exception as e:
			print "Error sending chat request to UDP server"
			sys.exit()

		try:
			print "waiting for response from server"
			response, server = self.sock.recvfrom(4096)
			print "response from server"
			print response
		except Exception as e:
			print "Error receiving data from UDP server"
			sys.exit()

		return response

	def parseResponse(self, response):
		# if invalid response
		return self.RESPONSE_INVALID

		# if reponse only magic number
		return self.RESPONSE_WAITING

		# if response complete
		return self.RESPONSE_READY

	def verifyMagicNumber(self, response):
		pass



	def displayInvalidResponseMessage(self, response):
		print "Invalid response from server"
		print response

	def setupChatServer(self):
		print "waiting for a partner to connect"

		self.chatServer = ChatServer(self.clientPort)



	def connectToChatServer(self):
		self.chatClient = ChatClient(serverIP, serverPort)

		


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
