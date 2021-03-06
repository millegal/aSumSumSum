#include "types.hpp"
#include "set.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>

namespace com_b_velop
{
  Set::Set(const std::vector<std::vector<double>> &in_values) : values(nullptr){
    this->SetSet(in_values);
  }
  Set *Set::SetSet(const std::vector<std::vector<double>> &in_values){
    this->InitValues(in_values.size(), in_values[0].size());
    for (size_t row = 0; row < in_values.size(); ++row)
      for (size_t col = 0; col < in_values[0].size(); ++col)
        this->values[row][col] = in_values[row][col];
    return this;
  }
  Set::~Set(){
    this->DeallocValues();
	  std::cout << "Set (" << this << ") killed" << std::endl;
  }
  Set *Set::SaveSet(const char *in_destination){
    std::cout << "Store Set to: " << in_destination << std::endl;
	  std::ofstream fs;
    fs.open(in_destination, std::ios::out | std::ios::binary);
    if (fs.is_open())
    {
      fs.write((char *)&this->rows, sizeof(this->rows));
      fs.write((char *)&this->cols, sizeof(this->cols));
      this->SaveSetInfo(fs);
      for (size_t row = 0; row < this->rows; ++row)
        for (size_t col = 0; col < this->cols; ++col)
          fs.write((char *)&this->values[row][col], sizeof(this->values[row][col]));
      fs.close();
    }
    else{
      std::cerr << "I/O Error - can't write Set to Filesystem" << std::endl;
    }
    return this;
  }
  Set *Set::OpenSet(const char *in_source){
    // std::cout << "Open Set: " << in_source << std::endl;
    auto set = new Set();
	  std::ifstream fs;
    fs.open(in_source, std::ios::in | std::ios::binary);
    size_t tmp_rows = 0;
    size_t tmp_cols = 0;
    SetInfo info;
    if (fs.is_open())
    {
      fs.read((char *)&tmp_rows, sizeof(tmp_rows));
      fs.read((char *)&tmp_cols, sizeof(tmp_cols));
      info = SetInfo::OpenSetInfo(fs);
      set->InitValues(tmp_rows, tmp_cols);
      for (size_t row = 0; row < set->rows; ++row)
        for (size_t col = 0; col < set->cols; ++col)
          fs.read((char *)&set->values[row][col], sizeof(set->values[row][col]));
      fs.close();
    }
    set->features_size = info.features_size;
    set->features = info.features;
    return set;
  }

  Set *Set::InitValues(const size_t &in_rows, const size_t &in_cols){
    this->values = static_cast<double **>(malloc(sizeof(double *) * in_rows));
    for (size_t row = 0; row < in_rows; ++row)
    {
      this->values[row] = static_cast<double *>(malloc(sizeof(double) * in_cols));
      for (size_t col = 0; col < in_cols; ++col)
        this->values[row][col] = 0;
    }
    this->rows = in_rows;
    this->cols = in_cols;
    // cout << "Process init_values ready" << endl;
    return this;
  }
  Set *Set::DeallocValues(){
		if (this->values != nullptr)
		{
			for (size_t row = 0; row < rows; ++row)
				delete[] this->values[row];
			delete[] this->values;
		}
    rows = 0;
    cols = 0;
    values = nullptr;
	  std::cout << "Dealloc Process ready" << std::endl;
    return this;
  }
  Set *Set::SplitValues(const unsigned short &in_percent){
    auto new_set = new Set();
    auto tmp_set = new Set();
    std::random_device rd;
    std::mt19937 g(rd());

    // Zufällige Indices
	  std::vector<size_t> indices;
    for (size_t i = 0; i < this->rows; ++i)
      indices.push_back(i);
    std::shuffle(indices.begin(), indices.end(), g);

    // Berechnen wie groß die neuen Sets werden
    double factor = in_percent / 100.0;
    size_t new_rows = static_cast<size_t>(this->rows * factor);
    size_t old_rows = this->rows - new_rows;
    size_t tmp_cols = this->cols;

    // Temporäres- und Zielset erzeugen
    tmp_set->InitValues(old_rows, tmp_cols);
    new_set->InitValues(new_rows, tmp_cols);

    // Features übertragen
    new_set->features = this->features;

    // Werte ins Zielset schreiben
    for (size_t row = 0; row < new_rows; ++row)
      for (size_t col = 0; col < tmp_cols; ++col)
        new_set->values[row][col] = this->values[indices[row]][col];

    // Werte ins Temporäre Set schreiben
    for (size_t row = 0; row < old_rows; ++row)
      for (size_t col = 0; col < tmp_cols; ++col)
        tmp_set->values[row][col] = this->values[indices[row + new_rows]][col];

    // Altes Set löschen
    this->DeallocValues();

    // Neues Set kopieren
    this->InitValues(old_rows, tmp_cols);
    for (size_t row = 0; row < old_rows; ++row)
      for (size_t col = 0; col < tmp_cols; ++col)
        this->values[row][col] = tmp_set->values[row][col];

    // Temporäres Set löschen
    delete tmp_set;

    // Rückgabe vom abgesplitteten Teil
    return new_set;
  }
}; // namespace com_b_velop
