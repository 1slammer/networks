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
  private static int operation;           //Operation request number
  private static String message;          //Message by the user
  private static int messageSize;         //size of the text message NOT the total message sent to server!!!


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
      operation = Integer.parseInt(args[3]);
    }
    catch(NumberFormatException e)
    {
      System.out.println("\n\nString does not contain a parsable integer\n");
      e.printStackTrace();
    }

    //Set the user's messsage
    message = args[4];

    //Now the communication
    try
    (   //Establish a connection with the server via socket
        Socket socket = new Socket(address, port);
    )
    {
      //Set up output stream for user(client)
      OutputStream stream = socket.getOutputStream();
      OutputStreamWriter osw = new OutputStreamWriter(stream);
      BufferedWriter output = new BufferedWriter(osw);


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
  private static String getMessage()
  {
    return message;
  }









}
