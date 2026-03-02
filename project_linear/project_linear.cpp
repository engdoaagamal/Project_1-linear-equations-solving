/*
Doaa Gamal Abd Elall
Ahmed talat camel ali
Basma abd-elmenham mohamed
*/
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
void store_equation(int n_eq, string eq, int** equations, int* right_side) {
	size_t equalPos = eq.find('=');
	if (equalPos == string::npos) return;

	string l_side = eq.substr(0, equalPos);
	string r_side = eq.substr(equalPos + 1);

	// --- STEP 1: PARSE THE RIGHT SIDE ---
	size_t r_start = 0;
	size_t r_end = r_side.find_first_of("+-", 1);
	if (r_end == string::npos) r_end = r_side.length();

	while (r_start < r_side.length()) {
		string r_term = r_side.substr(r_start, r_end - r_start);
		if (r_term == "" || r_term == "+" || r_term == "-") {  }
		else {
			size_t xPos = r_term.find('x');
			if (xPos == string::npos) {
				
				right_side[n_eq] += atoi(r_term.c_str());
			}
			else {
				
				string coeffStr = r_term.substr(0, xPos);
				int coeff;
				if (coeffStr == "" || coeffStr == "+") coeff = 1;
				else if (coeffStr == "-") coeff = -1;
				else coeff = atoi(coeffStr.c_str());

				int varIdx = atoi(r_term.substr(xPos + 1).c_str()) - 1;
				equations[n_eq][varIdx] -= coeff;
			}
		}
		r_start = r_end;
		r_end = r_side.find_first_of("+-", r_start + 1);
		if (r_end == string::npos) r_end = r_side.length();
	}

	//  STEP 2: PARSE THE LEFT SIDE 
	size_t l_start = 0;
	size_t l_end = l_side.find_first_of("+-", 1);
	if (l_end == string::npos) l_end = l_side.length();

	while (l_start < l_side.length()) {
		string l_term = l_side.substr(l_start, l_end - l_start);
		if (l_term == "" || l_term == "+" || l_term == "-") { /* skip empty */ }
		else {
			size_t xPos = l_term.find('x');
			if (xPos == string::npos) {
				// Constant on Left: Subtract from right_side (moving to right)
				right_side[n_eq] -= atoi(l_term.c_str());
			}
			else {
				// Variable on Left: Add to matrix
				string coeffStr = l_term.substr(0, xPos);
				int coeff;
				if (coeffStr == "" || coeffStr == "+") coeff = 1;
				else if (coeffStr == "-") coeff = -1;
				else coeff = atoi(coeffStr.c_str());

				int varIdx = atoi(l_term.substr(xPos + 1).c_str()) - 1;
				equations[n_eq][varIdx] += coeff;
			}
		}
		l_start = l_end;
		l_end = l_side.find_first_of("+-", l_start + 1);
		if (l_end == string::npos) l_end = l_side.length();
	}
}

void print_equation(int* eq, int n, int rhs) {
	bool firstTerm = true;
	for (int j = 0; j < n; j++) {
		int coeff = eq[j];
		if (coeff == 0) continue;
		
			if (!firstTerm && coeff > 0) cout << "+";
		if (coeff == 1) cout << "x" << (j + 1);
		else if (coeff == -1) cout << "-x" << (j + 1);
		else cout << coeff << "x" << (j + 1);

		firstTerm = false;
	}
	cout << "=" << rhs << endl;
}
void print_All_equation(int** equations, int n, int *rhs) {
	for (int i = 0; i < n; i++) {
				bool firstTerm = true;
				for (int j = 0; j < n; j++) {
					int coeff = equations[i][j];

					if (coeff == 0) continue;  

					
					if (!firstTerm && coeff > 0) cout << "+";

					
					if (coeff == 1) cout << "x" << (j + 1)<<" ";
					else if (coeff == -1) cout << "-x" << (j + 1) << " ";
					else cout << coeff << "x" << (j + 1) << " ";

					firstTerm = false;
				}
				cout << "=" << rhs[i] << endl; 
			}

}

void print_column(int** equations, int n, const string& var_name) {
	int idx = stoi(var_name.substr(1)) - 1;
	for (int i = 0; i < n; i++) {
		cout << equations[i][idx] << " ";
	}
	cout << endl;
}


