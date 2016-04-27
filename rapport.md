# NEAT Ninja Mario

---

- **Sujet:** Algorithmes génétiques et réseaux de neurones sur un jeu de plateforme
- **Contexte:** Présenté à Pr. Éric Beaudry dans le cours INF4230 (Hiver 2016)
- **Équipe:** #3
- **Membres:** Alexandre Cayer-Boucher et John Béjot
- **Date:** 4 mai 2016

---

## 1. Introduction

L'application que nous avons décidé de résoudre est un simple jeu de plate-forme dynamique (en déplacement continu). Les jeux de plateformes sont une cible intéressante pour l'apprentissage machine car la stratégie requise est minime, avec une emphase sur les réflexes. Le temps requis pour qu'un agent apprenne à jouer est moindre comparé à d'autres jeux offrant plus de possibilités d'action, et l'apprentissage peut s'effectuer de manière non-supervisée. 

Destinés à être joués par des individus humains, seuls, il est rare de voir des agents intelligents jouer à des jeux de plateforme. Les agents intelligents vont plutôt être sollicités dans des jeux offrant la possibilité de compétition. Néanmoins, il est très intéressant de voir comment une machine s'y prendrait pour ce type de jeu: son comportement convergera-t-il vers le notre ? Ou peut-être sommes nous coincés dans un optimum local, duquel les agents intelligents ont su sortir ? 

Dans notre jeu, Mario doit sauter par dessus des trous et éviter des oiseaux en se baissant, afin de survivre dans l'environnement hostile dans lequel il est coincé. Son score final est fonction directe du temps qu'il a survécu, indépendemment du nombre d'obstacles évités. La collision avec un seul obstacle, trou ou oiseau, entraine une mort dans d'affreuses souffrances. Mario ignore malheureusement que la partie ne prendra fin qu'à sa mort, c'est à dire qu'il ne peut pas gagner.

---

## 2. Problématique

Pour une configuration quelconque de l'environnement, le joueur artificiel devra procéder de facon optimale: dans ce contexte, c'est-à-dire effectuer une action qui ne résultera pas directement en la mort. Les obstacles sont générés de facon aléatoire, et en se basant sur un tuple de paramètres simples _\<distance du trou, taille du trou, distance oiseau\>_, l'agent devra retourner une action qui lui assurera la survie. C'est à la discrétion de l'agent de prévoir toutes les combinaisons possibles de paramètres.

L’environnement de notre problème est : 

- Partiellement Observable\*
- Agent Seul
- Déterministe
- Episodique 
- Dynamique
- Continu

\* À n'importe quel moment donné, l'agent dispose de toutes les informations qu'il lui faut pour survivre (son champ de vision est l'écran, ce qu'il se passe en dehors de l'écran ne peut en aucun cas l'affecter au temps `t`). Cependant, les obstacles sont générés aléatoirement et à l'infini, donc l'environnement au complet n'est pas entièrement observable.

---

## 3. Technique d'IA retenue

Les caractéristiques de notre environnement, précédemment cités, ainsi que le besoin de la problématique nous ont conduit vers une réponse évidente et immédiate: des **réseaux neuronaux avec un algorithme génétique**. Pour clarifier, la réponse "évidente" l'est depuis notre point de vue, avec les connaissances acquises pendant le cours d'Intelligence Artificielle; peut-être que certains seraient arrivés à une conclusion différente.

Le choix d'algorithme génétique avec réseaux neuronaux (abrégé *AGRN*) sera expliqué sous peu, mais avant tout, et principalement à des fins spéculatives, nous passerons en revue certaines techniques étudiées en cours et essayerons de les appliquer à notre problème.

Techniques | Critiques
---|---
Recherches Locales / Chemin | Taille de l'espace d'états colossal (variables continues), qui doit être recalculé extrêmement souvent (observabilité partielle)
CSP | Il n'y a pas de "configuration gagnante" dans ce jeu
Processus Stochastiques | Les actions sont déterministes, et la probabilité de mourir en touchant un obstacle est toujours 1, donc aucune utilité ici

Dans l'idéal, il nous faudrait une technique **capable d'apprendre** et de **gérer des variables continues**. Allons voir ce que peut nous apporter les AGRN dans notre problémantique. Bien que souvent couplés, ces deux outils n'ont pas la même fonction, et agissent de manière complémentaire:

