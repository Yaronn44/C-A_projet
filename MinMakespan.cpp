#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <chrono>
#include <algorithm>
#include <stdlib.h>

using namespace std;

// g++ -std=c++0x -Ofast -W -Wall -Wextra -pedantic -Wno-sign-compare -Wno-unused-parameter MinMakespan.cpp -o MinMakespan && ./MinMakespan

int saisie_fichier(int &nbM, int &nbT, vector<int> &tempsT){

	string file = "";

	system("clear");
	cout << "Veuillez fournir le nom du fichier texte (avec l'extension) \n";
	cout << "Cela peut être  son chemin absolu ou realtif : \n";

	cin >> file;
	ifstream read(file, ios::in);  // We open the csv file for reading it
	
	if(read){
		
		string instance = "";

		nbM = 0;
		nbT = 0;
		tempsT.clear();

		getline(read, instance);

		string delimiter = ":";

		nbM = 0;
		nbT = 0;
		tempsT.clear();

		string::size_type sz;
		string token;
		try{
			token = instance.substr(0, instance.find(delimiter));
			nbM = stoi(token, &sz);
			instance.erase(0, delimiter.length() + token.size());

			token = instance.substr(0, instance.find(delimiter));
			nbT = stoi(token, &sz);
			instance.erase(0, delimiter.length() + token.size());

			for (int i = 0; i < nbT; ++i){
				
				token = instance.substr(0, instance.find(delimiter));
				tempsT.push_back(stoi(token, &sz));
				instance.erase(0, delimiter.length() + token.size());
			}
		 }catch(const invalid_argument){

		 	system("clear");
			cerr << "Le fichier est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
			read.close();
			return 1;
		}

		if(tempsT.size() != nbT || nbM == 0 || nbT == 0){

			system("clear");
			cerr << "Le fichier est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
			read.close();
			return 1;
		}

	} else{

		system("clear");
		cerr << "Le fichier a mal été chargé. \n\n";
		read.close();
		return 1;
	}

	read.close();
	return 0;
}

int saisie_console(int &nbM, int &nbT, vector<int> &tempsT){

	system("clear");
	cout << "Veuillez saisir l'instance voulu au format (m:n:d1:d2:...:dn) sans les parenthèses \n";
	
	string instance = "";
	
	cin >> instance;

	string delimiter = ":";

	nbM = 0;
	nbT = 0;
	tempsT.clear();

	string::size_type sz;
	string token;

	try{
		token = instance.substr(0, instance.find(delimiter));
		nbM = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		token = instance.substr(0, instance.find(delimiter));
		nbT = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		for (int i = 0; i < nbT; ++i){
			
			token = instance.substr(0, instance.find(delimiter));
			tempsT.push_back(stoi(token, &sz));
			instance.erase(0, delimiter.length() + token.size());
		}

	}catch(const invalid_argument){

		system("clear");
		cerr << "L'instance est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
		return 1;
	}

	if(tempsT.size() != nbT || nbM == 0 || nbT == 0){

		system("clear");
		cerr << "L'instance est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
		return 1;
	}
	return 0;
}


int saisie_aleatoire(int &nbM, int &nbT, vector<vector<int> > &tempsTR){

	system("clear");
	cout << "Veuillez saisir le format des instances au format (m:n:k:min:max) sans les parenthèses \n";
	cout << "Avec m : nbMachines, n : nbtaches, k : nbInstances, min et max : valeurs limites des taches.\n";

	string instance = "";
	
	cin >> instance;

	string delimiter = ":";

	nbM = 0;
	nbT = 0;
	tempsTR.clear();

	int nbInstances;
	int min;
	int max;

	string::size_type sz;
	string token;

	try{

		token = instance.substr(0, instance.find(delimiter));
		nbM = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		token = instance.substr(0, instance.find(delimiter));
		nbT = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		token = instance.substr(0, instance.find(delimiter));
		nbInstances = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		token = instance.substr(0, instance.find(delimiter));
		min = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		token = instance.substr(0, instance.find(delimiter));
		max = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		for(int i = 0; i < nbInstances; ++i){

			vector<int> vector;
			for(int j = 0; j < nbT; ++j){
				vector.push_back(rand() % max + min);
			}
			tempsTR.push_back(vector);
		}
	}catch(const invalid_argument){

		system("clear");
		cerr << "L'instance est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
		return 1;
	}

	return 0;
}


