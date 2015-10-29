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
            request, address = self.sock.recvfrom(4096)

            print >>sys.stderr, 'received %s bytes from %s' % (len(request), address)
            print >>sys.stderr, request

            if request:
                valid = self.validateRequest(request)
                if valid:
                    response = self.generateValidRequestResponse(request)
                else:
                    response = self.generateInvalidRequestResponse(request)

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
            return False

    def checkLength(self, request):
        # Valid total message length field is equal to request length
        # Total message length is the 3-4 bytes of the request
        length = len(request)
        # My initial thought was to do this, but I'm not sure if the len() function will accept
        # a list like this.
        strlength = str(request[2:4])
        lengthIn = request[2] + request[3]
        if length == request[2] + request[3]:
            return True
        return False


    def generateValidRequestResponse(self, request):
        # Resolve IP Addresses
        # Return a message with the following pieces in order:
        #    1.Header
        #        (magic number, total message length, checksum, group ID, requestID)
        #            Checksum here is calculated on the response datagram 
        #    2.IP Addresses of all hostnames, one after another
        #        In same order as hostnames
        ipAddresses = self.getIPAddresses(request)
        ipBytes = bytearray(ipAddresses, "utf-8")
        returnData = bytearray()
        returnData[:1] = 0x1234
        lengthMsg = 7 + len(ipBytes)
        returnData[2:3] = bytes(lengthMsg)
        returnData[5] = bytes(9)
        returnData[6] = 0x01
        returnData[7:lengthMsg] = ipBytes
        returnData[4] = self.calculateCheckSum(returnData)


    def getIPAddresses(self, request):
        firstLen = request[7]
        x = 7
        ipAddresses = list()
        for x in range(7, len(request), firstLen):
            hostname = str(request[x:firstLen])
            addr = socket.gethostbyname(hostname)
            ipAddresses.append(addr)
            x = firstLen + x + 1
            firstLen = request[x - 1]
        return ipAddresses


    def calculateCheckSum(self, request):
        """ Compute the Internet Checksum of the supplied data.  The checksum is
    initialized to zero.  Place the return value in the checksum field of a
    packet.  When the packet is received, check the checksum, by passing
    in the checksum field of the packet and the data.  If the result is zero,
    then the checksum has not detected an error.
    """
        sum = 0
        for i in range(0,len(request), 2):
            sum += int(request[i:i+2], 16) & 0xff
            sum = (sum & 0xff) + (sum >> 8)
        # one's complement the result
        sum = ~sum
        return sum & 0xff

    def generateInvalidRequestResponse(self, request):
        # Return a message with the following pieces in order:
        #    1.Header
        #        (magic number total message length, checksum, group ID)
        #            Checksum here is calculated on the response datagram 
        #    2.Error Code BEC
        #        calculated as below:
        #             bad length: b0 = 1
        #            bad checksum: b1 = 1
        #            bad magic number: b2 = 1
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

