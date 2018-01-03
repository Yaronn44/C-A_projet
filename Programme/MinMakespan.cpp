#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <chrono>
#include <algorithm>
#include <stdlib.h>

using namespace std;

#define max(a,b) (a>=b?a:b)

// Ligne de commande pour la compilation et l'exécution du programme
// g++ -std=c++0x -Ofast -W -Wall -Wextra -pedantic -Wno-sign-compare -Wno-unused-parameter MinMakespan.cpp -o MinMakespan && ./MinMakespan


// Fonction utilisée pour permettre à l'utilisateur de choisir le fichier en entrée contenant une instance
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int saisie_fichier(int &nbM, int &nbT, vector<int> &tempsT){

	string file = "";

	 	
	cout << "Veuillez fournir le nom du fichier texte (avec l'extension) \n";
	cout << "Cela peut être  son chemin absolu ou relatif : \n";

	cin >> file;
	ifstream read(file, ios::in);  // We open the csv file for reading it
	
	// Si la lecture a réussi
	if(read){
		
		string instance = "";

		nbM = 0;
		nbT = 0;
		tempsT.clear();

		getline(read, instance);

		string delimiter = ":";

		string::size_type sz;
		string token;

		// On essaye de récupérer les différentes informations au sein de la chaine de caractère présente dans le fichier en entrée
		try{
			// On récupère le nombre de machine
			token = instance.substr(0, instance.find(delimiter));
			nbM = stoi(token, &sz);
			instance.erase(0, delimiter.length() + token.size());

			// On récupère le nombre de tache
			token = instance.substr(0, instance.find(delimiter));
			nbT = stoi(token, &sz);
			instance.erase(0, delimiter.length() + token.size());

			// On récupère les différentes taches d'en un vector
			for (int i = 0; i < nbT; ++i){
				
				token = instance.substr(0, instance.find(delimiter));
				tempsT.push_back(stoi(token, &sz));
				instance.erase(0, delimiter.length() + token.size());
			}
		// On catch les exception provoqué par une mauvaise chaine de caractère fournis en entrée
		 }catch(const invalid_argument){

		 	system("clear");
			cerr << "Le fichier est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
			read.close();
			return 1;
		}


		// Si il manque des taches dans l'instance d'entrée ou que des paramètres ont été initialisé à 0 on sort une erreur
		if(tempsT.size() != nbT || nbM == 0 || nbT == 0){

			system("clear");
			cerr << "Le fichier est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
			read.close();
			return 1;
		}
	// Si le fichier a mal été chargé on sort une erreur
	} else{

		system("clear");
		cerr << "Le fichier a mal été chargé. \n\n";
		read.close();
		return 1;
	}

	read.close();
	return 0;
}


// Fonction utilisée pour permettre à l'utilisateur de choisir l'instance d'entrée
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

	// On essaye de récupérer les différentes informations au sein de la chaine de caractère présente dans l'instance en entrée
	try{
		// On récupère le nombre de machine
		token = instance.substr(0, instance.find(delimiter));
		nbM = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());
		
		// On récupère le nombre de tache
		token = instance.substr(0, instance.find(delimiter));
		nbT = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		// On récupère les différentes taches d'en un vector
		for (int i = 0; i < nbT; ++i){
			
			token = instance.substr(0, instance.find(delimiter));
			tempsT.push_back(stoi(token, &sz));
			instance.erase(0, delimiter.length() + token.size());
		}
	// On catch les exception provoqué par une mauvaise chaine de caractère fournis en entrée
	}catch(const invalid_argument){

		system("clear");
		cerr << "L'instance est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
		return 1;
	}
	// Si il manque des taches dans l'instance d'entrée ou que des paramètres ont été initialisé à 0 on sort une erreur
	if(tempsT.size() != nbT || nbM == 0 || nbT == 0){

		system("clear");
		cerr << "L'instance est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
		return 1;
	}
	return 0;
}


