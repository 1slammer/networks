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

          sendData = new byte[3];
          sendData[0] = 0x03;
          sendData[1] = request;
          sendData[2] = (byte) numConsonants;                   
          //sendData[3] = 0x05;
          //sendData[4] = 0x09;
          //sendData[5] = 0x08;                   
          System.out.println("numConsonants" + numConsonants);
          sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
          //System.out.println(sendData[0]);
          //System.out.println(sendData[1]);
          //System.out.println(sendData[2]);
          //System.out.println(sendData[3]);
          //System.out.println(sendData[4]);
          //System.out.println(sendData[5]);
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
          for(int x = 0; x < sendData.length-2; x++){
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
          for(int x = 0; x < sendData.length-2; x++){
            sendData[x + 2] = stringData[x];
          }                                
          sendPacket =  new DatagramPacket(sendData, sendData.length, IPAddress, port);                   
          serverSocket.send(sendPacket);                
      }                   
      }       
    } 
  } 