int borne_inf_moy(int nbM, vector<int> tempsT){
	int sum_of_elems = 0;

	for (auto& n : tempsT)
    	sum_of_elems += n;

    return sum_of_elems/nbM;
}


int borne_inf_max(vector<int> tempsT){
	vector<int>::iterator result = max_element(tempsT.begin(),tempsT.end());
	return tempsT[distance(tempsT.begin(),result)];
}


bool compare(const pair<int, int>&i, const pair<int, int>&j){
    return i.second >= j.second;
}


int get_min_machine(vector<pair<int,vector<int>>> taches_par_machines){
	int min = taches_par_machines[0].first;
	int index_machine = 0;
	for (int i = 1; i < taches_par_machines.size(); ++i){
		if (taches_par_machines[i].first < min){
			min = taches_par_machines[i].first;
			index_machine = i;
		}
	}
	return index_machine;
}


double LSA(int nbM, int nbT, vector<int> tempsT){

 	chrono::steady_clock::time_point start = chrono::steady_clock::now();
    
 	int compt = 0;
 	vector<pair<int,vector<int>>> taches_par_machines;

 	for (int i = 0; i < nbM; ++i){
 		vector<int> vector;
 		taches_par_machines.push_back(make_pair(0,vector));
 	}

 	while(compt < nbT){

 		int min_machine = get_min_machine(taches_par_machines);

 		taches_par_machines[min_machine].first += tempsT[compt];
 		taches_par_machines[min_machine].second.push_back(compt+1);
 		++compt;
 	}

 	/*
 	cerr << "LSA : \n";
 	for (int i = 0; i < taches_par_machines.size(); ++i){
 		cerr << "Machine n°" << i+1 << " :  ";
 		for (int j = 0; j < taches_par_machines[i].second.size(); ++j){
 			cerr << taches_par_machines[i].second[j] << ", ";
 		}
 		cerr << endl;
 	}
 	cerr << endl;
 	*/

   	chrono::steady_clock::time_point end = chrono::steady_clock::now();
   	chrono::duration<double> elapsed_seconds = (end - start);	
    
    return elapsed_seconds.count();
}


double LPT(int nbM, int nbT, vector<int> tempsT){

	chrono::steady_clock::time_point start = chrono::steady_clock::now();

	int compt = 0;

	vector<pair<int,int> > liste_taches;

 	for (int i = 0; i < tempsT.size(); ++i){
 		liste_taches.push_back(make_pair(i+1,tempsT[i]));
 	}

 	sort(liste_taches.begin(), liste_taches.end(), compare);

 	vector<pair<int,vector<int>>> taches_par_machines;

 	for (int i = 0; i < nbM; ++i){
 		vector<int> vector;
 		taches_par_machines.push_back(make_pair(0,vector));
 	}

	while(compt < nbT){

		int min_machine = get_min_machine(taches_par_machines);

 		taches_par_machines[min_machine].first += liste_taches[compt].second;
 		taches_par_machines[min_machine].second.push_back(liste_taches[compt].first);;
 		++compt;
	}

	/*
	cerr << "LPT : \n";
	for (int i = 0; i < taches_par_machines.size(); ++i){
		cerr << "Machine n°" << i+1 << " :  ";
		for (int j = 0; j < taches_par_machines[i].second.size(); ++j){
			cerr << taches_par_machines[i].second[j] << ", ";
		}
		cerr << endl;
	}
	cerr << endl;
	*/
	
  	chrono::steady_clock::time_point end = chrono::steady_clock::now();
  	chrono::duration<double> elapsed_seconds = (end - start);	
   
   return elapsed_seconds.count();
}

double MyAlgo(int nbM, int nbT, vector<int> tempsT){

	chrono::steady_clock::time_point start = chrono::steady_clock::now();



	/*TODO*/


	vector<pair<int,vector<int>>> taches_par_machines;

	for (int i = 0; i < nbM; ++i){
		vector<int> vector;
		taches_par_machines.push_back(make_pair(0,vector));
	}


	/*TODO*/

	/*
	cerr << "MyAlgo : \n";
	for (int i = 0; i < taches_par_machines.size(); ++i){
		cerr << "Machine n°" << i+1 << " :  ";
		for (int j = 0; j < taches_par_machines[i].second.size(); ++j){
			cerr << taches_par_machines[i].second[j] << ", ";
		}
		cerr << endl;
	}
	cerr << endl;
	*/

  	chrono::steady_clock::time_point end = chrono::steady_clock::now();
  	chrono::duration<double> elapsed_seconds = (end - start);

	return elapsed_seconds.count();
}


