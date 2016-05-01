# LISEZMOI

### Équipe

> **Cours:** Intelligence Artificielle (INF4230) - TP3
> **Prof:** Eric Beaudry
> **Élève1:** Alexandre Cayer-Boucher - Caya10119000 - cayer-boucher.alexandre@courrier.uqam.ca
> **Élève2:** John Béjot - BEJJ20119308 - bejot.john@courrier.uqam.ca


### Compilation

La compilation est relativement simple. Il faut d'abord générer le Makefile avec `qmake`, puis lancer la commande `make`:

```
$ qmake
$ make
```


### Exécution

1. Pour jouer au jeu il faut simplement exécuter:

`./GeneticAI`


Cependant, le programme peut prendre 3 paramètres en ligne de commande, qui ne sont utiles que en mode AI (c'est à dire, si l'option `--ai` est donnée):

- `--ai`, le jeu est joué par l'AI implémentée
- `--no_log`, les statistiques de la session ne seront enregistrées
- `--nop_gene`, le jeu utilise le génome du fichier `ai/mario_genes` sans le modifier

Les fichiers utilisés seront expliqués ultérieurement.

2. La première série de tests à effectuer avec l'IA pourrait être logguée, et nécessite une population initiale

`./GeneticAI --ai`

Cette commande va créer un dossier dans `ai/log-X` où X est la date et l'heure d'exécution du programme. Toutes les `NEAT::print_every` générations, un fichier texte sera créé dans le dossier de log avec les génotypes des membres de la population investiguée. La population initiale est créée à partir de celle lue dans `ai/mario_genes`

3. Rendus avec plusieurs génotypes, il est possible de lancer l'IA avec un réseau neuronal particulier. Pour ce faire, il faut remplacer le contenu du fichier `ai/mario_genes` par le génotype souhaité. Si l'on souhaite créer une population à partir de génotype, la commande **2** fera l'affaire, mais si l'on veut que l'IA utilise ce génotype sans le modifier, alors il faut rajouter une option:

`./GeneticAI --ai --nop_gene`

4. Quelque soit la raison, si l'on ne souhaite conserver un journal de la session, il suffit de rajouter l'option `--no_log`:

`./GeneticAI --ai --no_log [--nop_gene]`


### Fichiers

#### Configuration

Il y a 2 fichiers de configuration:

1. `ai/mario_params.ne`
2. `ai/mario_genes`

Le premier, `mario_params.ne`, contient des paramètres qu'utilisera NEAT pour faire évoluer les populations. Le fichier source `ai/neat/neat.h` contient des commentaires pour chacune des valeurs. Pour d'amples explications, se référer au livre des créateurs de NEAT: Kenneth O. Stanley & Risto Miikkulainen - "Competitive Coevolution through Evolutionary Complexification", 2004.

Le deuxième fichier, `mario_genes`, contient le génotype initial qui sera utilisé durant la session. Par défault, ce génotype représente une topologie minimale, et le programme s'en sert pour créer une population initiale. Il est cependant possible de remplacer ce génotype par un autre, pour créer une population intiale "plus avancée" ou juste pour utiliser un réseau neuronal particulier. La signification du format peut être trouvée dans le "NEAT Software Doc File", écrit par Kenneth O. Stanley en 2001.

#### Code

Tel que mentionné dans les autres documents, très peu de code fut écrit. Les seuls fichiers de code modifiés par notre équipe sont les suivants:

- `main.cpp` pour prendre compte des paramètres de ligne de commande
- `game.hpp` pour rajouter les signatures de fonctions
- `game.cpp` pour inclure l'IA dans le jeu, en liant l'application et NEAT
- `GeneticAI.pro` pour s'assurer que les dépendances (notamment à NEAT), soient respectées

Tout le reste du code provient de parties tierces, dont notamment:

- la librairie Boost, pour simplifier le C++
- la librairie SFML, pour le rendu graphique
- la librairie NEAT, noyau dur de notre AI
- les fichiers du jeu, écrit par Jérémy Goussé, un ami à John