- Un **réseau neuronal** est une entité qui va affecter directement la décision. En fonction de paramètres donnés, une simulation neuronale répondant à des lois mathématiques va retourner un certain résultat (ici, une seule valeur). Les réseaux neuronaux (abrégé *RN*) sont capables d'apprentissage: leur structure interne peut être modifiée afin de changer la valeur de sortie pour un jeu d'entrées donné. Dans notre cas, un RN efficace retournera par exemple une valeur entrainant l'action "se baisser" lorsque l'input désignant la distance de l'oiseau descend en dessous d'un certain seuil. Les RN ne calculent pas d'espace d'états, et computent les nombres réels de la même manière que les nombres naturels.

- Pour faire évoluer efficacement des RN, on peut faire appel à un **algorithme génétique** (abrégé *AG*). L'AG ne va pas directement influencer la décision: il a seulement pour but d'améliorer les RN, tout en conservant leurs caractéristiques prisées. Par exemple, un RN qui sait se baisser lorsqu'un oiseau s'approche, va maintenant vouloir apprendre à sauter par dessus les trous, et cela sans oublier de se baisser au moment opportun ! C'est donc l'AG qui permet aux RN d'apprendre. Basé sur la théorie de l'évolution de Darwin, un AG va typiquement traiter une population de RN, en les faisant évoluer sur plusieurs générations à travers des méchanismes plus ou moins complexes, et qui seront expliqués dans une prochaine partie. En manipulant plusieurs RN plutôt qu'un, et en modifiant leur structure interne, l'AG va en quelques sortes permettre l'apparition de RN "champions", c'est à dire les RN les plus aptes à survivre dans leur environnement, i.e. ceux qui obtiennent le meilleur score au jeu.

---

## 4. Particularités et Implémentation

Les particularités propres à ce projet viennent de NEAT. Les librairies AGRN requièrent généralement la spécification de la topologie des RN, et vont soit jouer uniquement sur les poids entre les noeuds, soit altérer la topologie. Outre la difficulté de choisir une structure appropriée (qui peut aussi être générée au hasard), une conséquence directe est l'abondance de noeuds qui pourraient retarder l'apprentissage du RN, et même lui être contre-productive. En effet, chaque noeud en plus rajoute un poid dans l'équation, et si un noeud est superflu, il nécessitera beaucoup d'efforts pour "isoler" son influence.

L'avantage de NEAT réside du fait que son point de départ est une topologie minimale, i.e. avec les entrées connectées directement aux sorties. Au fur et à mesure, des noeuds et des liens vont se rajouter, augmentant ainsi la topologie, comme son nom l'indique. Cette méthode vise à éliminer la redondance précédemment citée, en gardant une structure minimale en tout temps: un neurone apparait et survit dans la topologie seulement s'il est nécessaire à la survie du réseaux, et donc que son influence est positive à la synergie globale. De plus, cela permet aussi d'alléger les calculs dans les premières générations par rapport aux RN des autres libraries qui seront déjà lourdement complexes.

Un autre point majeur concernant NEAT est sa spéciation. Derrière ce mot se cache une technique pour permettre à l'innovation de se développer, et de survivre au moins le temps de devenir utile. Par exemple, les premiers animaux à avoir développé des ailes n'ont pas été les premiers à voler car leurs ailes devaient être petites et faibles. Mais en permettant à ces futurs oiseaux de continuer d'étendre leur ailes, c'est à dire en permettant à cette innovation de se développer, de nouvelles espèces ont pu dominer leur environnement grâce à leur capacité de voler. La spéciation permet à la recherche de s'effectuer dans différents espaces simultanément, et donc d'éviter que tous convergent vers un optimum local.

### 4.1 Détails d’implémentation

Étant donné l'approche d'augmentation de topologie de NEAT, il n'a pas été nécessaire de concevoir une structure RN à l'avance: la topologie minimale a été à la base de tous les tests. Là où il a été intéressant de changer les valeurs, fut dans le fichier de paramètres: généralement sous formes de coefficients, ses paramètres définissent la manière dont les RN vont changer au cours des générations. On a entre autres le seuil de compatibilité, le seuil de survie, la probabilité de mutation, les coefficients de compatibilité etc.

Le processus est vraiment simple: pour tous les organismes d'une population, i.e. pour chaque RN, on lui fait essayer le jeu et on note son score (fitness). Un RN "joue" en chargeant les inputs dans ses senseurs, puis en s'activant pour retourner la valeur computée. En fonction de cette valeur, telle ou telle action va être effectuée. Une fois ces les fitness de chaque membre recensés, on demande à NEAT de faire évoluer la population courrante, c'est à dire d'engendrer la génération suivante. On répète tout ce processus X * Y fois, X étant le nombre de générations voulues et Y le nombre d'essais. 

