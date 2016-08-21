/* Reading Spectral file for Color Chips
and search for the 1 wavelength that would best separate the 5 color chips
Wavelengths to be considered are from 400.02 to 799.86 nm (1197 readings)
By: Chi Thai (2/26/2016)
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

int main(int argc, char** argv)
{
	
	// Declare variables and arrays
	double w[1200];     // Input wavelength array
	double rs[5][1200];	// Input raw spectra array - 5 color chips & up to 1200 wavelengths

	double tempD;  // temporary computed distance between feature points
	double minD;	// current minimum distance found
	double maxD;	// current maximum distance found
	double maxMinD = 0.0; // current max of all the minD found
	int i_maxMinD = 1201;  // wavelength index corresponding to current maxMinD

	int i, j, k;                        // Loop indices
	int n_wav = 0;                    // Number of values read

//	string fileName = "ColorChips400-700.txt";
//	string inLine = " ";   // input line from file

//	cout << "Please enter a file name: ";
//	cin >> fileName;
	
	ifstream inFile("ColorChips400-700.txt", ios::in);

	if (!inFile) {
		cerr << "File could not be opened" << endl;
		exit(1);
	}
	
	// Read numbers into arrays
	i = 0;   // init array index to 0
	inFile >> w[i] >> rs[0][i] >> rs[1][i] >> rs[2][i] >> rs[3][i] >> rs[4][i++];
	
//	while ( inFile >> w[i] >> rs[0][i] >> rs[1][i] >> rs[2][i] >> rs[3][i] >> rs[4][i++] )
	while ( !inFile.eof() )
	{
		inFile >> w[i] >> rs[0][i] >> rs[1][i] >> rs[2][i] >> rs[3][i] >> rs[4][i++];
	}    // end of reading in file
	n_wav = i;

	// Close INPUT file
	inFile.close();

	// Search for max of minD
	for (i = 0; i < n_wav; i++) {  // use max index = 880 for wavelengths below 700 nm else use n_wav
		maxD = 0.0;
		minD = 4096.0;
		// to find minD & maxD for each wavelength by trying out all possible pairings of the color chips
		for (j = 0; j < 4; j++)
			for (k = j + 1; k < 5; k++) {
				tempD = abs(rs[j][i] - rs[k][i]);
				if (tempD < minD) minD = tempD;
				if (tempD > maxD) maxD = tempD;
			}  // end of nested j,k loops

		// finding the max of all the minD
		if (minD > maxMinD) {
			maxMinD = minD;
			i_maxMinD = i; // save corresponding wavelength index
			cout << "maxMinD = " << maxMinD << "\t i = " << i << endl;
		}   // end of if (minD > maxMinD

	}  // end of for i loop

	if (i_maxMinD != 1201) {
		cout << "Optimal wavelength is " << w[i_maxMinD] << endl;
		maxD = 0.0;
		minD = 4096.0;
		for (j = 0; j < 4; j++) // to verify obtained results
			for (k = j + 1; k < 5; k++) {
				tempD = abs(rs[j][i_maxMinD] - rs[k][i_maxMinD]);
				if (tempD < minD) minD = tempD;
				if (tempD > maxD) maxD = tempD;
			}  // end of nested j,k loops
		cout << "minD=" << minD << "\t\t maxD-minD=" << maxD - minD << endl;
	}  // end of if optimal wavelength found

	else
		cout << "No optimal wavelength was found \n\n";


} // end of main
