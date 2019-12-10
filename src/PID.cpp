#include "PID.h"

#include <cmath>
#include <iostream>
#include <limits>

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
  
  // Twiddling parameters
  yes_i_wanna_twiddle = false;
  dp = {0.1*Kp,0.1*Kd,0.1*Ki};
  step = 1;
  param_index = 2;  // this will wrao back to 0 after the first twiddle loop
  n_settle_steps = 100;
  n_eval_steps = 2000;
  total_error = 0;
  best_error = std::numeric_limits<double>::max();
  tried_adding = false; 
  tried_subtracting = false;
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
  
  //std::cout << "(-) p error: " << - p_error << " (-) i error: " << - i_error << " (-) d error: " << - d_error <<  " total error: " << TotalError() << std::endl;  
  
  // update total error only if we're past number of settle steps
  if (step % (n_settle_steps + n_eval_steps) > n_settle_steps){
      total_error += pow(cte,2);
  }

  // last step in twiddle loop... twiddle it?
  if (yes_i_wanna_twiddle && step % (n_settle_steps + n_eval_steps) == 0){
     std::cout << "step: " << step << std::endl;
     std::cout << "total error: " << total_error << std::endl;
     std::cout << "best error: " << best_error << std::endl;
     if (total_error < best_error) {
        std::cout << "improvement!" << std::endl;
        best_error = total_error;
        if (step !=  n_settle_steps + n_eval_steps) {
        	// don't do this if it's the first time through
            dp[param_index] *= 1.1;            
        }
        // next parameter
        param_index = (param_index + 1) % 3;
        tried_adding = tried_subtracting = false;
     }
     if (!tried_adding && !tried_subtracting) {
        // try adding dp[i] to params[i]
        AddToParameterAtIndex(param_index, dp[param_index]);
        tried_adding = true;
     }
     else if (tried_adding && !tried_subtracting) {
        // try subtracting dp[i] from params[i]
		AddToParameterAtIndex(param_index, -2 * dp[param_index]);     
        tried_subtracting = true;         
     }
     else {
        // set it back, reduce dp[i], move on to next parameter
		AddToParameterAtIndex(param_index, dp[param_index]);      
        dp[param_index] *= 0.9;
        // next parameter
        param_index = (param_index + 1) % 3;
        tried_adding = tried_subtracting = false;
     }
     total_error = 0;
     std::cout << "new parameters" << std::endl;
     std::cout << "P: " << Kp << ", I: " << Ki << ", D: " << Kd << std::endl;
  }
  step++;
}

void PID::AddToParameterAtIndex(int index, double amount) {
    if (index == 0) {
        Kp += amount;
    }
    else if (index == 1) {
        Kd += amount;
    }
    else if (index == 2) {
        Ki += amount;
    }
    else {
        std::cout << "AddToParameterAtIndex: index out of bounds";
    }
}

double PID::TotalError() {
  return - (p_error + i_error + d_error);
}