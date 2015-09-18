


public class Strings
{

  final static char[] digits =
  {
          '0' , '1' , '2' , '3' , '4' , '5' ,
          '6' , '7' , '8' , '9' , 'a' , 'b' ,
          'c' , 'd' , 'e' , 'f' , 'g' , 'h' ,
          'i' , 'j' , 'k' , 'l' , 'm' , 'n' ,
          'o' , 'p' , 'q' , 'r' , 's' , 't' ,
          'u' , 'v' , 'w' , 'x' , 'y' , 'z'
 };

  public static void main(String[] args) throws Exception
  {
    // String message = "Hello";
    // char[] message2 = {'H', 'e', 'l', 'l', 'o'};

    // System.out.println("\n" + message.getBytes()  +"\t" + message.length());
    //
    // byte[] mess = message.getBytes();
    // byte[] test = new byte[message.length()];
    // for(byte a : mess)
    // {
    //   System.out.println(a);
    // }
    // for(byte a : mess)
    // {
    //     System.out.println(Integer.toHexString(a));
    // }
    //
    // for (int i = 0; i < message2.length; i++)
    // {
    //   System.out.print(message2[i] + " ");
    // }
    // System.out.println("\n" + message2.length);

    // byte[] temp = new byte[252];
    // temp[0] = 34;
    // temp[1] = 2;
    // temp[2] = 5;
    // int i = 3;
    // char[] t = "ThisIsTest".toCharArray();
    // for(char x : t)
    // {
    //   temp[i] = (byte)x;
    //   i++;
    //   System.out.print(x);
    // }
    //
    // System.out.println();
    // int j = 0;
    // while(temp[j] != 0)
    // {
    //   System.out.print(temp[j] + " ");
    //   j++;
    // }
    // System.out.println("\n\n");
    //

    // String message = "This is my message";
    // char[] message1 = message.toCharArray();
    // for(int i = 0; i < message1.length; i++)
    // {
    //   System.out.print(message1[i]);
    // }
    // String message3 = "";
    // System.out.println("\n\n");
    // for(int i = 0; i < message1.length; i++)
    // {
    //   message3 += Integer.toHexString(message1[i]);
    //   // System.out.print(message3 + " ");
    // }
    // System.out.print("");
    //
    // byte[] messages = message3.getBytes("UTF-8");
    // String message4 = new String(messages, "UTF-8");
    //
    // for(int i = 0; i < message4.length(); i++)
    // {
    //   System.out.print(message4.charAt(i) + " ");
    // }

    char[] temp = "ABCDEFG".toCharArray();
    byte[] temp2 = new byte[temp.length];

    for(int i = 0; i < temp.length; i++)
    {
      temp2[i] = byteToByte((byte)temp[i]);
    }
    System.out.println("Temp2 Bytes: ");
    for(byte i : temp2)
    {
      System.out.println(i);
    }

    System.out.println("\n");
    // byte[] temp3 = new byte[temp.length];
    // for(int i = 0; i < temp3.length; i++)
    // {
    //   temp3[i] = byteToByte(temp2[i]);
    // }
    //
    // for(byte i : temp3)
    // {
    //   System.out.println(i);
    // }

//     int check = Integer.parseInt(Integer.toHexString((int)temp2[0]));
//     System.out.println("check: " + check);
//
// //     String temp3 = "";
// //     for(int i = 0; i < temp.length; i++)
// //     {
// //       temp3 += Integer.toHexString((int)temp2[i]);
// //     }
//
//     byte[] news = new byte[temp.length];
//     // for(int i = 0; i < temp.length; i++)
//     // {
//     //   news[i] = intToByte(Integer.parseInt(Integer.toHexString(temp2[i])));
//     // }
//
//     //more readable than the immediate for-loop
//     for(int i = 0; i < temp.length; i++)
//     {
//       String s = Integer.toHexString(temp2[i]);
//       int hex = Integer.parseInt(s);
//       news[i] = intToByte(hex);
//     }
//
//     for(byte x : news)
//     {
//       System.out.println(x);
//     }

//     System.out.println("Temp3 bytes: " + temp3);
//
//     for(int i = 0; i < temp3.length(); i++)
//     {
//       System.out.println(temp3.charAt(i));
//     }

    // char[] temp4 = temp3.toCharArray();
    //
    // for(char x : temp4)
    // {
    //   System.out.println(x);
    // }
    //
    // String test = new String(temp2, "UTF-8");
    // byte[] b = new byte[test.length()];
    // for(int i = 0; i < test.length(); i++)
    // {
    //   b[i] = (byte)test.charAt(i);
    // }
    //
    // System.out.println("B[]: ");
    // for(byte x: b)
    // {
    //   System.out.println(x);
    // }

    // System.out.println("\nTemp4 char[]: ");
    // System.out.print(temp4);
    // byte[] temp5 = new byte[temp4.length];
    // for(char i = 0; i < temp4.length; i++)
    // {
    //   System.out.println(temp4[i]);
    //   System.out.println((byte)temp4[i]);
    //   temp5[i] = byteToByte((byte)temp4[i]);
    // }
    //
    // System.out.println("\n\nFinal print of temp5:");
    // for(byte i : temp5)
    // {
    //   System.out.println(i);
    // }

    // char[] what = toUnsignedCharArray(65, 4);
    // System.out.println(what);
    // int t  = 0;
    // for(char x : what)
    // {
    //   System.out.print(t + ": ");
    //   System.out.println(x);
    //   t++;
    // }
    //
    // System.out.println((char)what[30]);
    // System.out.println((char)what[31]);

//     String message  = "This is my message";
//     byte[] data = new byte[message.length()];
//
//     for(int i = 0; i < data.length; i++)
//     {
//       char character = message.charAt(i);
//       int characterDecNumber = (int)character;
//       String characterStringHexNumber = Integer.toHexString(characterDecNumber);
//       int characterHexNumber = Integer.parseInt(characterStringHexNumber.toString());
//       data[i] = intToByte(characterHexNumber);
//     }
//
//     for(byte x : data)
//     {
//       System.out.println(x);
//     }


    char[] message = "ABCDEFGHIJKLMNOPQRST".toCharArray();
    byte[] data = packetSetUp(message);

    for(byte x : data)
    {
      System.out.println(x);
    }

  }//end main





