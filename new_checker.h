#ifndef NEW_CHECKER_H
#define NEW_CHECKER_H

#include "data_structure.h"
#include "invsolver.h"
#include "startsolver.h"
#include "mainsolver.h"
#include "model.h"
#include <assert.h>
#include "utility.h"
#include "statistics.h"
#include <fstream>
#include <algorithm>

#define MAX_SOLVER_CALL 500
#define MAX_TRY 4

namespace newcar 
{
    class Checker
    {
    public:
        Checkr (Model* model, Statistics& stats, std::ofstream* dot, bool evidence = false,bool verbose = false);
        ~Checker ();

        bool check (std::ofstream&);

    protected:
        //flags
        bool evidence_;
        bool verbose_;

        //members
        Statistics *stats_;
        std::ofstream *dot_;
        int solver_call_counter_; //counter for solver_ calls
		int start_solver_call_counter_; //counter for start_solver_ calls
		
        State *init_; //start state
        int bad_;
        
        Model *model_;
        MainSolver *solver_;
		StartSolver *start_solver_;
		InvSolver *inv_solver_;

        std::vector<State*> states_set; //states set to record traces
        int state_set_index; //index of current states set
        void print_evidence (std::ofstream&);
        void new_car_initialization ();
		void new_car_finalization ();
		void destroy_states ();
		bool new_car_check ();
		std::vector<int> get_partial(); 		



        //inline function
        inline void create_inv_solver (){
			inv_solver_ = new InvSolver (model_, verbose_);
		}
		inline void delete_inv_solver (){
			delete inv_solver_;
			inv_solver_ = NULL;
		}
		
		inline void reset_start_solver (){
	        assert (start_solver_ != NULL);
	        start_solver_->reset ();
	    }
	    
	    inline bool reconstruct_start_solver_required () {
	        start_solver_call_counter_ ++;
	        if (start_solver_call_counter_ == MAX_SOLVER_CALL) {
	            start_solver_call_counter_ = 0;
	            return true;
	        }
	        return false;
	    }
	    
	    inline void reconstruct_start_solver () {
	        delete start_solver_;
	        start_solver_ = new StartSolver (model_, bad_, forward_, verbose_);
	        for (int i = 0; i < frame_.size (); i ++) {
	            start_solver_->add_clause_with_flag (frame_[i]);
	        }
	    }
	    
	    inline bool start_solver_solve_with_assumption (){
	        if (reconstruct_start_solver_required ())
	            reconstruct_start_solver ();
	            
	        stats_->count_start_solver_SAT_time_start ();
	    	bool res = start_solver_->solve_with_assumption ();
	    	stats_->count_start_solver_SAT_time_end ();
	    	return res;
	    }
	    
	    inline bool reconstruct_solver_required () {
	        solver_call_counter_ ++;
	        if (solver_call_counter_ == MAX_SOLVER_CALL) {
	            solver_call_counter_ = 0;
	            return true;
	        }
	        return false;
	    }
	    
	    inline void reconstruct_solver () {
	        delete solver_;
	        MainSolver::clear_frame_flags ();
	        solver_ = new MainSolver (model_, stats_, verbose_);
	        for (int i = 0; i < F_.size (); i ++) {
	            solver_->add_new_frame (F_[i], i, forward_);
	        }
	    }
	    
	    inline bool solver_solve_with_assumption (const Assignment& st, const int p){
	        if (reconstruct_solver_required ())
	            reconstruct_solver ();
	        Assignment st2 = st;
	        add_intersection_last_uc_in_frame_level_plus_one (st2, -1);
	        stats_->count_main_solver_SAT_time_start ();
	        bool res = solver_->solve_with_assumption (st2, p);
	        stats_->count_main_solver_SAT_time_end ();
	        if (!res) {
	        	Assignment st3; 
		    	st3.reserve (model_->num_latches());
		    	for (int i = st2.size ()-model_->num_latches(); i < st2.size (); ++ i)
		    		st3.push_back (st2[i]);

	            if (0 < cubes_.size ()) 
		            cubes_[0] = st3;
		        else
		            cube_ = st3;
	        }
	        return res;
	    }
	    }
	    
    }











}
