#include <iostream>
#include <iomanip>
using namespace std;

#include "Lecteur.h"
#include "Symbole.h"

int main(int argc, char *argv[]) {
  string nomFich;
  if (argc != 2) {
    cout << "Usage : " << argv[0] << " nom_fichier_source" << endl << endl;
    cout << "Entrez le nom du fichier dont voulez-vous lire les symboles : ";
    getline(cin, nomFich);
  } else nomFich = argv[1];
  ifstream fichier(nomFich.c_str());
  Lecteur lecteur(fichier);
  while (lecteur.getSymbole() != "<FINDEFICHIER>") {
    cout << setw(3) << lecteur.getLigne() << "," << setw(3) << lecteur.getColonne() 
         << " - " << lecteur.getSymbole() << endl;
    lecteur.avancer();
  }
  cout << setw(3) << lecteur.getLigne() << "," << setw(3) << lecteur.getColonne() 
       << " - " << lecteur.getSymbole() << endl;
  return 0;
}
