import sys
import socket

class Server():
    def __init__(self, portNum):
        self.sock = None
        self.setup(portNum)


    def setup(self, portNum):
        # Create a UDP socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.code = 0

        # Bind the socket to the port
        server_address = (socket.gethostname(), portNum)
        print >> sys.stderr, 'starting up on %s port %s' % server_address
        self.sock.bind(server_address)
        

    def run(self):
        while True:
            print >>sys.stderr, '\nwaiting to receive message'
            request, address = self.sock.recvfrom(4096)
            request.encode("hex")

            print >>sys.stderr, 'received %s bytes from %s' % (len(request), address)
            print >>sys.stderr, request

            if request:
                valid = self.validateRequest(request)
                if valid:
                    response = self.generateValidRequestResponse(request)
                else:
                    response = self.generateInvalidRequestResponse(request, self.code)

                sent = self.sock.sendto(response, address)
                print >>sys.stderr, 'sent %s bytes back to %s' % (sent, address)


    def validateRequest(self, request):
        validMagicNumber = self.checkMagicNumber(request)
        validChecksum = self.checkChecksum(request)
        validLength = self.checkLength(request)

        if validMagicNumber and validChecksum and validLength:
            return True
        else:
            return False



    def checkMagicNumber(self, request):
        # Valid magic number is 0x1234
        # Magic number field is the first 2 bytes of the request
        if request[0:4] == "1234":
            return True
        else:
            self.code = 3
            return False

    def checkChecksum(self, request):
        # Valid checksum adds up to -1 (0xff)
        # Checksum field is the 5th byte of the request
        sum = 0
        for i in range(0, len(request), 2):
            sum += int(request[i:i+2], 16) & 0xff
            sum = (sum & 0xff) + (sum >> 8)
        # one's complement the result
        if sum & 0xff == 0xff:
            return True
        else:
            self.code = 2
            return False

    def checkLength(self, request):
        # Valid total message length field is equal to request length
        # Total message length is the 3-4 bytes of the request
        length = len(request) / 2
        # My initial thought was to do this, but I'm not sure if the len() function will accept
        # a list like this.
        strlength = int(request[4:8], 16)
        if length == strlength:
            return True
        self.code = 1
        return False


    def generateValidRequestResponse(self, request):
        # Resolve IP Addresses
        # Return a message with the following pieces in order:
        #     1.Header
        #         (magic number, total message length, checksum, group ID, requestID)
        #             Checksum here is calculated on the response datagram 
        #     2.IP Addresses of all hostnames, one after another
        #         In same order as hostnames
        ipAddresses = self.getIPAddresses(request)
        ipBytes = bytearray(ipAddresses, "utf-8")
        
        
        lengthMsg = 7 + len(ipBytes)
        x = self.intToBytes(lengthMsg) 
        returnData = bytearray(lengthMsg)
        returnData[0] = 0x12
        returnData[1] = 0x34
        returnData[2] = x[0]
        returnData[3] = x[1]
        returnData[5] = 0x09
        returnData[6] = 0x01
        returnData[7:lengthMsg] = ipBytes
        returnData[4] = self.calculateCheckSum(returnData)
        return returnData


    def getIPAddresses(self, request):
        firstLen = int(request[12:16], 16) & 0xff
        x = 16
        ipAddresses = ""
        for y in range(16, len(request), firstLen):
            hostname = request[x:x + firstLen * 2].decode("hex")
            #print request[16:20].decode("hex")
            addr = socket.gethostbyname(hostname)
            ipAddresses += addr
            x = firstLen * 2 + x
            if x + 2 > len(request):
                break
            firstLen = int(request[x - 2:x + 2], 16) & 0xff
            x = x + 2
        return ipAddresses


    def calculateCheckSum(self, request):
        """ Compute the Internet Checksum of the supplied data.  The checksum is
    initialized to zero.  Place the return value in the checksum field of a
    packet.  When the packet is received, check the checksum, by passing
    in the checksum field of the packet and the data.  If the result is zero,
    then the checksum has not detected an error.
    """
        sum = 0
        for i in range(0,len(request)):
            sum += request[i] & 0xff
            sum = (sum & 0xff) + (sum >> 8)
        # one's complement the result
        sum = ~sum
        return sum & 0xff

    def generateInvalidRequestResponse(self, request, code):
        # Return a message with the following pieces in order:
        #     1.Header
        #         (magic number total message length, checksum, group ID)
        #             Checksum here is calculated on the response datagram 
        #     2.Error Code BEC
        #         calculated as below:
        #              bad length: b0 = 1
        #             bad checksum: b1 = 1
        #             bad magic number: b2 = 1
        lengthMsg = 7
        returnData = bytearray(lengthMsg)
        returnData[0] = 0x12
        returnData[1] = 0x34
        returnData[2] = 0x00
        returnData[3] = 0x07
        returnData[5] = 0x09
        if code == 1:
            returnData[6] = 0x01
        if code == 2:
            returnData[6] = 0x02
        if code == 3:
            returnData[6] = 0x04
        return returnData
    

    def intToBytes(self, n):
        b = bytearray([0, 0])   # init
        b[1] = n & 0xFF
        n >>= 8
        b[0] = n & 0xFF
        return b
        
	def __init__(self, portNum):
		self.sock = None
		self.setup(portNum)


	def setup(self, portNum):
		# Create a UDP socket
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.code = 0

		# Bind the socket to the port
		server_address = (socket.gethostname(), portNum)
		print >> sys.stderr, 'starting up on %s port %s' % server_address
		self.sock.bind(server_address)
		

	def run(self):
		while True:
			print >>sys.stderr, '\nwaiting to receive message'
			request, address = self.sock.recvfrom(128)
			#print "request length"
			#print len(request)

			print >>sys.stderr, 'received %s bytes' % (len(request))
			#print >>sys.stderr, request.encode("hex")

			if request:
				valid = self.validateRequest(request)
				if valid:
					print "Valid Message"
					response = self.generateValidRequestResponse(request)
				else:
					print "Invalid Message"
					response = self.generateInvalidRequestResponse(request, self.code)

				#print response
				sent = self.sock.sendto(response, address)
				print >>sys.stderr, 'sent %s bytes back' % (sent)


	def validateRequest(self, request):
		validMagicNumber = self.checkMagicNumber(request)
		validChecksum = self.checkChecksum(request)
		validLength = self.checkLength(request)

		if validMagicNumber and validChecksum and validLength:
			return True
			#print "valid recieve packet"
		else:
			#print validMagicNumber
			#print validChecksum
			#print validLength
			return False



	def checkMagicNumber(self, request):
		# Valid magic number is 0x1234
		# Magic number field is the first 2 bytes of the request

		if request.encode("hex")[0:4] == "1234":
			return True
		else:
			self.code |= 0x4
			#print "invalid Magic number "
			#print self.code
			#print "\n\n\n"
			#print "check magic number"
			#print request.encode("hex")[0:4]
			#print "check magic number"
			return False

	def checkChecksum(self, request):
		# Valid checksum adds up to -1 (0xff)
		# Checksum field is the 5th byte of the request
		sum = 0
		#print "checksum"
		for i in range(0, len(request)*2 ,2):
			#print str(sum) + "\t" + str(int(request.encode("hex")[i:i+2], 16))
			sum += int(request.encode("hex")[i:i+2], 16)
			sum = (sum & 0xff) + (sum >> 8)
			#print sum
		# one's complement the result
		if sum & 0xff == 0xff:
			return True
		else:
			self.code |= 0x2
			#print "Invalid Checksum"
			return False

	def checkLength(self, request):
		# Valid total message length field is equal to request length
		# Total message length is the 3-4 bytes of the request
		length = len(request)
		# My initial thought was to do this, but I'm not sure if the len() function will accept
		# a list like this.
		strlength = int(request.encode("hex")[4:8], 16)
		#print length
		#print strlength
		if length == strlength:
			return True
		self.code |= 0x1
		#print "Invalid Length"
		return False


	def generateValidRequestResponse(self, request):
		# Resolve IP Addresses
		# Return a message with the following pieces in order:
		#	  1.Header
		#		  (magic number, total message length, checksum, group ID, requestID)
		#			  Checksum here is calculated on the response datagram 
		#	  2.IP Addresses of all hostnames, one after another
		#		  In same order as hostnames
		ipAddresses = self.getIPAddresses(request)
		#print "ipAddresses"
		#print ipAddresses
		#print "ipAddresses"
		#ipBytes = bytearray(ipAddresses, "utf-8")
		#print "ipAddresses"
		#print ipBytes
		#print "ipAddresses"

			
		
		
		lengthMsg = 7 + len(ipAddresses)*4
		x = self.intToBytes(lengthMsg) 
		returnData = bytearray(lengthMsg)
		returnData[0] = 0x12
		returnData[1] = 0x34
		returnData[2] = x[0]
		returnData[3] = x[1]
		returnData[5] = 0x09
		returnData[6] = 0x01

		for ipIdx,ip in enumerate(ipAddresses):
			ipParts = ip.split(".")
			for x in range(4):
				#print int(ipParts[x])
				returnData[7+x+ipIdx*4] = int(ipParts[x])

			temp = returnData[7+ipIdx*4]
			returnData[7+ipIdx*4] = returnData[7+ipIdx*4+1]
			returnData[7+ipIdx*4+1] = temp

			temp = returnData[7+ipIdx*4+2]
			returnData[7+ipIdx*4+2] = returnData[7+ipIdx*4+3]
			returnData[7+ipIdx*4+3] = temp


		returnData[4] = self.calculateCheckSum(returnData)
		#print "returnData"
		for x in returnData:
			#print x
			pass
		#print returnData
		return returnData


	def getIPAddresses(self, request):
		#print "GETIPADDRESSES\n\n"
		firstLen = int(request.encode("hex")[12:16], 16) & 0xff
		#print "firstlen"
		#print firstLen
		#print firstLen*2
		x = 16
		ipAddresses = []
		#print request

		idx = 12
		notDone = True
		
		while notDone:
			hostLength = int(request.encode("hex")[idx:idx+4], 16) & 0xff
			#print hostLength
			hostname = request.encode("hex")[idx+4:idx+4+hostLength*2]
			#print hostname.decode("hex")
			addr = socket.gethostbyname(hostname.decode("hex"))
			ipAddresses.append(addr)
			idx = idx+4+hostLength*2
			#print "idx after loop"
			#print idx
			#print len(request)
			if idx > len(request)*2:
				notDone = False

			

		#print "GETIPADDRESSES\n\n"





		#for y in range(16, len(request), firstLen):
		#	print x + firstLen*2 - x
		#	print x + firstLen*2+1 - x
		#	hostname = request.encode("hex")[x:x + firstLen*2]
		#	print hostname.decode("hex")
		#	addr = socket.gethostbyname(hostname.decode("hex"))
		#	print "addr"
		#	print addr
		#	ipAddresses.append(addr)
		#	x = firstLen * 2 + x
		#	if x + 2 > len(request):
		#		break
		#	firstLen = int(request.encode("hex")[x - 2:x + 2], 16) & 0xff
		#	x = x + 2
		return ipAddresses


	def calculateCheckSum(self, request):
		""" Compute the Internet Checksum of the supplied data.  The checksum is
	initialized to zero.  Place the return value in the checksum field of a
	packet.  When the packet is received, check the checksum, by passing
	in the checksum field of the packet and the data.  If the result is zero,
	then the checksum has not detected an error.
	"""
		sum = 0
		for i in range(0, len(request)):
			sum += request[i]
			sum = (sum & 0xff) + (sum >> 8)
			#print sum

		return ~sum & 0xFF

		#sum = 0
		#for i in range(0,len(request)):
		#	sum += request.encode("hex")[i] & 0xff
		#	sum = (sum & 0xff) + (sum >> 8)
		## one's complement the result
		#sum = ~sum
		#return sum & 0xff

	def generateInvalidRequestResponse(self, request, code):
		# Return a message with the following pieces in order:
		#	  1.Header
		#		  (magic number total message length, checksum, group ID)
		#			  Checksum here is calculated on the response datagram 
		#	  2.Error Code BEC
		#		  calculated as below:
		#			   bad length: b0 = 1
		#			  bad checksum: b1 = 1
		#			  bad magic number: b2 = 1
		lengthMsg = 7
		returnData = bytearray(lengthMsg)
		returnData[0] = 0x12
		returnData[1] = 0x34
		returnData[2] = 0x00
		returnData[3] = 0x07
		returnData[5] = 0x09
		returnData[6] = 0x00
		returnData[6] = code
		returnData[4] = self.calculateCheckSum(returnData)
		print "Error code sent: "
		print code
		return returnData
	

	def intToBytes(self, n):
		b = bytearray([0, 0])	# init
		b[1] = n & 0xFF
		n >>= 8
		b[0] = n & 0xFF
		return b

  


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