  private static byte intToByte(int i)
  {
    byte x = (byte)(i & 0xFF);
    return x;
  }

  /* Java sets signed bytes, lets make them unsigned    */
  private static byte byteToByte(byte i)
  {
    byte x = (byte)(i & 0xFF);
    return x;
  }


  private static char[] toUnsignedCharArray(int i, int shift)
  {
         char[] buf = new char[32];
         int charPos = 32;
         int radix = 1 << shift;
         int mask = radix - 1;
         do
         {
             buf[--charPos] = digits[i & mask];
             i >>>= shift;
         } while (i != 0);

         return buf;

  }

  private static byte[] packetSetUp(char[] message)
  {
    byte[] ret = new byte[message.length + 3];
    byte[] buffer = new byte[message.length];

    //Ensure the message elements are byte size and convert them to bytes
    for(int i = 0; i < message.length; i++)
    {
      buffer[i] = byteToByte((byte)message[i]);
    }

    //Set what we know
    ret[0] = intToByte(message.length + 3);
    ret[1] = intToByte(5);
    ret[2] = intToByte(80);

    //Copy over the message
    int j = 0;
    for(int i = 3; i < ret.length; i++)
    {
      ret[i] = buffer[j];
      j++;
    }

    //make all values hex
    for(int i = 0; i < ret.length; i++)
    {
      String hexString = Integer.toHexString(ret[i]);
      int hexNumber = Integer.parseInt(hexString, 16);
      ret[i] = byteToByte(ret[i]);
    }

    return ret;

   }

//     private static byte stringToByte(String s)
//     {
//       byte x = (byte) (s & 0xFF);
//       return x;
//     }

}
