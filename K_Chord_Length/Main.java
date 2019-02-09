package mypack;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;


public class Main {

	public static void main(String[] args) throws FileNotFoundException, IOException {

		PrintWriter output1 = new PrintWriter(args[1]); 
		PrintWriter output2 = new PrintWriter(args[2]); 
		PrintWriter output3 = new PrintWriter(args[3]); 
	    

	    // step 0
	    System.out.println( "enter the value for KChordLength :");
	    Scanner in = new Scanner(System.in);
	    int k = in.nextInt();
	    k = 2*k;

		Scanner input = new Scanner(new File(args[0]));
		Scanner input2 = new Scanner(new File(args[0]));

		int pts = 0;
		while(input.hasNextLine()){
			input.nextLine();
			pts++;
		}
		pts -= 3;
	    
	    Image img = new Image(input2.nextInt(), input2.nextInt(), input2.nextInt(), input2.nextInt(), input2.nextInt());
	    ArcChord acd = new ArcChord(k,pts);
	    int index = 0;
	    
	    while(index < pts){
	        int x, y;
	        x = input2.nextInt();
	        y = input2.nextInt();
	        acd.loadData(x, y, index);
	        index++;
	    }
	    
	    // step 1 - 13
	    output3.println(acd.doArcChordDistance());
	    // step 14 - 15
	    img.plotPt2Img(acd.ptAry,pts);
	    
	    // step 16
	    output2.println(img.prettyPrint());
	    output1.println(img.resultFile(acd.ptAry, pts));
	    
	    // step 17
	    in.close();
	    input.close();
	    input2.close();
	    output1.close();
	    output2.close();
	    output3.close();
	}

}
