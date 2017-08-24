#pragma once
#include <vector>
#include "som_def.h"

class In_Out
{
public:
  static double scale_value(double x, double min, double max, double a, double b);
  static vector<double> scale_values(const vector<double> &in_column_vector);
  static void import_data(const char *source, vector<vector<double>> &out_scaled_data);
  static vector<Merkmal> get_merkmal(const vector<vector<double>> &in_values, const vector<string> &in_header);
};