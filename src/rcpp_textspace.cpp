#include <Rcpp.h>
#include "starspace.h"

using namespace std;


// [[Rcpp::export]]
void textspace_help(std::string type = "help") {
  shared_ptr<starspace::Args> args = make_shared<starspace::Args>();
  if(type == "help"){
    args->printHelp();
  }else{
    args->printArgs();
  }
}

// [[Rcpp::export]]
Rcpp::List textspace_args(SEXP textspacemodel) {
  Rcpp::XPtr<starspace::StarSpace> sp(textspacemodel);
  // Get the list of model arguments
  std::shared_ptr<starspace::Args> args = sp->args_;
  Rcpp::List out = Rcpp::List::create(
    Rcpp::Named("file") = args->model,
    Rcpp::Named("dim") = args->dim,
    Rcpp::Named("data") = Rcpp::List::create(
      Rcpp::Named("trainMode") = args->trainMode,
      Rcpp::Named("fileFormat") = args->fileFormat,
      Rcpp::Named("trainFile") = args->trainFile,
      Rcpp::Named("validationFile") = args->validationFile,
      Rcpp::Named("testFile") = args->testFile,
      Rcpp::Named("initModel") = args->initModel,
      Rcpp::Named("useWeight") = args->useWeight
    ),
    Rcpp::Named("param") = Rcpp::List::create(
      Rcpp::Named("loss") = args->loss,
      Rcpp::Named("margin") = args->margin,
      Rcpp::Named("similarity") = args->similarity,
      Rcpp::Named("epoch") = args->epoch,
      Rcpp::Named("adagrad") = args->adagrad,
      Rcpp::Named("lr") = args->lr,
      Rcpp::Named("termLr") = args->termLr,
      Rcpp::Named("norm") = args->norm,
      Rcpp::Named("maxNegSamples") = args->maxNegSamples,
      Rcpp::Named("negSearchLimit") = args->negSearchLimit,
      Rcpp::Named("p") = args->p,
      Rcpp::Named("shareEmb") = args->shareEmb,
      Rcpp::Named("ws") = args->ws,
      Rcpp::Named("dropoutLHS") = args->dropoutLHS,
      Rcpp::Named("dropoutRHS") = args->dropoutRHS,
      Rcpp::Named("initRandSd") = args->initRandSd
    ),
    Rcpp::Named("dictionary") = Rcpp::List::create(
      Rcpp::Named("minCount") = args->minCount,
      Rcpp::Named("minCountLabel") = args->minCountLabel,
      Rcpp::Named("ngrams") = args->ngrams,
      Rcpp::Named("bucket") = args->bucket,
      Rcpp::Named("label") = args->label
    ),
    Rcpp::Named("options") = Rcpp::List::create(
      Rcpp::Named("thread") = args->thread,
      Rcpp::Named("verbose") = args->verbose,
      Rcpp::Named("debug") = args->debug,
      Rcpp::Named("maxTrainTime") = args->maxTrainTime,
      Rcpp::Named("saveEveryEpoch") = args->saveEveryEpoch,
      Rcpp::Named("saveTempModel") = args->saveTempModel,
      Rcpp::Named("validationPatience") = args->validationPatience,
      Rcpp::Named("normalizeText") = args->normalizeText,
      Rcpp::Named("trainWord") = args->trainWord,
      Rcpp::Named("wordWeight") = args->wordWeight
    ),
    Rcpp::Named("test") = Rcpp::List::create(
      Rcpp::Named("basedoc") = args->basedoc,
      Rcpp::Named("predictionFile") = args->predictionFile,
      Rcpp::Named("K") = args->K,
      Rcpp::Named("excludeLHS") = args->excludeLHS),
      Rcpp::Named("isTrain") = args->isTrain
  );
  return out;
}