void print_coefficient_matrix(int** equations, int n) {
	cout << "\n--- Cramer's Coefficient Matrix ---\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << equations[i][j] << "\t";
		}
		cout << endl;
	}
}
void print_cramer_column(int** equations, int* rhs, int n, const string& var_name) {
	int col_idx = stoi(var_name.substr(1)) - 1;

	cout << "\n--- Cramer's Matrix with " << var_name << " column replaced by RHS ---\n";

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (j == col_idx) cout << rhs[i] << "\t"; // replace column with RHS
			else cout << equations[i][j] << "\t";
		}
		cout << endl;
	}
}

int main()
{
	
	int n;
	cout << "Please enter number of equations: ";
	cin >> n;

	int** equations = new int* [n];
	int* right_side = new int[n];

	for (int i = 0; i < n; i++) {
		equations[i] = new int[n] {0};
		right_side[i] = 0;
	}

	cout << "Enter the equations (example: 2x1+3x2-5=10):\n";
	cin.ignore();
	for (int i = 0; i < n; i++) {
		string equation;
		getline(cin, equation);
		store_equation(i, equation, equations, right_side);
	}

	
	while (true) {
		cout << "\n=== COMMAND MENU ===\n";
		cout << "1. num_vars       - Print number of variables\n";
		cout << "2. equation        - Print specific equation\n";
		cout << "3. equations        - Print All equation\n";
		cout << "4. column          - Print column of variable coefficients\n";
		cout << "5. add             - Add two equations\n";
		cout << "6. subtract        - Subtract two equations\n";
			cout << "7. cramer          - Print Cramer's matrix with specific variable column replaced\n";
		cout << "8. matrix          - Print Cramer's coefficient matrix\n";
	cout << "9. exit            - Quit program\n";
		cout << "Choose a command: ";

		string choice;
		cin >> choice;

		if (choice == "1" || choice == "num_vars") {
			cout << "Number of variables: " << n << endl;
		}
		else if (choice == "2" || choice == "equation") {
			int i;
			cout << "Enter equation number: ";
			cin >> i;
			print_equation(equations[i - 1], n, right_side[i - 1]);
		}
		else if (choice == "3" || choice == "All Equation") {
			
			print_All_equation(equations, n, right_side);
		}
		else if (choice == "4" || choice == "column") {
			string var;
			cout << "Enter variable name (e.g., x2): ";
			cin >> var;
			int idx = stoi(var.substr(1)) - 1;
			for (int i = 0; i < n; i++) {
				cout << equations[i][idx] << " ";
			}
			cout << endl;
		}
		else if (choice == "5" || choice == "add") {
			int a, b;
			cout << "Enter first equation number: ";
			cin >> a;
			cout << "Enter second equation number: ";
			cin >> b;
			int* result = new int[n];
			for (int i = 0; i < n; i++) result[i] = equations[a - 1][i] + equations[b - 1][i];
			int rhs = right_side[a - 1] + right_side[b - 1];
			print_equation(result, n, rhs);
			delete[] result;
		}
		else if (choice == "6" || choice == "subtract") {
			int a, b;
			cout << "Enter first equation number: ";
			cin >> a;
			cout << "Enter second equation number: ";
			cin >> b;
			int* result = new int[n];
			for (int i = 0; i < n; i++) result[i] = equations[a - 1][i] - equations[b - 1][i];
			int rhs = right_side[a - 1] - right_side[b - 1];
			print_equation(result, n, rhs);
			delete[] result;
		}
		
		else if (choice == "7" || choice == "cramer") {
			string var;
			cout << "Enter variable name (e.g., x1): ";
			cin >> var;
			print_cramer_column(equations, right_side, n, var);
		}
		else if (choice == "8" || choice == "matrix") {
			print_coefficient_matrix(equations, n);
		}
		
		else if (choice == "9" || choice == "exit") {
			break;
		}
		
		else {
			cout << "Unknown command! Please try again.\n";
		}
	}

	
	for (int i = 0; i < n; i++) delete[] equations[i];
	delete[] equations;
	delete[] right_side;


	}