### 4.2 Code externe

En fin de compte, le travail fourni a été grandement investi dans la recherche: une fois NEAT compris, il a seulement suffit de le brancher au jeu pour que tout marche. À part le lien etre NEAT et le jeu, tout le reste est du code externe:

- le jeu a été développé en C++ par Jérémy Goussé, un ami à John. Il utilise les librairies:
    - Boost, pour tous les "raccourcis" C++
    - SFML, pour faire le rendu graphique
- l'outil NEAT a été développé en C++ par Kenneth Stanley, un professeur à la University of Central Florida

---

## 5 Expérimentation, résultats et limites

Le jeu étant infini, nous avons posé comme limite de victoire le score de 1000. Si un RN réussisait à faire 1000 ou plus, alors on lui descernait l'appelation de "Ninja". Avec plusieurs jeux de données différents, notre programme a toujours réussi à faire évoluer des "Ninja". L'apprentissage a donc été un succès dans 100% des cas.

En partant tous d'une topologie minimale, il serait intéressant de comparer deux populations en fonction du nombres de générations qu'il leur a fallut pour arriver au stade Ninja. Dans un cas d'égalité, on pourrait regarder leur topologie et récompenser la minimalité.

### Expérimentation

En fonction des paramètres NEAT, la génération Ninja a été obtenue plus ou moins rapidement.

INSERER DONNEES ICI !!!!

### Résultats

Un constat intéressant a été de remarquer 2 écoles systématiques:

1. des Mario qui sautillent tout le temps, et donc qui peuvent par chance éviter certains trous, mais se prennent le 1er oiseau
2. des Mario qui ne gardent que la tête baissée, et donc qui évitent tous les oiseaux mais se prennent le 1er trou

Une compétition a eut lieu entre ces deux groupes: le premier a apprendre la compétence de l'autre pourrait devenir l'espèce dominante. Le groupe 1 a compris en premier qu'il fallait se baisser en présence d'un oiseau, mais ces sauts répétitifs lui ont valu de tomber dans un trou tôt ou tard ; n'étant pas possible de faire de doubles sauts, pour éviter un trou il faut être à terre pour sauter au bon moment, et pour sauter au bon moment il faut ne pas sauter tout le temps. Le groupe 1 devait donc désapprendre le saut en quelques sortes. C'est pour cette raison que le groupe 2 finissait toujours par générer les Ninja en premier, car il lui suffisait juste d'apprendre à sauter au bon moment étant donné qu'il évitait déjà les oiseaux. 


### Limites 

Le jeu reste assez basique, avec seulement 3 commandes. On a remarqué que les RN convergeaient tous vers un même style de jeu ; il n'y a pas vraiment place à l'innovation. Un jeu plus complexe aurait permis de voir plusieurs stratégies IA se développer, et les mettre en compétition aurait été un bon moyen de les comparer.

---
   
## 6 Conclusion

NEAT a été un excellent choix pour notre projet. Les fonctionnalités qu'il offre, comparé à d'autres librairies AGRN, sont véritablement remarquables. Dans 100% des cas, NEAT a trouvé une solution au problème. La simplicité du jeu a grandement joué, la complexification de celui-ci pourrait rendre les choses plus intéressantes.

---

## 7 Répartition des tâches

Nous avons tous les deux fait des lectures intensives sur les réseaux neuronaux et les algorithmes génétiques afin de nous aiguiller vers une méthode qui nous intéressait. Notre choix est tombé sur NEAT, qui a été recommandé par le professeur, mais que nous avons aussi retrouvé lors de nos recherches. Nous avons ensuite lu des publications sur NEAT, la théorie qui se cache derrière, comment il se compare aux autres AGRN, et comment se servir du code.

Le code, découlant des recherches effectuées ensemble, a été formulé en groupe mais écrit par John, plus à l'aise avec le C++. Pendant ce temps, Alexandre préparait les slides de présentation. Les spéculations sur NEAT, les révisions de code, les interrogations sur ARGN, et les simulations de notre IA ont été effectué en groupe. La rédaction du document s'est faite par les deux membres de l'équipe.

Notre base de connaissance était complètement partagée, et à tout moment, chacun savait ce que faisait l'autre, et était capable de le faire soi-même. Les tâches ont été équitablement réparties, toute la reflexion s'est faite en équipe et chacun s'est investi autant que l'autre pour ce projet.

