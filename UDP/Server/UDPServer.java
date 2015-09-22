import java.io.*; 
import java.net.*;
import java.util.*; 


class UDPServer {

public static void main(String args[]) throws Exception  { 
  DatagramSocket serverSocket;
  if (args.length > 0) {
     serverSocket = new DatagramSocket(Integer.parseInt(args[0]));
         }
  else {
      serverSocket = new DatagramSocket(10019);
  }                     
  byte[] receiveData = new byte[256];                          
  while(true) {                   
      DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
      serverSocket.receive(receivePacket);

      byte[] data = receivePacket.getData();
      int length = data[0];
      byte request = data[1];
      int operation = data[2];
      InetAddress IPAddress = receivePacket.getAddress();
      int port = receivePacket.getPort();
      byte[] sendData;
      DatagramPacket sendPacket;

      //for (int i=0; i < data.length; i++)
      int i = 0;
      while (data[i] != 0)
      {
          System.out.println(Integer.toHexString((int)data[i]));
          i++;
      }

      switch (operation) {
        case 5: 
            numStringConsonants(data, length, IPAddress, port, serverSocket, request);
            break;

        case 80:
          disenvowel(data, length, IPAddress, port, serverSocket, request);
          break;

        case 10:
          uppercasing(data, length, IPAddress, port, serverSocket, request);
      }                   
      }       
    } 
    public static void uppercasing(byte[] data, int length, InetAddress IPAddress,
            int port, DatagramSocket serverSocket, byte request) throws Exception {
      //String sentence = new String(data);                                     
      StringBuilder myBuilder = new StringBuilder("");
      for(int x = 3; x < length; x++) {
        char ch = (char) data[x];
        myBuilder.append(ch);
      }
      String capitalizedSentence = myBuilder.toString().toUpperCase();
      System.out.println("cap sentence size: " + capitalizedSentence.length());
      System.out.println("cap sentence size: " + length);
      byte[] stringData = capitalizedSentence.getBytes();                   
      byte[] sendData = new byte[capitalizedSentence.length() + 2];
      sendData[0] = (byte)(capitalizedSentence.length() + 2);
      sendData[1] = request;
      for(int x = 0; x < capitalizedSentence.length(); x++){
        sendData[x + 2] = stringData[x];
        System.out.println(capitalizedSentence.charAt(x));
      }                                
      DatagramPacket sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
      serverSocket.send(sendPacket);                
    }
    public static void numStringConsonants(byte[] data, int length, InetAddress IPAddress,
            int port, DatagramSocket serverSocket, byte request) throws Exception {
      int numConsonants = 0;
      System.out.println("Data length: " + data.length);
      System.out.println("Data length: " + length);
      for(int x = 3; x < length; x++) {
        char ch = (char) data[x];
        System.out.println("Data msg data char: " + ch);
        if (ch == 'a' || ch == 'e' || ch == 'i' || 
                                    ch == 'o' || ch == 'u')
                    {
                        //do nothing
                    }
                    else
                    { 
                        numConsonants++;
                    }
      }

      byte[] sendData = new byte[3];
      sendData[0] = 0x03;
      sendData[1] = request;
      sendData[2] = (byte) numConsonants;                   
      //sendData[3] = 0x05;
      //sendData[4] = 0x09;
      //sendData[5] = 0x08;                   
      System.out.println("numConsonants" + numConsonants);
      DatagramPacket sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
      serverSocket.send(sendPacket);                
    }

    public static void disenvowel(byte[] data, int length, InetAddress IPAddress, 
            int port, DatagramSocket serverSocket, byte request) throws Exception {
      StringBuilder myBuilder = new StringBuilder("");
      for(int x = 3; x < length; x++) {
        char ch = (char) data[x];
        
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            //do nothing
        } else { 
            myBuilder.append(ch);
        }
      }
      byte[] newBytes = myBuilder.toString().getBytes();

      byte[] sendData = new byte[myBuilder.length() + 2];
      sendData[0] = (byte)myBuilder.length() + 2;
      sendData[1] = request;
      for(int x = 0; x < sendData.length-2; x++){
        sendData[x + 2] = newBytes[x];
      }                   
      DatagramPacket sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
      serverSocket.send(sendPacket);                

    }
  } 

