# Rapport

## NEAT

Outil utilisé: NEAT v1.2.1 (2010)

Topologie minimale:

- 3 neurones en entrée 
    - `bird_dist`
    - `hole_dist`
    - `hole_size`
- 1 neurone de biais
- 1 neurone de sortie
    - `x < 0.45 : se baisser`
    - `0.45 <= x <= 0.55 : ne rien faire`
    - `0.55 < x : sauter`

### Fichiers

##### mario_genes

Sample input:

```
genomestart <gen_id>
trait   <trait_id>  0 0 0 0 0 0 0 0
node    <node_id>   <trait_id>  <node_type> <node_place>
gene    <trait_id>  <inode_id>  <onode_id>  <weight>    <recur> <innov_id>  <mutation>  <enabled>
genomeend <gen_id>
```

where:

```
node_type = { 0: NEURON, 1: SENSOR }
node_place = { 0: HIDDEN, 1: INPUT, 2: OUTPUT, 3: BIAS }
```

Even though traits aren't considered, at least one trait line must be declared.


The minimal topolgy includes all input and output nodes, one bias nodes, and links with a weight of 1 from each input and bias node to each output node.

Actual input:

```
genomestart 1
trait 1 0 0 0 0 0 0 0 0
node 1 0 1 3
node 2 0 1 1
node 3 0 1 1
node 4 0 1 1
node 5 0 0 2
gene 1 1 5 0.0 0 1 0 1
gene 1 2 5 0.0 0 2 0 1
gene 1 3 5 0.0 0 3 0 1
gene 1 4 5 0.0 0 4 0 1
genomeend 1
```
