package mypack;

public class ArcChord {

	private int KchordLength; // ask user form console
	private int numPts;       // get from input-1
	private double [] chordAry;
	private int P1;          // the array index of the first-end of the arc Chord;
	private int P2;          // the array index of the second-end of the arc Chord;
	private int currPt;

	public BoundaryPt[] ptAry;

	public ArcChord(int k, int pts){
		this.KchordLength=k;
		this.numPts=pts;		
		ptAry = new BoundaryPt[numPts];
		chordAry = new double[k];
	}

	void loadData(int x, int y, int index) {
		ptAry[index] = new BoundaryPt();
		ptAry[index].loadXY(x, y);
	}

	double computeDistance(int P1, int P2, int currPt) {
		double A = ptAry[P2].Y - ptAry[P1].Y;
		double B = ptAry[P1].X - ptAry[P2].X;
		double C = (ptAry[P2].X * ptAry[P1].Y) - (ptAry[P1].X * ptAry[P2].Y);
		double dist = Math.abs(((A * ptAry[currPt].X) + (B * ptAry[currPt].Y) + C)) / Math.sqrt((A * A) + (B * B));
		return dist;
	}

	int findMaxDist() {
		int index = 0;
		double max = chordAry[0];
		for (int i = 1; i < KchordLength; i++)
			if (chordAry[i] > max) {
				max = chordAry[i];
				index = i;
			}
		return index;

	}

	public boolean computeLocalMaxima(int index) {
		for (int i = index - 3; i <= index + 3; i++) {
			if (ptAry[(i + numPts) % numPts].maxVotes > ptAry[index].maxVotes)
				return false;
		}
		return true;
	}

	public int setCorner(int index) {
		if (ptAry[index].isLocalMaxima
			&& ptAry[(index + numPts - 1) % numPts].maxDistance < ptAry[index].maxDistance
			&& ptAry[(index + 1) % numPts].maxDistance < ptAry[index].maxDistance)
			return 9;
		return 1;
	}

	public String doArcChordDistance() {
		
		String ret = "";
		
		//step 1
		P1 = 0;
		P2 = KchordLength - 1;

		while (P2 != KchordLength / 2) {
			// step 2
			int index = 0;
			currPt = P1;

			while (index < KchordLength) {
				// step 3
				double dist = computeDistance(P1,P2,currPt);
				chordAry[index] = dist;
				index++;
				currPt = (currPt + 1) % numPts;
			} // step 4

			  // step 5
			ret += "P1: " + P1 + " P2: " + P2 + " currPt: " + currPt;
			ret += "\n";
			ret += "chordAry: ";
			for (int i = 0; i < KchordLength; i++)
				ret += chordAry[i] + " ";
			ret += "\n";

			//step 6
			int maxIndex = findMaxDist();
			int whichIndex = (P1 + maxIndex) % numPts;
			ptAry[whichIndex].maxVotes++;
			if (ptAry[whichIndex].maxDistance < chordAry[maxIndex]) {
				ptAry[whichIndex].maxDistance = chordAry[maxIndex];
			}

			// step 7
			for (int i = P1; i <= P2; i++){
				ret += "ptAry[" + i + "]: X = " + ptAry[i].X + " Y = " + ptAry[i].Y;
				ret += "\n";
			}

			// step 8
			P1 = (P1 + 1) % numPts;
			P2 = (P2 + 1) % numPts;

		}// step 9

		 // step 10
		ret += "\n=================================================================\n";
		ret += "ptAry:";
		ret += "\n";
		for (int i = 0; i < numPts; i++) {
			ret += i + ":x=" + ptAry[i].X + ",y=" + ptAry[i].Y + ",maxVotes=" + ptAry[i].maxVotes + ",maxDistance=" + ptAry[i].maxDistance + "\t";
			if ((i + 1) % 5 == 0) ret += "\n";
		}

		// step 11
		for (int i = 0; i < numPts; i++)
			ptAry[i].isLocalMaxima = computeLocalMaxima(i);

		// step 12
		for (int i = 0; i < numPts; i++)
			ptAry[i].corner = setCorner(i);
		
		ret += "\n=================================================================\n";
		// step 13
		for (int i = 0; i < numPts; i++){
			ret +=  "X:" + ptAry[i].X + " Y:" + ptAry[i].Y + " Corner:" + ptAry[i].corner;
			ret += "\n";
		}
		
		return ret;
	}

}
