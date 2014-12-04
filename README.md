Minimalistic Makefile Project
Makefile générique

Makefile idéal pour la construction rapide d'un exécutable sans se préoccuper
de la gestion parfois complexe du Makefile : la fénéantise n'est plus une
excuse,

**********************
* Utilisation simple *
**********************

1. décompresser l'archive
2. renommer le répertoire ProjetMakefile
3. placer les sources dans le répertoire src
4. lancer make

make va automatiquement gérer les dépendances entre les fichiers sources et
créer un exécutable dans le répertoire Debug. Cet exécutable portera le nom
du répertoire qui contient le Makefile.
Exemple:

~/Projets> unzip ProjetMakefile.zip
~/Projets> mv ProjetMakefile test
~/Projets> cd test
~/Projets/test> cat > src/main.c << EOF
> #include <stdio.h>
> 
> int main()
> {
>   puts("Hello world");
> 
>   return 0;
> }
> EOF
~/Projets/test> make
GENDEP   libutil/xlog.c
GENDEP   src/main.c
CC       src/main.c
CC       libutil/xlog.c
AR       libutil.a
CCLD     Debug/test
debug version built
~/Projets/test> ls
Debug/  libutil/  license.txt  Makefile  src/
~/Projets/test> ./Debug/test 
Hello world
~/Projets/test> 

***********************
* Utilisation avancée *
***********************

Le fichier Makefile est commenté. Les options et variables qui dictent son
comportement sont décrites lors de leur déclaration.
Ce Makefile est idéal dans le cas d'un petit projet qui ne crée qu'un
exécutable, qui peut utiliser une bibliothèque de commodité et d'autres
bibliothèques gérées par pkg-config.

Deux types principaux de build sont supportés :
* Debug   : aucune option d'optimisation activée
* Release : options d'optimisation classiques activées

Dans chaque cas, il est possible d'ajouter les options pour le profilage et
le support des pthreads.

Le build peut se faire en mode SILENT, c'est-à-dire que les commandes ne sont
pas affichées mais une indication (comme dans l'exemple ci-dessus) de ce qui
est fait. Cette option est facilement débrayable soit en modifiant le Makefile,
soit par la ligne de commande : make SILENT=0 (cf les commentaires dans le 
Makefile).

Remarques

 * par défaut le compilateur utilisé est gcc en mode C99 avec les extensions
GNU (builtins et attibutes gcc et fonctions GNU_SOURCE de la glibc). Pour
uriliser ce Makefile laversion GNU de make est indispensable.

 * le template est fourni avec un exemple de bibliothèque de commodité
 permettant un logging simple (4 niveaux debug/info/warning/error, sortie
 uniquement vers stderr)

 * Ce template n'a pas la prétention d'être universel ou de remplacer d'autres
 outils comme autoconf et cie. Il a pour seul but de simplifier le prototypage
 ou la construction de petits projets. Entre autre il n'y a aucune cible
 install ni aucune vérification de la présence de fonctionnalités ou d'outils.

N'hésitez pas à laisser un commentaire, des propositions d'améliorations ou de
correction. N'hésitez pas non plus si vous avez la moindre question à propos
de cette contribution.

K.
