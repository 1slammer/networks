import sys

def main(portNum):
	print portNum
	pass


if __name__ == '__main__':
	if len(sys.argv) != 2:
		print "Proper usage:\n\tUDPServer.py port#"
		sys.exit()
	portNum = sys.argv[1]
	main(portNum)
