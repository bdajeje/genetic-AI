# NEAT Ninja Mario



- **Sujet:** Algorithmes génétiques et réseaux de neurones sur un jeu de plateforme
- **Contexte:** Présenté à Pr. Éric Beaudry dans le cours INF4230 (Hiver 2016)
- **Équipe:** #3
- **Membres:** Alexandre Cayer-Boucher et John Béjot
- **Date:** 4 mai 2016



## 1. Introduction

L'application que nous avons décidé de résoudre est un simple jeu de plateforme dynamique (en déplacement continu). Les jeux de plateformes sont une cible intéressante pour l'apprentissage machine basique, car la stratégie requise est minime, avec une emphase sur les réflexes. Le temps requis pour qu'un agent apprenne à jouer est moindre comparé à d'autres jeux offrant plus de possibilités d'action, et l'apprentissage peut s'effectuer de manière non-supervisée. 

Destiné à être joué par des individus humains, seuls, il est rare de voir des agents intelligents jouer à des jeux de plateforme. Les agents intelligents vont plutôt être sollicités dans des jeux offrant la possibilité de compétition. Néanmoins, il est très intéressant de voir comment une machine s'y prendrait pour ce type de jeu: son comportement convergera-t-il vers le nôtre ? Ou peut-être sommes-nous coincés dans un optimum local, duquel les agents intelligents ont su sortir ? 

Dans notre jeu, Mario doit sauter par-dessus des trous et éviter des oiseaux en se baissant, afin de survivre dans l'environnement hostile dans lequel il est. Son score final est calculé en fonction du temps qu'il a survécu, indépendamment du nombre d'obstacles évités. La collision avec un seul obstacle, trou ou oiseau, entraine une mort dans d'affreuses souffrances. Mario ignore malheureusement que la partie ne prendra fin qu'à sa mort, c'est-à-dire qu'il ne peut pas « compléter » le jeu.



## 2. Problématique

Pour une configuration quelconque de l'environnement, le joueur artificiel devra procéder de façon optimale: c'est-à-dire effectuer une action qui ne résultera pas directement en sa mort. Les obstacles sont générés de façon aléatoire, et en se basant sur un tuple de paramètres simples _\<distance du trou, taille du trou, distance oiseau\>_, l'agent devra retourner une action qui lui assurera la survie. C'est à la discrétion de l'agent de prévoir toutes les combinaisons possibles de paramètres.

L’environnement de notre problème est : 

- Partiellement Observable\*
- Agent Seul
- Déterministe
- Épisodique 
- Dynamique
- Continu

