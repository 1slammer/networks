/*
  This is a TCP client design according to Dr. Biaz's specs
  @author: Alex Aguirre, Dillon Beck, Wayne Nolen
  Lab 01
  Created: 17 Sep 2015
  Modified:
*/

import java.io.*;
import java.net.*;


public class ClientTCP
{

  private static String clientName;       //Name of client machine or username
  private static int port;                //Port for the server
  private static InetAddress address;     //Address of the server
  private static byte operation;           //Operation request number
  private static char[] message;          //Message by the user
  private static int messageSize;         //size of the text message NOT the total message sent to server!!!
  private static byte requestID = 1;      //Request id for the client (allowed to be hardcoded as of 17 Sep 2015)


  /*
    Terminal use is as follows:
    java ClientTCP <clientName> <serverName> <portNumber> <operation> <message>
                        args[0]      args[1]      args[2]    args[3]    args[4]
  */
  public static void main(String[] args)
  {

    //User input error checking
    if (args.length != 5)
    {
      System.out.println("\n\nInput the correct usage: <clientName> <serverName> <portNumber> <operation> <message>\n");
      return;
    }
    //First part of setting the size of the data to be sent to the server. And checking input error
    messageSize = args[4].length();
    if(messageSize > 252)
    {
      System.out.println("\n\nYou message must be less than or equal to 252 characters");
      return;
    }

    //Set client's Name
    clientName = args[0];

    //Set the Server Inet Address
    try
    {
      address = InetAddress.getByName(args[1]);
    }
    catch(UnknownHostException e)
    {
      System.out.println("\n\nNo IP address for the host could be found, or if a scope_id was specified for a global IPv6 address\n");
      e.printStackTrace();
    }
    catch(SecurityException e)
    {
      System.out.println("\n\nA security manager exists and its checkConnect method doesn't allow the operation\n");
      e.printStackTrace();
    }

    //Set port number of server to client's field variable
    try
    {
      port = Integer.parseInt(args[2]);
    }
    catch(NumberFormatException e)
    {
      System.out.println("\n\nString does not contain a parsable integer\n");
      e.printStackTrace();
    }

    //Set operation value
    try
    {
      operation = (byte)Integer.parseInt(args[3]);
    }
    catch(NumberFormatException e)
    {
      System.out.println("\n\nString does not contain a parsable integer\n");
      e.printStackTrace();
    }

    //Set the size of the data to be sent to the server.
    byte dataSize = intToByte(messageSize + 3);

    //Set the size of the data to be sent to the Server
    byte[] data = new byte[dataSize];

    //Setup the first part of the data to send to the server
    data[0] = dataSize;
    data[1] = requestID;
    data[2] = byteToByte(operation);

    //Set the user's messsage
    char[] tempMsg = args[4].toCharArray();

    

    //Now the communication part
    try
    (   //Establish a connection with the server via socket
        Socket socket = new Socket(address, port);
    )
    {
      //Set up output stream for user(client)
      OutputStream stream = socket.getOutputStream();
      OutputStreamWriter osw = new OutputStreamWriter(stream);
      BufferedWriter output = new BufferedWriter(osw);

      //load the packet



    }
    catch(Exception e)
    {

    }



  }

  /*  Returns the client's Name */
  private static String getClientName()
  {
    return clientName;
  }

  /* Return the port number   */
  private static int getPortNumber()
  {
      return port;
  }

  /* Return the server's Inet address */
  private static InetAddress getServerAddress()
  {
    return address;
  }

  /* Return the user's requested operation  */
  private static int getOperation()
  {
      return operation;
  }

  /* Return the user's (client's) message */
  private static char[] getMessage()
  {
    return message;
  }

  /* returns an unsigned byte for our purposes of total message length  */
  private static byte intToByte(int i)
  {
    byte x = (byte)(i & 0xFF);
    return x;
  }

  /* Java sets signed bytes, lets make them unsigned    */
  private static byte byteToByte(byte i)
  {
    byte x = i & 0xFF;
    return x;
  }








}