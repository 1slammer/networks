import sys
import socket
import select
import array

class ChatClient(object):
	def __init__(self, serverIP, serverPort):
		# Cpreate a TCP/IP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.serverIP = serverIP

		# Connect the socket to the port where the server is listening
		server_address = (serverIP, serverPort)
		print >>sys.stderr, 'connecting to %s port %s' % server_address
		self.sock.connect(server_address)

	def run(self):
		self.prompt()
		finished = False
		while not finished:
			socket_list = [sys.stdin, self.sock]

			# Get list of sockets which are readable
			read_sockets, write_sockets, error_sockets = select.select(socket_list , [], [])

			for sock in read_sockets:
				# Incoming message from remote server
				if sock == self.sock:
					data = self.sock.recv(4096)
					if not data:
						print "Connection Closed"
						sys.exit()
					else:
						print "Them: " + str(data)
						self.prompt()

				else: # user entered a message
					msg = sys.stdin.readline()[:-1]

					if msg == "Bye Bye Birdie":
						finished = True
						self.finish()
						break
					packet = self.formPacket(msg)
					self.sendPacket(packet)
					self.prompt()

	def prompt(self):
		print "enter message: "
		sys.stdout.flush()

	def formPacket(self, msg):
		# I think we just send the msg, no header or anything fancy..
		packet = msg

		return packet

	def sendPacket(self, packet):
		self.sock.sendall(packet)


	def finish(self):
		self.sock.close()

class ChatServer(object):
	def __init__(self, clientPort):
		# Create a TCP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

		# Bind the socket to the port
		server_address = (socket.gethostname(), clientPort)
		print "starting up on %s port %s" % server_address
		self.sock.bind(server_address)

		# Listen for incoming connections
		self.sock.listen(1)

		# Wait for a connection
		print "Waiting for a connection"
		self.connection, client_address = self.sock.accept()

		self.run()


	def run(self):
		self.prompt()
		finished = False
		while not finished:
			socket_list = [sys.stdin, self.connection]

			# Get list of sockets which are readable
			read_sockets, write_sockets, error_sockets = select.select(socket_list , [], [])

			for sock in read_sockets:
				# Incoming message from remote server
				if sock == self.connection:
					data = self.connection.recv(4096)
					if not data:
						print "Connection Closed"
						sys.exit()
					else:
						print "Them: " + str(data)
						self.prompt()

				else: # user entered a message
					msg = sys.stdin.readline()[:-1]

					if msg == "Bye Bye Birdie":
						finished = True
						self.finish()
						break
					packet = self.formPacket(msg)
					self.sendPacket(packet)
					self.prompt()

	def prompt(self):
		print "You: "
		#sys.stdout.flush()

	def formPacket(self, msg):
		# I think we just send the msg, no header or anything fancy..
		packet = msg

		return packet

	def sendPacket(self, packet):
		self.connection.sendall(packet)


	def finish(self):
		self.connection.close()

class Client(object):

	REQUEST_INVALID = 0
	RESPONSE_READY = 1
	RESPONSE_WAITING = 2
	RESPONSE_INVALID = 3

	def __init__(self, serverName, serverPort, clientPort):
		self.sock = None
		self.clientPort = clientPort
		self.setup(serverName, serverPort, clientPort)

	def hasMagicNumber(self, response):
		#magic number is A5A5
		if response[0] == 165 and response[1] == 165:
			return True
		else:
			return False


	def setup(self, serverName, serverPort, clientPort):
		# Create a UDP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.server_address = (serverName, serverPort)

		print >> sys.stderr, 'connecting to %s port %s' % self.server_address

	def requestChat(self):
		request = self.formRequest()
		response = self.sendChatRequest(request)
		state = self.parseResponse(response)

		if state == self.REQUEST_INVALID:
			self.displayInvalidRequestMessage(request)
		elif state == self.RESPONSE_WAITING:
			self.setupChatServer()
		elif state == self.RESPONSE_READY:
			serverIP, serverPort = self.getServerInfo(response)
			self.connectToChatServer(serverIP, serverPort)
		
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
			#print "waiting for response from server"
			response, server = self.sock.recvfrom(4096)
			#print "response from server"
			#print response
		except Exception as e:
			print "Error receiving data from UDP server"
			sys.exit()

		return response

	def parseResponse(self, response):
		#print len(response)
		responseArray =  array.array('B', response)
		#print responseArray
		#print responseArray[0]


		# Check if response indicates error in our previous chat request packet
		if (len(responseArray) == 5) and (responseArray[3] == 00):
			self.createErrorMessage(responseArray[-1])
			return self.REQUEST_INVALID

		# Valid response from server here

		print "Magic number from Server: %x%x" % (responseArray[0], responseArray[0])
		
		# if reponse only magic number, GID, and port
		if len(responseArray) == 5:
			#See specs about print screen
			return self.RESPONSE_WAITING

		# if response says client already waiting to chat
		serverIP, serverPort = self.getServerInfo(response)
		print "IP of Waiting Client: %s" % serverIP
		print "Port of Waiting Client: %s" % serverPort
		return self.RESPONSE_READY


	def getServerInfo(self, response):
		responseArray = array.array('B', response)

		if not self.hasMagicNumber(responseArray):
			return self.RESPONSE_INVALID

		ip = [None]*4
		ip = (str(responseArray[5]) + "." + 
			  str(responseArray[4]) + "." +
			  str(responseArray[3]) + "." + 
			  str(responseArray[2]))

		port = (responseArray[6] << 8) + responseArray[7]

		return ip, port

	def checkLastByteForErrorCode(self, lastByte):
		print "last byte: %s" % lastByte
		if lastByte & 7:
			return True
		else:
			return False

	def createErrorMessage(self, errorCode):
		self.errorMessage = ""
		if errorCode & 1:
			self.errorMessage += "Client's request did not provide magic number!\n"
		if errorCode & 2:
			self.errorMessage += "Client's request in not a valid length!\n"
		if errorCode & 4:
			self.errorMessage += "Client's request port number is out of range!\n"


	def displayInvalidRequestMessage(self, request):
		print self.errorMessage
		print request
		print


	def displayInvalidResponseMessage(self, response):
		print "Invalid response from server"
		print response
		print

	def setupChatServer(self):
		print "waiting for a partner to connect"
		self.chatServer = ChatServer(self.clientPort)

	def connectToChatServer(self, serverIP, serverPort):
		self.chatClient = ChatClient(serverIP, serverPort)
		self.chatClient.run()

		


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
