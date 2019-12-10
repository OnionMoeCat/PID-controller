#include "PID.h"

#include <iostream>

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
  
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
   
  int_cte = 0.0;
  prev_cte = 0.0;
  
  first_frame = true;
}

void PID::UpdateError(double cte) {
  
  // p error
  p_error = cte * Kp;  
  
  // d error
  if (first_frame)
  {
    first_frame = false;
    prev_cte = cte;
  }  
  d_error = (cte - prev_cte) * Kd;
  prev_cte = cte;
  
  
  // i error
  int_cte += cte;
  i_error = int_cte * Ki;
  
  std::cout << "(-) p error: " << - p_error << " (-) i error: " << - i_error << " (-) d error: " << - d_error <<  " total error: " << TotalError() << std::endl;  
}

double PID::TotalError() {
  return - (p_error + i_error + d_error);
}