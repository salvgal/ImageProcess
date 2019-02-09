package mypack;

public class Image{
	
	private int numRows;
	private int numCols;
	private int minVal;
	private int maxVal;
	private int[][] img;
	private int label;

	
    public Image(int rows, int cols, int min, int max, int l) {
		this.numRows = rows;
		this.numCols = cols;
		this.minVal = min;
		this.maxVal = max;
		this.label=l;
		this.img = new int[numRows][numCols];
    }
	
	public void plotPt2Img(BoundaryPt[] boundAry, int numPts) {
		int r, c;
		for (int i = 0; i < numPts; i++) {
			r = boundAry[i].X;
			c = boundAry[i].Y;
			img[r][c] = boundAry[i].corner;
		}
	}
	public String prettyPrint() {
		String ret="";
		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j < numCols; j++){
				if (img[i][j] > 0)
					ret += img[i][j] + " ";
				else 
					ret += "  ";
			}
			ret += "\n";
		}
		return ret;
	}
	
	public String resultFile (BoundaryPt[] boundAry, int numPts) {
		String ret="";
		ret+= numRows + " " + numCols +" " + minVal + " "+  maxVal;
		ret += "\n";
		ret += label;
		ret += "\n";
		ret += numPts;
		ret += "\n";
		for (int i = 0; i < numPts; i++){
			ret += boundAry[i].X + " " + boundAry[i].Y + " " + boundAry[i].corner;
			ret += "\n";
		}
		return ret;
	}	
}