\* À tout moment, l'agent dispose des informations qu'il lui faut pour survivre (son champ de vision est l'écran, ce qui se passe en dehors de l'écran ne peut en aucun cas l'affecter au temps `t`). Cependant, les obstacles sont générés aléatoirement et à l'infini, donc l'environnement au complet n'est pas entièrement observable.



## 3. Technique d'IA retenue

Il y a plusieurs choix qui pourraient nous permettre de résoudre un problème comme le nôtre. Il serait préférable que notre algorithme d'intelligence artificielle puisse s'adapter avec le temps pour résoudre un problème changeant. Notre jeu est simple présentement, mais il est voué à se complexifier.

Techniques | Commentaires
---|---
"Hard Coder" les règles | Notre jeu serait aisé à résoudre avec des valeurs "hard codées", comme à partir de quelle distance d'oiseau il faut se baisser. Par contre, nous souhaitons avoir un algorithme d'IA et une application qui puisse évoluer avec notre jeu, peu importe si les mécaniques du jeu passé restent les mêmes ou non
A\* | Il serait possible d'imaginer un processus qui percevrait un chemin optimal vers la fin, visible, d'un niveau. La nature continue des variables, ansi que la possibilité de complexification du jeu, rendraient cependant la taille de l'espace d'états trop importante
Planification de mouvement | Ce processus serait très adapté à ce type de problème et fonctionnerait très bien dans plusieurs types d'environnement. Il est fortement à envisager.

Les Algorithmes Génétiques avec Réseaux Neuronaux (AGRN) sont des techniques **capables d'apprendre** et de **gérer des variables continues**. Allons voir ce que peuvent nous apporter les AGRN dans notre problématique. Bien que souvent couplés, ces deux outils n'ont pas la même fonction, et agissent de manière complémentaire:

- Un **réseau neuronal** est une entité qui va affecter directement la décision. En fonction de paramètres donnés, une simulation neuronale répondant à des lois mathématiques va retourner un certain résultat (ici, une seule valeur). Les réseaux neuronaux (abrégé *RN*) sont capables d'apprentissage: leur structure interne peut être modifiée afin de changer la valeur de sortie pour un jeu d'entrées donné. Dans notre cas, un RN efficace retournera par exemple une valeur entrainant l'action "se baisser" lorsque l'input désignant la distance de l'oiseau descend en dessous d'un certain seuil. Les RN ne calculent pas d'espace d'états, et computent les nombres réels de la même manière que les nombres naturels.

- Pour faire évoluer efficacement des RN, on peut faire appel à un **algorithme génétique** (abrégé *AG*). L'AG ne va pas directement influencer la décision: il a seulement pour but d'améliorer les RN, tout en conservant leurs caractéristiques prisées. Par exemple, un RN qui sait se baisser lorsqu'un oiseau s'approche va maintenant vouloir apprendre à sauter par dessus les trous, et cela sans oublier de se baisser au moment opportun ! C'est donc l'AG qui permet aux RN d'apprendre. Basé sur la théorie de l'évolution de Darwin, un AG va typiquement traiter une population de RN, en les faisant évoluer sur plusieurs générations à travers des mécanismes plus ou moins complexes, et qui seront expliqués dans une prochaine partie. En manipulant plusieurs RN plutôt qu'un, et en modifiant leur structure interne, l'AG va en quelque sorte permettre l'apparition de RN "champions", c'est-à-dire les RN les plus aptes à survivre dans leur environnement, i.e. ceux qui obtiennent le meilleur score au jeu.



## 4. Particularités et Implémentation

Les particularités propres à ce projet viennent de NEAT. Les librairies AGRN requièrent généralement la spécification de la topologie des RN, et vont soit jouer uniquement sur les poids entre les noeuds, soit altérer la topologie. Outre la difficulté de choisir une structure appropriée (qui peut aussi être générée au hasard), une conséquence directe est l'abondance de noeuds qui pourraient retarder l'apprentissage du RN, et même lui être contre-productive. En effet, chaque noeud en plus rajoute un poids dans l'équation, et si un noeud est superflu, il nécessitera beaucoup d'efforts pour "isoler" son influence.

L'avantage de NEAT réside du fait que son point de départ est une topologie minimale, i.e. avec les entrées connectées directement aux sorties. Au fur et à mesure, des noeuds et des liens vont se rajouter, augmentant ainsi la topologie, comme son nom l'indique. Cette méthode vise à éliminer la redondance précédemment citée, en gardant une structure minimale en tout temps: un neurone apparait et survit dans la topologie seulement s'il est nécessaire à la survie du réseau, et donc que son influence est positive à la synergie globale. De plus, cela permet aussi d'alléger les calculs dans les premières générations.

Un autre point majeur concernant NEAT est sa spéciation. Derrière ce mot se cache une technique pour permettre à l'innovation de se développer, et de survivre le temps de devenir utile. Par exemple, les premiers animaux à avoir développé des ailes n'ont pas été les premiers à voler, car leurs ailes devaient être petites et faibles. Mais en permettant à ces futurs oiseaux de continuer d'étendre leurs ailes, c'est-à-dire en permettant à cette innovation de se développer, de nouvelles espèces ont pu dominer leur environnement grâce à leur capacité à voler. La spéciation permet à la recherche de s'effectuer dans différents espaces simultanément, et donc d'éviter que tous convergent vers un seul optimum local.

### 4.1 Détails d’implémentation

Bien que NEAT requiert une topologie minimale comme génome de base, un fichier de configuration grandement paramétrable nous permet d'influencer la manière dont les RN vont évoluer au cours des générations. On a entre autres le seuil de compatibilité, le seuil de survie, la probabilité de mutation, les coefficients de compatibilité, etc.

Le processus est simple: pour tout organisme d'une population, on lui fait essayer le jeu et on note son score (fitness). Un RN joue en chargeant les inputs dans ses senseurs, puis en s'activant retourne une valeur calculée. Cette valeur amènera une action à être effectuée. Une fois les fitness de chaque membre recensés, on demande à NEAT de faire évoluer la population courante pour la génération suivante. On répète ce processus X * Y fois, X étant le nombre de générations voulues et Y le nombre d'essais. 

### 4.2 Code externe

En fin de compte, le travail fourni a été grandement investi dans la recherche: une fois NEAT compris, il nous a seulement fallu le brancher au jeu pour que tout marche. À part le lien entre NEAT et le jeu, tout le reste est du code externe:

- le jeu a été développé en C++ par Jérémy Goussé, un ami à John. Il utilise les librairies:
    - Boost, pour tous les "raccourcis" C++
    - SFML, pour faire le rendu graphique
- l'outil NEAT a été développé en C++ par Kenneth Stanley, un professeur à la University of Central Florida



## 5. Expérimentation, résultats et limites

### But

Le jeu étant infini, nous avons posé comme limite de victoire le score de 1000. Si un RN réussissait à faire 1000 ou plus, alors on lui décernait l'appellation de "Ninja". Avec plusieurs paramètres différents, NEAT a toujours réussi à faire évoluer des "Ninjas". L'apprentissage a donc été un succès dans 100% des cas.

### Expérimentations

Impact du nombre d'organismes dans la population (taux de mutation = 0.3)

Temps avant Ninja | > 8h | 4h | 5h | 6h30
---|---|---|---|---
Nb membres | 5 | 35 | 50 | 100

Impact du taux de mutation aléatoire (population = 35 organismes)

Temps avant Ninja | 6h | 4h | 8h 
---|---|---|---
Taux mutation | 0 < x < 0.2 | 0.2 <= x <= 0.8 | 0.8 < x < 1

Impact du nombre moyen d'espèces (population = 35 organismes, taux de mutation = 0.3)

Temps avant Ninja | 7h30 | 4h | 4h30 | 5h | > 8h
---|---|---|---|---|---
Nb espèces | 1 | 5 | 10 | 20 | 30

### Résultats

- une population trop importante (> 50) ne fait qu'allonger le temps requis pour arriver aux Ninjas. Ce temps, bien que computationnel, peut-être réduit en choisissant une taille de population moyenne, et en passant plus de temps à la faire évoluer
- un coefficient de mutation trop élevé causait parfois une régression dans l'évolution des RN: une innovation qui paraissait prometteuse pouvait disparaître avant même d'impacter d'autres organismes
- un grand nombre d'espèces a le même effet qu'une trop grande population: trop de recherches simultanées se ralentissent entre elles

Ce dernier point est intéressant, car nous avons distingué deux groupes de Mario dans le jeu, apparaissant systématiquement:

1. des Mario qui sautillent tout le temps, et qui peuvent par chance éviter certains trous, mais se prennent le 1er oiseau
2. des Mario qui ne gardent que la tête baissée, et qui évitent tous les oiseaux, mais se prennent le 1er trou

Une compétition a eu lieu entre ces deux groupes: le premier à apprendre la compétence de l'autre pourrait devenir l'espèce dominante. Le groupe 1 a compris en premier qu'il fallait se baisser en présence d'un oiseau, mais ces sauts répétitifs lui ont valu de tomber dans un trou tôt ou tard ; n'étant pas possible de faire de doubles sauts, pour éviter un trou il faut être à terre pour sauter au bon moment, et pour sauter au bon moment il faut ne pas sauter tout le temps. Le groupe 1 devait donc désapprendre le saut en quelque sorte. C'est pour cette raison que le groupe 2 finissait toujours par générer les Ninjas en premier, car il lui suffisait juste d'apprendre à sauter au bon moment étant donné qu'il évitait déjà les oiseaux. 

### Limites

Le fait qu'on ait remarqué seulement 2 types de styles de jeu, même en variant les paramètres, est dû à la simplicité du jeu ; en effet à un moment `t` il n'y a que 3 actions à faire. La limite dans ce cas n'a pas été atteinte par la technologie utilisée, mais par la problématique. Les RN convergent toutes vers un même style de jeu ; il n'y a pas vraiment place à l'innovation. Un jeu plus complexe aurait permis de voir plusieurs stratégies IA se développer, et les mettre en compétition aurait été un bon moyen de les comparer.



## 6. Conclusion

NEAT a été un excellent choix pour notre projet. La lecture de ses manifestes nous a donné un aperçu des monstrueuses possibilités des AGRN. Ses fonctionnalités uniques qu'il offre, comparées à d'autres librairies AGRN, sont véritablement remarquables. Dans 100% des cas, NEAT a trouvé une solution au problème. 

Malheureusement, la simplicité du jeu a fait qu'on n'ait pas pu exploiter au maximum NEAT. Des améliorations possibles, dans le cadre du jeu, pourraient être:

- pénaliser toute autre action que "rester debout" ou récompenser l'évitement de chaque obstacle 
- augmenter la difficulté avec le temps, en accélérant le jeu par exemple (il faudra rajouter un input _vitesse_ au réseau)
- ajouter de nouvelles actions, comme frapper un ennemi



## 7. Répartition des tâches

Nous avons tous les deux fait des lectures intensives sur les réseaux neuronaux et les algorithmes génétiques afin de nous aiguiller vers une méthode qui nous intéressait. Notre choix est tombé sur NEAT, qui a été recommandé par le professeur, mais que nous avons aussi retrouvé lors de nos recherches. Nous avons ensuite lu des publications sur NEAT, la théorie qui se cache derrière, comment il se compare aux autres AGRN, et comment se servir du code.

Le code, découlant des recherches effectuées ensemble, a été formulé en groupe, mais écrit par John, plus à l'aise avec le C++. Pendant ce temps, Alexandre préparait les diapositives de présentation. Les spéculations sur NEAT, les révisions de code, les interrogations sur ARGN, et les simulations de notre IA ont été effectuées en groupe. La rédaction du document s'est faite par les deux membres de l'équipe.

Notre base de connaissance était complètement partagée, et à tout moment, chacun savait ce que faisait l'autre, et était capable de le faire soi-même. Les tâches ont été équitablement réparties, toute la réflexion s'est faite en équipe et chacun s'est investi autant que l'autre pour ce projet.



## Références

1. S. Russell and P. Norvig. Artificial Intelligence: A Modern Approach. Prentice Hall, troisième édition, 2010
2. Kenneth O. Stanley and Risto Miikkulainen. Evolving Neural Networks through Augmenting Topologies, 2002 
3. Kenneth O. Stanley and Risto Miikkulainen. Competitive Coevolution through Evolutionary Complexification, 2004
4. Kenneth O. Stanley. NEAT Software Doc File, 2001