// [[Rcpp::export]]
Rcpp::List textspace(std::string file = "textspace.bin",
                     /* Arguments specific for training */
                     std::string trainFile = "",
                     std::string initModel = "",
                     std::string validationFile = "",
                     /* Arguments specific for test  */
                     std::string testFile = "",
                     std::string basedoc = "",
                     std::string predictionFile = "",
                     /* Rest are the starspace defaults from Starspace/src/utils/args */
                     std::string fileFormat = "fastText",
                     std::string label = "__label__",
                     std::string loss = "hinge",
                     std::string similarity = "cosine",
                     double lr = 0.01,
                     double termLr = 1e-9,
                     double norm = 1.0,
                     double margin = 0.05,
                     double initRandSd = 0.001,
                     double p = 0.5,
                     double dropoutLHS = 0.0,
                     double dropoutRHS = 0.0,
                     double wordWeight = 0.5,
                     size_t dim = 100,
                     int epoch = 5,
                     int ws = 5,
                     int maxTrainTime = 60*60*24*100,
                     int validationPatience = 10,
                     int thread = 10,
                     int maxNegSamples = 10,
                     int negSearchLimit = 50,
                     int minCount = 1,
                     int minCountLabel = 1,
                     int bucket = 2000000,
                     int ngrams = 1,
                     int trainMode = 0,
                     int K = 5,
                     bool verbose = false,
                     bool debug = false,
                     bool adagrad = true,
                     bool normalizeText = false,
                     bool saveEveryEpoch = false,
                     bool saveTempModel = false,
                     bool shareEmb = true,
                     bool useWeight = false,
                     bool trainWord = false,
                     bool excludeLHS = false) {
  shared_ptr<starspace::Args> args = make_shared<starspace::Args>();
  args->model = file;
  /*
   * Check if it is training or testing
   */
  if(trainFile == "" && testFile == ""){
    Rcpp::stop("Either provide a training file or a test file");
  }
  if(trainFile != "" && testFile != ""){
    Rcpp::stop("Either provide a training file or a test file, not both");
  }
  if(std::ifstream(trainFile)){
    args->isTrain = true;  
    args->trainFile = trainFile;
    if(std::ifstream(initModel))      args->initModel = initModel;
    if(std::ifstream(validationFile)) args->validationFile = validationFile;
  }else if(std::ifstream(testFile)){
    args->isTrain = false;  
    args->testFile = testFile;
    if(std::ifstream(basedoc))        args->basedoc = basedoc;
    if(std::ifstream(predictionFile)) args->predictionFile = predictionFile;
  }else{
    Rcpp::stop("No valid trainFile nor testFile. Please check your path and check if the file is not opened.");
  }
  /*
   * Assign the other parameters of the modelling
   */
  args->fileFormat = fileFormat;
  args->label = label;
  args->loss = loss;
  args->similarity = similarity;
  args->lr = lr;
  args->termLr = termLr;
  args->norm = norm;
  args->margin = margin;;
  args->initRandSd = initRandSd;
  args->p = p;
  args->dropoutLHS = dropoutLHS;
  args->dropoutRHS = dropoutRHS;
  args->wordWeight = wordWeight;
  args->dim = dim;
  args->epoch = epoch;
  args->ws = ws;
  args->maxTrainTime = maxTrainTime;
  args->validationPatience = validationPatience;
  args->thread = thread;
  args->maxNegSamples = maxNegSamples;
  args->negSearchLimit = negSearchLimit;
  args->minCount = minCount;
  args->minCountLabel = minCountLabel;
  args->bucket = bucket;
  args->ngrams = ngrams;
  args->trainMode = trainMode;
  args->K = K;
  args->verbose = verbose;
  args->debug = debug;
  args->adagrad = adagrad;
  args->normalizeText = normalizeText;
  args->saveEveryEpoch = saveEveryEpoch;
  args->saveTempModel = saveTempModel;
  args->shareEmb = shareEmb;
  args->useWeight = useWeight;
  args->trainWord = trainWord;
  args->excludeLHS = excludeLHS;

  /*
   * Build and save the model
   */
  Rcpp::XPtr<starspace::StarSpace> sp(new starspace::StarSpace(args), true);
  Rcpp::List out;
  if(args->isTrain){
    if(std::ifstream(args->initModel)){
      sp->initFromSavedModel(args->initModel);
    }else{
      sp->init();  
    }
    sp->train();
    sp->saveModel(args->model);  
    out = Rcpp::List::create(
      Rcpp::Named("model") = sp,
      Rcpp::Named("args") = textspace_args(sp));
  }else{
    sp->initFromSavedModel(args->model);
    sp->evaluate();
    out = Rcpp::List::create(
      Rcpp::Named("model") = sp,
      Rcpp::Named("args") = textspace_args(sp),
      Rcpp::Named("test") = "UNDER CONSTRUCTION: capture results of sp->evaluate() or write own sp->evaluate");
  }

  /*
   * Return pointer to the model and the used arguments as a list
   */
  return out;
}


// [[Rcpp::export]]
Rcpp::List textspace_load_model(const std::string file_model) {
  shared_ptr<starspace::Args> args = make_shared<starspace::Args>();
  args->model = file_model;
  Rcpp::XPtr<starspace::StarSpace> sp(new starspace::StarSpace(args), true);
  sp->initFromSavedModel(args->model);
  Rcpp::List out = Rcpp::List::create(
    Rcpp::Named("model") = sp,
    Rcpp::Named("args") = textspace_args(sp));
  return out;
}

// [[Rcpp::export]]
Rcpp::List textspace_save_model(SEXP textspacemodel, std::string file = "textspace.tsv") {
  Rcpp::XPtr<starspace::StarSpace> sp(textspacemodel);
  sp->saveModelTsv(file);
  return file;
}


// [[Rcpp::export]]
Rcpp::NumericMatrix textspace_embedding_doc(SEXP textspacemodel, std::string input) {
  Rcpp::XPtr<starspace::StarSpace> sp(textspacemodel);
  // set useWeight by default. use 1.0 for default weight if weight is not found
  sp->args_->useWeight = true;
  // get docvector and return it as a matrix
  starspace::Matrix<starspace::Real> vec = sp->getDocVector(input, " \t");
  Rcpp::NumericMatrix embedding(vec.numRows(), vec.numCols());
  for(int i = 0; i < vec.numRows(); i++){
    for(int j = 0; j < vec.numCols(); j++){
      embedding(i, j) = vec.cell(i, j);
    }
  }
  return embedding;
}



// [[Rcpp::export]]
int textspace_embedding_ngram(SEXP textspacemodel, std::string input) {
  Rcpp::XPtr<starspace::StarSpace> sp(textspacemodel);
  if (sp->args_->ngrams <= 1) {
    Rcpp::stop("Error: your provided model does not use ngram > 1.\n");
  }
  auto vec = sp->getNgramVector(input);
  cout << input;
  for (auto v : vec) { cout << "\t" << v; }
  cout << endl;
  return 1;
}