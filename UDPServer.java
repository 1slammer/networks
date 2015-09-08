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

      switch (operation) {
        case 5: 
          int numConsonants = 0;
          for(int x = 3; x < data.length; x++) {
            char ch = (char) data[x];
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

          sendData = new byte[3];
          sendData[0] = 0x03;
          sendData[1] = request;
          sendData[2] = (byte) numConsonants;                   
          sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
          serverSocket.send(sendPacket);                
          break;
        case 80:
          StringBuilder myBuilder = new StringBuilder();
          for(int x = 3; x < data.length; x++) {
            char ch = (char) data[x];
            
            if (ch == 'a' || ch == 'e' || ch == 'i' || 
                                        ch == 'o' || ch == 'u')
                        {
                            //do nothing
                        }
                        else
                        { 
                            myBuilder.append(ch);
                        }
          }
          byte[] newBytes = myBuilder.toString().getBytes();

          sendData = new byte[newBytes.length + 2];
          sendData[0] = 0x50;
          sendData[1] = request;
          for(int x = 0; x < sendData.length; x++){
            sendData[x + 2] = newBytes[x];
          }                   
          sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
          serverSocket.send(sendPacket);                
          break;
        case 10:
          String sentence = new String( receivePacket.getData());                                     
          String capitalizedSentence = sentence.toUpperCase();
          byte[] stringData = capitalizedSentence.getBytes();                   
          sendData = new byte[stringData.length + 2];
          sendData[0] = 0x0A;
          sendData[1] = request;
          for(int x = 0; x < sendData.length; x++){
            sendData[x + 2] = stringData[x];
          }                                
          sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
          serverSocket.send(sendPacket);                
      }                   
      }       
    } 
  } 