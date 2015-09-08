import java.io.*; 
import java.net.*;
import java.util.*; 


class UDPServer {

public static void main(String args[]) throws Exception  { 
  if (args.length > 0) {
     DatagramSocket serverSocket = new DatagramSocket(Integer.parseInt(args[0]);
         }
  else {
      DatagramSocket serverSocket = new DatagramSocket(10019);
  }                     
  byte[] receiveData = new byte[256];                          
  while(true) {                   
      DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
      serverSocket.receive(receivePacket);

      byte[] data = receivePacket.getData();
      int length = data[0];
      byte request = data[1];
      int operation = data[2];

      switch (operation) {
        case 5: 
          int numConsonants = 0;
          for(int x = 3; x < data.length; x++) {
            char ch = data[x];
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
          InetAddress IPAddress = receivePacket.getAddress();                   
          int port = receivePacket.getPort();

          byte[] sendData = new byte[3];
          sendData[0] = 0x03;
          sendData[1] = request;
          sendData[2] = (byte) numConsonants;                   
          DatagramPacket sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
          serverSocket.send(sendPacket);                
          break;
        case 80:
          StringBuilder myBuilder = new StringBuilder();
          for(int x = 3; x < data.length; x++) {
            char ch = data[x];
            
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
          InetAddress IPAddress = receivePacket.getAddress();                   
          int port = receivePacket.getPort();

          byte[] sendData = new byte[newBytes.length + 2];
          sendData[0] = 0x50;
          sendData[1] = request;
          for(int x = 0; x < sendData.length; x++){
            sendData[x + 2] = newBytes[x];
          }                   
          DatagramPacket sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
          serverSocket.send(sendPacket);                
          break;
        case 10:
          String sentence = new String( receivePacket.getData());                                     
          InetAddress IPAddress = receivePacket.getAddress();                   
          int port = receivePacket.getPort();                   
          String capitalizedSentence = sentence.toUpperCase();
          byte[] stringData = capitalizedSentence.getBytes();                   
          byte[] sendData = new byte[stringData.length + 2];
          sendData[0] = 0x0A;
          sendData[1] = request;
          for(int x = 0; x < sendData.length; x++){
            sendData[x + 2] = stringData[x];
          }                                
          DatagramPacket sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
          serverSocket.send(sendPacket);                
      }                   
      }       
    } 
  } 