#ifndef CSKECTH_H
#define CSKECTH_H

#include <iostream>
#include <algorithm>
#include <cstring>
#include <math.h>
#include "hasher.hpp"


class Sketch{

 protected:
  unsigned no_rows;
  unsigned no_cols;
  StrHash* hashes;
  long long int* table;
  double time;
  
  Sketch(double epsilon, double delta){
    no_rows = (log(1 / delta) / log(2));
    no_cols = (2 / epsilon);
    
    //prime column count
    bool found = false;
    
    while(!found){
      found = true;
      
      for(unsigned i = 2; i <= sqrt(no_cols); i++){
	if(no_cols % i == 0){
	  found = false;
	  no_cols++;
	}
      }
    }

    table = new long long int[this->no_rows * this->no_cols];
    
    hashes = new StrHash[no_rows];
    for(unsigned i = 0; i < no_rows; i++){
      hashes[i].set_parameters(no_cols);
    }

    time = 0;
    
    std::cout << "A sketch with " << this->no_rows << " rows and " << this->no_cols << " columns is created" << std::endl;    
  }

 public:

  long long int get(const unsigned& i, const unsigned& j) const {
    return table[i * no_cols + j];
  }

  void add(const unsigned& i, const unsigned& j, const int& val){
    table[i * no_cols + j] += (long long int)val;
  }

  virtual void insert_to_row(const std::string& data, unsigned row_id) = 0;
  virtual void insert(const std::string& data) = 0;
  virtual long long int query(const std::string& data) const = 0;

  virtual std::string name(){
    return "Sketch";
  }

  void reset(){
    memset(table, 0, sizeof(unsigned)*no_rows*no_cols);
  }

  unsigned get_no_rows(){
    return no_rows;
  }

  unsigned get_no_cols(){
    return no_cols;
  }

  double getError(std::pair<std::string, int> freqs[]){
    double err = 0.0;

    for(int i = 0; i < 1000; i++){
      std::string curr = freqs[i].first;
      long long int actual = freqs[i].second;
      long long int guess = query(curr);

      double cerr = ((double)(abs(guess-actual)) / actual);
      err += cerr * cerr;
    }
    return sqrt(err / 1000);
  }
  
  void print() {
    for(unsigned i = 0; i < no_rows; i++) {
      for(unsigned j = 0; j < no_cols; j++) {
	std::cout << get(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }
  

  void add_to_time(double partial){
    this->time += partial;
  }

  double get_time(){
    return this->time;
  }

  void reset_time(){
    this->time = 0;
  }
  
  ~Sketch(){
    delete[] table;
    delete[] hashes;
  }
};

class FreqSketch: public Sketch{
protected:
  FreqSketch(double epsilon, double delta, int id):Sketch(epsilon,delta){
    //TO DO: IMPLEMENT DERIVED SKETCHES
