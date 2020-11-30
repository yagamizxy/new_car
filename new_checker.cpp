#include"new_checker.h"
#include <vector>
#include <iostream>
#include "utility.h"
#include "statistics.h"
#include <algorithm> //zxy add this
using namespace std;

namespace newcar
{
    ////////////main function//////////
    bool Checker::check (std::ofstream& out){
        for (int i = 0; i < model_->num_outputs (); i ++){
	        bad_ = model_->output (i);
	        
	        //for the particular case when bad_ is true or false
	        if (bad_ == model_->true_id ()){
	        	out << "1" << endl;
	        	out << "b" << i << endl;
	        	if (evidence_){
	        	    //print init state
	        	    out << init_->latches() << endl;
	        	    //print an arbitary input vector
	        	    for (int j = 0; j < model_->num_inputs (); j ++)
	        	        out << "0";
	        	    out << endl;
	        	}
	        	out << "." << endl;
	        	if (verbose_){
	        		cout << "return SAT since the output is true" << endl;
	        	}
	        	return true;
	        }
	        else if (bad_ == model_->false_id ()){
	        	out << "0" << endl;
	        	out << "b" << endl;
	        	out << "." << endl;
	        	if (verbose_){
	        		cout << "return UNSAT since the output is false" << endl;
	        	}
	        	return false;
	        }
	        
	        new_car_initialization ();
	        bool res = new_car_check ();   //key step of checking,1 stands for counterexample found 
	        if (res)                        
    			out << "1" << endl;
   			else
    			out << "0" << endl;
    		out << "b" << i << endl;
   			if (evidence_ && res)
    			print_evidence (out);
    		out << "." << endl;
	        new_car_finalization ();
	        return res;
	    }
    }

	bool Checker::new_car_check (){
		if (verbose_) cout<<"start checking"<<endl;
		if ()

	}

    
    ////help function/////
    void Checker::print_evidence (ofstream& out) {
		if (forward_)
			init_->print_evidence (forward_, out);
		else
			last_->print_evidence (forward_, out);
	}

	void Checker::new_car_initialization ()
	{
	    solver_ = new MainSolver (model_, stats_, verbose_);
		start_solver_ = new StartSolver (model_, bad_, forward_, verbose_);
	}








}