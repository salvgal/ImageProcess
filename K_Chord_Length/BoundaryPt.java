package mypack;

public class BoundaryPt {

	public int X;
	public int Y;
	public int maxVotes;
	public double maxDistance;
	public int corner;
	public boolean isLocalMaxima;

	
	public BoundaryPt() {
		maxVotes = 0;
		maxDistance = 0.0;
		corner = 1;
		isLocalMaxima = false;
	}

	public void loadXY(int x, int y) {
		X = x;
		Y = y;
	}
}