// Fonction utilisée pour permettre à l'utilisateur de choisir les paramètres des instances qui vont être générée aléatoirement
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
		// On récupère le nombre de machine		
		token = instance.substr(0, instance.find(delimiter));
		nbM = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		// On récupère le nombre de tache	
		token = instance.substr(0, instance.find(delimiter));
		nbT = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());
		
		// On récupère le nombre d'instance
		token = instance.substr(0, instance.find(delimiter));
		nbInstances = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		// On récupère la valeur minimum des taches
		token = instance.substr(0, instance.find(delimiter));
		min = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		// On récupère la valeur maximum des taches
		token = instance.substr(0, instance.find(delimiter));
		max = stoi(token, &sz);
		instance.erase(0, delimiter.length() + token.size());

		// Si on fournit un minimum supérieur au maximum on sort une erreur
		if(min > max){
			throw invalid_argument("Min est supérieur à max.");
		}

		// On créer chacunes des instances
		for(int i = 0; i < nbInstances; ++i){

			vector<int> vector;
			for(int j = 0; j < nbT; ++j){
				vector.push_back(rand() % (max-min+1) + min);

			}
			tempsTR.push_back(vector);
		}
	// On catch les exception provoqué par une mauvaise chaine de caractère fournis en entrée
	}catch(const invalid_argument){

		system("clear");
		cerr << "L'instance est mal formé (espaces en trop par exemple), il manque des paramètres ou ceux-ci sont incorrectes. \n\n";
		return 1;
	}

	return 0;
}



// Fonctions utilitaires
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Fonction utilisé pour calculer la borne infèrieure "moyenne"
int borne_inf_moy(int nbM, vector<int> tempsT){
	long sum_of_elems = 0;

	for (auto& n : tempsT)
    	sum_of_elems += n;


    return sum_of_elems/nbM;
}


// Fonction utilisé pour calculer la borne infèrieure "maximum"
int borne_inf_max(vector<int> tempsT){
	vector<int>::iterator result = max_element(tempsT.begin(),tempsT.end());
	return tempsT[distance(tempsT.begin(),result)];
}


// Fonction de comparaisons utilisé par la fonction sort() sur les vector, tri par ordre décroissnat
bool compare_desc(const pair<int, int>&i, const pair<int, int>&j){
    return i.second > j.second;
}


// Fonction de comparaisons utilisé par la fonction sort() sur les vector, tri par ordre croissant
bool compare_asc(const pair<int, int>&i, const pair<int, int>&j){
    return i.second < j.second;
}