int main(int argc, char const *argv[]){

	system("clear");

	while(true){
		int nbMachines = 0;
		int nbTaches = 0;
		vector<int> tempsTaches;
		vector<vector<int>> tempsTachesRand;
		string instance_string;
		int instance_number;

		srand (time(NULL));

		string::size_type sz;

		do{
			cout << "Veuillez choisir le mode d'instances d'entrée (tapé 1, 2, 3 ou 4 pour choisir) : \n";
			cout << "[1] : Depuis un fichier \n[2] : Au clavier \n[3] : Génération aléatoire de plusieurs instances \n[4] : Quittez \n\n";
			
			cin >> instance_string;

			try{
				instance_number = stoi(instance_string, &sz);
			}catch(const invalid_argument){
				system("clear");
				cerr << "La valeur rentré n'est pas un nombre.\n\n";
				instance_number = 0;
			}
		}while(instance_number != 1 && instance_number != 2 && instance_number != 3 && instance_number != 4);

		int res;

		if (instance_number == 1){
			res = saisie_fichier(nbMachines, nbTaches, tempsTaches);
		}
		else if(instance_number == 2){
			res = saisie_console(nbMachines, nbTaches, tempsTaches);
		}
		else if (instance_number == 3){
			res = saisie_aleatoire(nbMachines, nbTaches, tempsTachesRand);
		}else{
			exit(0);
		}

		if (res == 0){

			system("clear");

			if (instance_number == 1 || instance_number == 2){
				
				int moyenne = borne_inf_moy(nbMachines, tempsTaches);
				int maximum = borne_inf_max(tempsTaches);
				double temps_LSA = LSA(nbMachines, nbTaches, tempsTaches);
				double temps_LPT = LPT(nbMachines, nbTaches, tempsTaches);
				double temps_MyAlgo;

				cout << "Borne inférieure ‘‘maximum’’ = " << maximum << "\n";
				cout << "Borne inférieure ‘‘moyenne’’ = " << moyenne << "\n";
				cout << "Résultat LSA = " << temps_LSA << "\n";
				cout << "Résultat LPT = " << temps_LPT << "\n";
				cout << "Résultat MyAlgo = " << temps_MyAlgo << "\n";
				cout << "--------------------------------------------------------------- \n\n";

			}else{
				cout << "Veuillez choisir un nom de fichier en sortie (cela peut comprendre son chemin absolu ou realtif) : \n";
				
				string fichier_sortie;

				cin >> fichier_sortie;

				ofstream write(fichier_sortie, ios::out | ios::trunc); 
			
				if(write){

					for (int i = 0; i < tempsTachesRand.size(); ++i){
						int moyenne = borne_inf_moy(nbMachines, tempsTachesRand[i]);
						int maximum = borne_inf_max(tempsTachesRand[i]);
						double temps_LSA = LSA(nbMachines, nbTaches, tempsTachesRand[i]);
						double temps_LPT = LPT(nbMachines, nbTaches, tempsTachesRand[i]);
						double temps_MyAlgo;

						write << "Borne inférieure ‘‘maximum’’ = " << maximum << "\n";
						write << "Borne inférieure ‘‘moyenne’’ = " << moyenne << "\n";
						write << "Résultat LSA = " << temps_LSA << "\n";
						write << "Résultat LPT = " << temps_LPT << "\n";
						write << "Résultat MyAlgo = " << temps_MyAlgo << "\n";
						write << "--------------------------------------------------------------- \n\n";
					}

					system("clear");
					cout << "L'opération c'est déroulé avec succès.\n\n";
				}
				else{
					cerr << "Le fichier a mal été chargé\n";
				}
		 	}
		}else{
			cerr << "La saisie de(s) instance(s) a echoué, veuillez réessayer.\n\n";
		}
	}

	return 0;
}



