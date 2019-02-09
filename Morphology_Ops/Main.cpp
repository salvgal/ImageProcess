#include <iostream>
#include <fstream>

using namespace std;

class Morphology {
public:
	int numRowsImg;
	int numColsImg;
	int minImg;
	int maxImg;

	int numRowsStructElem;
	int numColsStructElem;
	int minStrctElem;
	int maxStrctElem;

	int rowOrigin;
	int colOrigin;
	int rowFrameSize;
	int colFrameSize;

	int** imgAry;
	int** morphAry;
	int** morphAry2; //NO NEEDED
	int** structElemAry;

	Morphology(int row1, int col1, int min1, int max1, int row2, int col2, int min2, int max2) {
		numRowsImg = row1;
		numColsImg = col1;
		minImg = min1;
		maxImg = max2;
		numRowsStructElem = row2;
		numColsStructElem = col2;
		minStrctElem = min2;
		maxStrctElem = max2;

		computeFrameSize();

		imgAry = new int*[numRowsImg + rowFrameSize];
		morphAry = new int*[numRowsImg + rowFrameSize];
		morphAry2 = new int*[numRowsImg + rowFrameSize];
		for (int i = 0; i < numRowsImg + rowFrameSize; i++) {
			imgAry[i] = new int[numColsImg + colFrameSize];
			morphAry[i] = new int[numColsImg + colFrameSize];
			morphAry2[i] = new int[numColsImg + colFrameSize];
		}

		structElemAry = new int*[numRowsStructElem];
		for (int i = 0; i < numRowsStructElem; i++) {
			structElemAry[i] = new int[numColsStructElem];
		}
	}

	void computeFrameSize() {
		rowFrameSize = 2 * numRowsStructElem;
		colFrameSize = 2 * numColsStructElem;
	}

	void loadImage(ifstream& input) {
		for (int i = numRowsStructElem; i < numRowsImg + numRowsStructElem; i++)
			for (int j = numColsStructElem; j < numColsImg + numColsStructElem; j++)
				input >> imgAry[i][j];
	}

	void loadStruct(ifstream& input) {
		input >> rowOrigin >> colOrigin;
		for (int i = 0; i < numRowsStructElem; i++)
			for (int j = 0; j < numColsStructElem; j++)
				input >> structElemAry[i][j];
	}

	void zeroFrameImage() {
		for (int i = 0; i < numRowsImg + rowFrameSize; i++)
			for (int j = 0; j < numColsImg + colFrameSize; j++) {
				imgAry[i][j] = 0;
				morphAry[i][j] = 0;
				morphAry2[i][j] = 0;
			}
	}

	void dilation(int** img, int** morph) { //using img and write the result to morph
		for (int r = numRowsStructElem; r < numRowsImg + numRowsStructElem; r++)
			for (int c = numColsStructElem; c < numColsImg + numColsStructElem; c++)
				if (img[r][c] > 0)
					for (int i = 0; i < numRowsStructElem; i++)
						for (int j = 0; j < numColsStructElem; j++)
							if (structElemAry[i][j] > 0)
								morph[i + r - rowOrigin][j + c - colOrigin] = structElemAry[i][j];
	}

	bool isMatch(int r, int c, int** img) {
		for (int i = 0; i < numRowsStructElem; i++)
			for (int j = 0; j < numColsStructElem; j++)
				if (structElemAry[i][j] > 0 && img[r - rowOrigin + i][c - colOrigin + j] != structElemAry[i][j])
					return false;
		return true;
	}

	void erosion(int** img, int** morph) {  //using img and write the result to morph
		for (int r = numRowsStructElem; r < numRowsImg + numRowsStructElem; r++)
			for (int c = numColsStructElem; c < numColsImg + numColsStructElem; c++)
				if (img[r][c] > 0 && isMatch(r, c, img)) morph[r][c] = 1;
	}

	void closing() {
		dilation(imgAry, morphAry2);
		erosion(morphAry2, morphAry);
	}

	void opening() {
		erosion(imgAry, morphAry2);
		dilation(morphAry2, morphAry);
	}

	void prettyPrint(int** ary, int rowStart, int colStart, int rowEnd, int colEnd) {
		for (int i = rowStart; i < rowEnd; i++) {
			for (int j = colStart; j < colEnd; j++)
				if (ary[i][j] > 0) cout << ary[i][j] << " ";
				else cout << "  ";

				cout << endl;
		}

	}

	void outputResult(ofstream& outFile) {
		outFile << numRowsImg << " " << numColsImg << " " << minImg << " " << maxImg << endl;
		for (int i = numRowsStructElem; i < numRowsImg + numRowsStructElem; i++) {
			for (int j = numColsStructElem; j < numColsImg + numColsStructElem; j++)
				outFile << morphAry[i][j] << " ";
			outFile << endl;
		}
	}

	void initializeToZero() {
		for (int i = 0; i < numRowsImg + rowFrameSize; i++)
			for (int j = 0; j < numColsImg + colFrameSize; j++) {
				morphAry[i][j] = 0;
				morphAry2[i][j] = 0;
			}
	}

};

int main(int argc, const char * argv[]) {

	// step 0
	ifstream inFile1(argv[1]);
	ifstream inFile2(argv[2]);
	ofstream outFile1(argv[3]);
	ofstream outFile2(argv[4]);
	ofstream outFile3(argv[5]);
	ofstream outFile4(argv[6]);

	int r1, c1, min1, max1;
	int r2, c2, min2, max2;
	inFile1 >> r1 >> c1 >> min1 >> max1;
	inFile2 >> r2 >> c2 >> min2 >> max2;

	// step 1 - 3
	Morphology morphology(r1, c1, min1, max1, r2, c2, min2, max2);
	morphology.zeroFrameImage();
	morphology.loadImage(inFile1);
	cout << "Input Image:\n" << endl;
	morphology.prettyPrint(morphology.imgAry, r2, c2, r1 + r2, c1 + c2);
	morphology.loadStruct(inFile2);
	cout << "\nStructuring Element:\n" << endl;
	morphology.prettyPrint(morphology.structElemAry, 0, 0, r2, c2);

	// step 4: call dilation
	morphology.initializeToZero();
	morphology.dilation(morphology.imgAry, morphology.morphAry);
	cout << "\nAfter Dilation:\n" << endl;
	morphology.prettyPrint(morphology.morphAry, r2, c2, r1 + r2, c1 + c2);
	morphology.outputResult(outFile1);

	// step 5: call erosion
	morphology.initializeToZero();
	morphology.erosion(morphology.imgAry, morphology.morphAry);
	cout << "\nAfter Erosion:\n" << endl;
	morphology.prettyPrint(morphology.morphAry, r2, c2, r1 + r2, c1 + c2);
	morphology.outputResult(outFile2);


	// step 6: call closing
	morphology.initializeToZero();
	morphology.closing();
	cout << "\nAfter Closing:\n" << endl;
	morphology.prettyPrint(morphology.morphAry, r2, c2, r1 + r2, c1 + c2);
	morphology.outputResult(outFile3);

	// step 7: call opening
	morphology.initializeToZero();
	morphology.opening();
	cout << "\nAfter Opening:\n" << endl;
	morphology.prettyPrint(morphology.morphAry, r2, c2, r1 + r2, c1 + c2);
	morphology.outputResult(outFile4);

	// step 8: close all files
	inFile1.close();
	inFile2.close();
	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();

	return 0;
}