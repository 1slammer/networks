import java.util.*;

public class Partition {
  
  public static void main(String[] args) {
    Scanner scan = new Scanner(System.in);
    int width = scan.nextInt();
    int numDiv = scan.nextInt();
    int[] list = new int[numDiv + 1];
    list[list.length -1] = width;
    for(int i = 0; i < numDiv; ++i) {
      list[i] = scan.nextInt();
    }
    for(int i = 0; i < numDiv; ++i) {
      list[i] = scan.nextInt();
    }
  }

}
