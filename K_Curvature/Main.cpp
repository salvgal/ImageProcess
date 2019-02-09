#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;


class BoundaryPt {
	friend class kCurvature;
public:
	int X;
	int Y;
	double curvature;
	int corner;
	int localMax;
	bool isLocalMaxima;

	BoundaryPt() {
		curvature = 0.0;
		corner = 0;
		localMax = 0;
		isLocalMaxima = false;
	}

	void loadXY(int x, int y) {
		X = x;
		Y = y;
	}
};

class Image {
private:
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int** imgAry;
	int label;

public:

	Image(int r, int c, int min, int max, int l)
		: numRows(r), numCols(c), minVal(min), maxVal(max), label(l) {
		imgAry = new int*[numRows];
		for (int i = 0; i<numRows; i++) imgAry[i] = new int[numCols];
	}

	// put each point (x, y)’s corner value (1 or 8) at Img(x, y)
	void plotPt2Img(BoundaryPt* boundAry, int numPts) {
		int r, c;
		for (int i = 0; i < numPts; i++) {
			r = boundAry[i].X;
			c = boundAry[i].Y;
			imgAry[r][c] = boundAry[i].corner;
		}
	}

	void prettyPrint(ofstream &outFile) {
		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j < numCols; j++)
				if (imgAry[i][j] > 0) outFile << imgAry[i][j] << " ";
				else outFile << "  ";
				outFile << endl;
		}
	}
};

class kCurvature {

	int K; // ask user form console
	int numPts;       // get from input
	int Q;          // initially set to 0
	int P;          // initially set to K
	int R;          //initially set to 2*K

public:
	BoundaryPt * PtAry;

	kCurvature(int k, int pts) : K(k), numPts(pts) {
		PtAry = new BoundaryPt[numPts];
		//for (int i = 0; i < k; i++)
		//	PtAry[i].curvature = computeCurvature(95, 0, 1);
	}

	void storePt(int x, int y, int index) {
		PtAry[index].loadXY(x, y);
	}


	bool computeLocalMaxima(int index) {
		if (PtAry[index].curvature >= PtAry[(index + numPts - 1) % numPts].curvature &&
			PtAry[index].curvature >= PtAry[(index + numPts - 2) % numPts].curvature &&
			PtAry[index].curvature >= PtAry[(index + numPts + 1) % numPts].curvature &&
			PtAry[index].curvature >= PtAry[(index + numPts + 2) % numPts].curvature)
			return true;
		else
			return false;
	}

	int setCorner(int index) {
		if (PtAry[index].isLocalMaxima &&
			!PtAry[(index + numPts - 2) % numPts].isLocalMaxima &&
			!PtAry[(index + numPts + 2) % numPts].isLocalMaxima)
			return 8;
		else
			return 1;
	}


	double computeCurvature(int Q, int P, int R) {
		double r1 = PtAry[Q].X;
		double c1 = PtAry[Q].Y;
		double r2 = PtAry[P].X;
		double c2 = PtAry[P].Y;
		double r3 = PtAry[R].X;
		double c3 = PtAry[R].Y;
		double up = (c1 - c2) / (r1 - r2 + 0.001);
		double down = (c2 - c3) / (r2 - r3 + 0.001);

		if (up < down) {
			cout << down - up << endl;
			return down - up;
		}
		else {
			cout << up - down << endl;
			return up - down;
		}
	}


	void doCurvature(ofstream& outFile3) {

		//step 4:
		Q = 0;
		P = K;
		R = 2 * K;

		for (int i = 0; i < numPts; i++) {
			//Step 5
			int index = P;
			double curv = computeCurvature(Q, P, R);
			PtAry[index].curvature = curv;
			outFile3 << "Q: " << Q << " P: " << P << " R: " << R << " index: " << index << " Curvature: " << curv << endl;
			//Step 6
			Q = (Q + 1) % numPts;
			P = (P + 1) % numPts;
			R = (R + 1) % numPts;
		} // step 7 : repeat step 5 to step 6 until P == K-1

		  // step 8 :print the info (x, y, curvature) of the entire PtAry to argv[4] 
		for (int i = 0; i < numPts; i++) {
			outFile3 << "( " << PtAry[i].X << ", " << PtAry[i].Y << ", " << PtAry[i].curvature << " )" << endl;
		}

		// step 9: computeLocalMaxima (PtAry) for all point in PtAry[index], index from 0 to numPts-1
		for (int i = 0; i < numPts; i++) {
			PtAry[i].isLocalMaxima = computeLocalMaxima(i % numPts);
		}

		//step 10: setCorner(PtAry) do for all point in boundPtAry[index], index from 0 to numPts - 1
		for (int i = 0; i < numPts; i++) {
			PtAry[i].corner = setCorner(i);
		}
	}

	void printFile1(ofstream& outFile1) {
		//step 11: output only (x, y, corner) of the entire PtAry to argv[2]
		for (int i = 0; i < numPts; i++) {
			outFile1 << PtAry[i].X << " " << PtAry[i].Y << " " << PtAry[i].corner << endl;
		}
	}
};

int main(int argc, const char * argv[]) {
	// step 0: open all files
	ifstream inFile(argv[1]);
	ofstream outFile1(argv[2]);
	ofstream outFile2(argv[3]);
	ofstream outFile3(argv[4]);


	// ask user for KchordLength
	int k;
	cout << "enter the value for K :";
	cin >> k;

	//read numPts
	int n = 0;
	int ignore = 0;
	inFile >> ignore; 	inFile >> ignore; 	inFile >> ignore; 	inFile >> ignore; 	inFile >> ignore;
	while (!inFile.eof()) {
		inFile >> ignore;
		inFile >> ignore;
		n++;
	}
	inFile.clear();
	inFile.seekg(0, ios::beg);

	// read from argv[2] for numRows, numCols, minVal, maxVal, numPts
	int r, c, min, max, label, pts;
	pts = n - 1;
	inFile >> r >> c >> min >> max >> label;


	Image img(r, c, min, max, label);
	kCurvature kCurv(k, pts);
	int index = 0;

	while (index < pts) {
		// step 1: (x, y) <-- read from input
		int x, y;
		inFile >> x >> y;
		kCurv.storePt(x, y, index);
		// step 2:
		index++;
	} // step 3 : repeat step 1 - 2 until the file is empty

	  // step 4 - 10
	kCurv.doCurvature(outFile3);

	// step 11
	outFile1 << r << " " << c << " " << min << " " << max << endl;
	outFile1 << label << endl;
	outFile1 << pts << endl;
	kCurv.printFile1(outFile1);

	// step 12 -13
	img.plotPt2Img(kCurv.PtAry, pts);

	// step 14: prettyPrint imgAry to argv[4]
	img.prettyPrint(outFile2);

	// step 15: close all files
	inFile.close();
	outFile1.close();
	outFile2.close();
	outFile3.close();
	system("PAUSE");
	return 0;
}