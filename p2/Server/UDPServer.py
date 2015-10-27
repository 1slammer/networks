import sys
import socket

class Server():
	def __init__(self, portNum):
		self.sock = None
		self.setup(portNum)


	def setup(self, portNum):
		# Create a UDP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

		# Bind the socket to the port
		server_address = ('localhost', portNum)
		print >> sys.stderr, 'starting up on %s port %s' % server_address
		self.sock.bind(server_address)
		

	def run(self):
		while True:
			print >>sys.stderr, '\nwaiting to receive message'
			data, address = self.sock.recvfrom(4096)

			print >>sys.stderr, 'received %s bytes from %s' % (len(request), address)
			print >>sys.stderr, request

			if request:
				valid = validateRequest(request)
				if valid:
					response = generateValidRequestResponse(request)
				else:
					response = generateInvalidRequestResponse(request)

				sent = self.sock.sendto(response, address)
				print >>sys.stderr, 'sent %s bytes back to %s' % (sent, address)


	def validateRequest(request):
		validMagicNumber = checkMagicNumber(request)
		validChecksum = checkChecksum(request)
		validLength = checkLength(request)

		if validMagicNumber and validChecksum and validLength:
			return True
		else:
			return False



	def checkMagicNumber(request):
		# Valid magic number is 0x1234
		# Magic number field is the first 2 bytes of the request
		pass

	def checkChecksum(request):
		# Valid checksum adds up to -1 (0xff)
		# Checksum field is the 5th byte of the request
		pass

	def checkLength(request):
		# Valid total message length field is equal to request length
		# Total message length is the 3-4 bytes of the request
		pass



	def generateValidRequestResponse(request):
		# Resolve IP Addresses
		# Return a message with the following pieces in order:
		#	1.Header
		#		(magic number, total message length, checksum, group ID, requestID)
		#			Checksum here is calculated on the response datagram 
		#	2.IP Addresses of all hostnames, one after another
		#		In same order as hostnames
		pass

	def generateInvalidRequestResponse(request):
		# Return a message with the following pieces in order:
		#	1.Header
		#		(magic number total message length, checksum, group ID)
		#			Checksum here is calculated on the response datagram 
		#	2.Error Code BEC
		#		calculated as below:
		# 			bad length: b0 = 1
		#			bad checksum: b1 = 1
		#			bad magic number: b2 = 1
		pass


def main(portNum):
	server = Server(portNum)
	server.run()


if __name__ == '__main__':
	if len(sys.argv) != 2:
		print "Proper usage:\n\tUDPServer.py port#"
		sys.exit()

	try:
		# Convert portNum argument to int
		portNum = int(sys.argv[1])
	except ValueError as e:
		print e
		sys.exit()

	main(portNum)

