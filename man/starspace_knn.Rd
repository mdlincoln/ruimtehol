% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/embed-all-the-things.R
\name{starspace_knn}
\alias{starspace_knn}
\title{K-nearest neighbours using a Starspace model}
\usage{
starspace_knn(object, newdata, k = 5, ...)
}
\arguments{
\item{object}{an object of class \code{textspace} as returned by \code{\link{starspace}} or \code{\link{starspace_load_model}}}

\item{newdata}{a character string of length 1}

\item{k}{integer with the number of nearest neighbours}

\item{...}{not used}
}
\value{
a list with elements input and a data.frame called prediction which has columns called label and prob
}
\description{
K-nearest neighbours using a Starspace model
}
