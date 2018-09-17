# ruimtehol: R package to Embed All the Things! using StarSpace

This repository contains an R package which wraps the StarSpace C++ library (https://github.com/facebookresearch/StarSpace), allowing the following:

- Text classification
- Learning word, sentence or document level embeddings
- Finding sentence or document similarity
- Ranking web documents
- Content-based or Collaborative filtering-based Recommendation, e.g. recommending music or videos.

## Installation

This package is still under heavy construction and subject to major changes. It is not mature yet for CRAN release, you can only install it as follows:

- For installing the development version of this package: `devtools::install_github("bnosac/ruimtehol", build_vignettes = TRUE)`


#### Short example


```r
library(ruimtehol)

## Get some training data
download.file("https://s3.amazonaws.com/fair-data/starspace/wikipedia_train250k.tgz", "dev/wikipedia_train250k.tgz")
x <- readLines("dev/wikipedia_train250k.tgz", encoding = "UTF-8")
x <- x[sample(x = length(x), size = 100)]
writeLines(text = x, sep = "\n", con = "dev/wikipedia_train100.txt")
```

```r
## Train
model <- starspace(trainFile = "dev/wikipedia_train100.txt", fileFormat = "labelDoc", dim = 10, trainMode = 3)
model

Object of class textspace
 model saved at textspace.bin
 size of the model in Mb: 0.87
 dimension of the embedding: 10
 training arguments:
      loss: hinge
      margin: 0.05
      similarity: cosine
      epoch: 5
      adagrad: TRUE
      lr: 0.01
      termLr: 1e-09
      norm: 1
      maxNegSamples: 10
      negSearchLimit: 50
      p: 0.5
      shareEmb: TRUE
      ws: 5
      dropoutLHS: 0
      dropoutRHS: 0
      initRandSd: 0.001
```

```r
## Save trained model as TSV so that you can inspect the embeddings e.g. with data.table::fread("wikipedia_embeddings.tsv")
starspace_save_model(model, file = "wikipedia_embeddings.tsv")

## Load a pre-trained model
model <- starspace_load_model("textspace.bin")

## Get the document embedding
starspace_embedding(model, "The apps to predict / get nearest neighbours are still under construction.")

           [,1]      [,2]        [,3]       [,4]       [,5]       [,6]       [,7]     [,8]      [,9]      [,10]
[1,] -0.4213823 0.4987145 -0.08066317 -0.6519815 -0.1743725 0.09401496 0.02670185 0.262726 0.1761705 0.04599866
```

## Notes

- Why did you call the package ruimtehol? Because that is the translation of StarSpace in WestVlaams.
- The R wrapper is distributed under the Mozilla Public License 2.0. The package contains a copy of the StarSpace C++ code (namely all code under src/Starspace) which has a BSD license (which is available in file LICENSE.notes) and also has an accompanying PATENTS file which you can inspect [here](inst/PATENTS).

## Support in text mining

Need support in text mining?
Contact BNOSAC: http://www.bnosac.be