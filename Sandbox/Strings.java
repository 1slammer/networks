


public class Strings
{
  public static void main(String[] args) throws Exception
  {
    String message = "Hello";
    char[] message2 = {'H', 'e', 'l', 'l', 'o'};

    System.out.println("\n" + message.getBytes()  +"\t" + message.length());

    byte[] mess = message.getBytes();
    byte[] test = new byte[message.length()];
    for(byte a : mess)
    {
      System.out.println(a);
    }
    for(byte a : mess)
    {
        System.out.println(Integer.toHexString(a));
    }

    for (int i = 0; i < message2.length; i++)
    {
      System.out.print(message2[i] + " ");
    }
    System.out.println("\n" + message2.length);

  }

}