// Fonction permettant de récupérer l'index de la machine ayant le plus petit poids
int get_index_min_machines(vector<pair<int,vector<int>>> taches_par_machines){
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


// Fonction permettant de récupérer le poids le plus élevé des machines
int get_max_machine(vector<pair<int,vector<int>>> taches_par_machines){

	int max = taches_par_machines[0].first;
	
	for (int i = 1; i < taches_par_machines.size(); ++i){
		if (taches_par_machines[i].first > max){
			max = taches_par_machines[i].first;
		}
	}
	return max;
}


// Fonctions d'approximation
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Fonction LSA
int LSA(int nbM, int nbT, vector<int> tempsT){
    
 	int compt = 0;

 	// On créer un vector de pair, lui même constitué d'un entier : le temps total attribué à la machine, et un vector d'index : la liste des taches qui lui sont attribué
 	vector<pair<int,vector<int>>> taches_par_machines;

 	// On l'initialise
 	for (int i = 0; i < nbM; ++i){
 		vector<int> vector;
 		taches_par_machines.push_back(make_pair(0,vector));
 	}

 	// Tant que toutes les taches ne sont pas attribuées
 	while(compt < nbT){

 		int min_machine = get_index_min_machines(taches_par_machines);

 		// On attribue la suivante à la machine ayant le plus petit temps de réalisation des taches
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
 		cerr << "\n " << taches_par_machines[i].first << endl;
 	}
 	cerr << endl;
	*/
 	
    return get_max_machine(taches_par_machines);
}


// Fonction LPT
int LPT(int nbM, int nbT, vector<int> tempsT){

	int compt = 0;
	
	// On creer un vector de pair étant le numéro de chaque tache ainsi que sont temps de réalistion
	vector<pair<int,int> > liste_taches;

 	for (int i = 0; i < tempsT.size(); ++i){

 		liste_taches.push_back(make_pair(i+1,tempsT[i]));
 	}

 	// On classe les taches par temps de réalisation par ordre décroissant
 	sort(liste_taches.begin(), liste_taches.end(), compare_desc);

 	// On créer un vector de pair, lui même constitué d'un entier : le temps total attribué à la machine, et un vector d'index : la liste des taches qui lui sont attribué
 	vector<pair<int,vector<int>>> taches_par_machines;

	// On l'initialise
 	for (int i = 0; i < nbM; ++i){
 		vector<int> vector;
 		taches_par_machines.push_back(make_pair(0,vector));
 	}

	// Tant que toutes les taches ne sont pas attribuées
	while(compt < nbT){

		int min_machine = get_index_min_machines(taches_par_machines);

		// On attribue la tache suivante à la machine ayant le plus petit temps de réalisation des taches
 		taches_par_machines[min_machine].first += liste_taches[compt].second;
 		taches_par_machines[min_machine].second.push_back(liste_taches[compt].first);
 		++compt;
	}

	/* 
	cerr << "LPT : \n";
	for (int i = 0; i < taches_par_machines.size(); ++i){
		cerr << "Machine n°" << i+1 << " :  ";
		for (int j = 0; j < taches_par_machines[i].second.size(); ++j){
			cerr << taches_par_machines[i].second[j] << ", ";
		}
		cerr << "\n " << taches_par_machines[i].first << endl;
	}
	cerr << endl;
	*/
   
   	return get_max_machine(taches_par_machines);
}


// Algorithme personnel
int MyAlgo(int nbM, int nbT, vector<int> tempsT){


	// On creer deux vectors de pairs étant le numéro de chaque tache ainsi que sont temps de réalistion
	vector<pair<int,int> > liste_taches_asc;
	vector<pair<int,int> > liste_taches_desc;

	// On initialise ces deux vector
 	for (int i = 0; i < tempsT.size(); ++i){

 		liste_taches_asc.push_back(make_pair(i+1,tempsT[i]));
 		liste_taches_desc.push_back(make_pair(i+1,tempsT[i]));
 	}

 	// On sort les deux vector, l'un par ordre décroissant et l'autre par ordre croissant des valeurs des taches
 	sort(liste_taches_asc.begin(), liste_taches_asc.end(), compare_asc);
 	sort(liste_taches_desc.begin(), liste_taches_desc.end(), compare_desc);
 	
 	// On créer un vector de pair, lui même constitué d'un entier : le temps total attribué à la machine, et un vector d'index : la liste des taches qui lui sont attribué
 	vector<pair<int,vector<int>>> taches_par_machines;

 	// On l'initialise
 	for (int i = 0; i < nbM; ++i){
 		vector<int> vector;
 		taches_par_machines.push_back(make_pair(0,vector));
 	}

 	bool same = false;
 	int machine;
 	int compt1 = 0;
 	int compt2 = 0;

 	// Tant que toutes les taches ne sont pas attribuées
	while(compt1 + compt2 < nbT){

		// Si on est pas sur la même machine que la précédente
		if (!same){
			// On prend la machine ayant la 
			machine = get_index_min_machines(taches_par_machines);

			// On attribue la tache suivante à la machine ayant le plus petit temps de réalisation des taches
			// En prenant une tache de poids fort
	 		taches_par_machines[machine].first += liste_taches_desc[compt1].second;
	 		taches_par_machines[machine].second.push_back(liste_taches_desc[compt1].first);
	 		++compt1;
	 		same = true;
	 	// Si on est sur la même machine que la précédente
	 	}else{

	 		// On attribue la tache suivante à la même machine en prenant une tache de poids faible
	 		taches_par_machines[machine].first += liste_taches_asc[compt2].second;
	 		taches_par_machines[machine].second.push_back(liste_taches_asc[compt2].first);
	 		++compt2;
	 		same = false;
	 	}
	}

	/*
	cerr << "LPT : \n";
	for (int i = 0; i < taches_par_machines.size(); ++i){
		cerr << "Machine n°" << i+1 << " :  ";
		for (int j = 0; j < taches_par_machines[i].second.size(); ++j){
			cerr << taches_par_machines[i].second[j] << ", ";
		}
		cerr << "\n " << taches_par_machines[i].first << endl;
	}
	cerr << endl;
	*/

	return get_max_machine(taches_par_machines);
}


// Fonction principale 
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
				int temps_LSA = LSA(nbMachines, nbTaches, tempsTaches);
				int temps_LPT = LPT(nbMachines, nbTaches, tempsTaches);
				int temps_MyAlgo = MyAlgo(nbMachines, nbTaches, tempsTaches);

				cout << "Borne inférieure ‘‘maximum’’ = " << maximum << "\n";
				cout << "Borne inférieure ‘‘moyenne’’ = " << moyenne << "\n";
				cout << "Résultat LSA = " << temps_LSA << "\n";
				cout << "Résultat LPT = " << temps_LPT << "\n";
				cout << "Résultat MyAlgo = " << temps_MyAlgo << "\n";
				cout << "--------------------------------------------------------------- \n\n";

			}else{
				cout << "Veuillez choisir un nom de fichier en sortie (cela peut comprendre son chemin absolu ou relatif) : \n";
				
				string fichier_sortie;

				cin >> fichier_sortie;

				ofstream write(fichier_sortie, ios::out | ios::trunc); 
			
				if(write){

					vector<double> M;
					vector<vector<double>> R;
					vector<double> ratio;

					ratio.push_back(0);
					ratio.push_back(0);
					ratio.push_back(0);

					for (int i = 0; i < tempsTachesRand.size(); ++i){

						int moyenne = borne_inf_moy(nbMachines, tempsTachesRand[i]);
						int maximum = borne_inf_max(tempsTachesRand[i]);
						int temps_LSA = LSA(nbMachines, nbTaches, tempsTachesRand[i]);
						int temps_LPT = LPT(nbMachines, nbTaches, tempsTachesRand[i]);
						int temps_MyAlgo = MyAlgo(nbMachines, nbTaches, tempsTachesRand[i]);

						M.push_back(max(moyenne, maximum));
						
						vector<double> vector;
						R.push_back(vector);

						write << "Borne inférieure ‘‘maximum’’ = " << maximum << "\n";
						write << "Borne inférieure ‘‘moyenne’’ = " << moyenne << "\n";
						write << "Résultat LSA = " << temps_LSA << "\n";
						write << "Résultat LPT = " << temps_LPT << "\n";
						write << "Résultat MyAlgo = " << temps_MyAlgo << "\n";
						write << "--------------------------------------------------------------- \n\n";
						
						ratio[0] += temps_LSA/M[i];
						ratio[1] += temps_LPT/M[i];
						ratio[2] += temps_MyAlgo/M[i];
					}

					ratio[0] /= tempsTachesRand.size();
					ratio[1] /= tempsTachesRand.size();
					ratio[2] /= tempsTachesRand.size();

					write << "ratio d'approximation moyen LSA = " << ratio[0] << "\n";
					write << "ratio d'approximation moyen LPT = " << ratio[1] << "\n";
					write << "ratio d'approximation moyen MyAlgo = " << ratio[2] << "\n\n";
					
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



