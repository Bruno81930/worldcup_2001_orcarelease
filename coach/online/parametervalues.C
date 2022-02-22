// This file contains classes for server_param, player_param and player_type

#ifndef PARAMETERVALUES
#define PARAMETERVALUES
#include "parametervalues.h"
#endif
#include <iostream.h>
#include <stdio.h>

extern int verbose;

Server_param::Server_param(char * message){
	sscanf(message, "(server_param %lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf "
      				      "%lf %lf %lf %lf %lf %d "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf "				  
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %d %lf %lf "
				      "%d %d %d %d %d "
				      "%d %d %d %d %d "
				      "%d %d %d %d %d "
				      "%d %d %d %d %d "	
				      "%d %d %d %d %d "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf %lf "
				      "%d %d %d %d)",
	// & variables
	& gwidth , 					
	& inertia_moment , 	
	& psize , 			
	& pdecay , 			
	& prand , 			
	& pweight , 				
	& pspeed_max , 		
	& paccel_max , 		
	& stamina_max , 			
	& stamina_inc , 			
	& recover_init , 		
	& recover_dthr , 		
	& recover_min , 			
	& recover_dec , 			
	& effort_init , 			
	& effort_dthr , 			
	& effort_min , 		
	& effort_dec , 			
	& effort_ithr , 			
	& effort_inc , 			
	& kick_rand ,                             
	& team_actuator_noise ,                     
	& prand_factor_l ,                          
	& prand_factor_r ,                            
	& kick_rand_factor_l ,                       
	& kick_rand_factor_r ,                     
	& bsize , 			
	& bdecay , 			
	& brand , 			
	& bweight , 				
	& bspeed_max , 			
	& baccel_max , 			
	& dprate , 			
	& kprate , 			
	& kmargin , 				
	& ctlradius , 			
	& ctlradius_width , 		
	& maxp ,				
	& minp , 				
	& maxm , 				
	& minm , 				
	& maxnm , 				
	& minnm , 				
	& maxn , 				
	& minn , 				
	& visangle , 			
	& visdist , 					
	& windir , 					
	& winforce , 				
	& winang , 					
	& winrand , 					
	& kickable_area , 			
	& catch_area_l , 			
	& catch_area_w , 			
	& catch_prob , 				
	& goalie_max_moves ,                  	
	& ckmargin , 				
	& offside_area , 
			
	& win_no , 					
	& win_random , 				
	& say_cnt_max , 				
	& SayCoachMsgSize , 				
	& clang_win_size ,
	& clang_define_win ,
	& clang_meta_win ,
	& clang_advice_win ,
	& clang_info_win ,
	& clang_mess_delay ,
	& clang_mess_per_cycle ,
	& half_time , 					
	& sim_st , 					
	& send_st , 					
	& recv_st , 					
	& sb_step , 					
	& lcm_st , 		                        
	& SayMsgSize , 				
	& hear_max , 					
	& hear_inc , 					
	& hear_decay , 				
	& cban_cycle , 				
	& slow_down_factor ,                           
	& useoffside , 				
	& kickoffoffside , 			

	& offside_kick_margin , 		
	& audio_dist , 				
	& dist_qstep , 				
	& land_qstep , 
	& dir_qstep , 				
	& dist_qstep_l , 			
	& dist_qstep_r , 			
	& land_qstep_l , 			
	& land_qstep_r , 			
	& dir_qstep_l , 				
	& dir_qstep_r ,
	& CoachMode , 				
	& CwRMode , 					
	& old_hear , 					
	& sv_st
	); // end of sscanf
	if(verbose)
		cout << "Serverparam created." << endl;
}



Player_param::Player_param(char * message){
	sscanf(message, "(player_param %d %d %d "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf %lf)",
	// & variables	
	& player_types ,
	& subs_max ,
	& pt_max ,
    
	& player_speed_max_delta_min ,
	& player_speed_max_delta_max ,
	& stamina_inc_max_delta_factor ,
    
	& player_decay_delta_min ,
	& player_decay_delta_max ,
	& inertia_moment_delta_factor ,

	& dash_power_rate_delta_min ,
	& dash_power_rate_delta_max ,
	& player_size_delta_factor ,

	& kickable_margin_delta_min ,
	& kickable_margin_delta_max ,
	& kick_rand_delta_factor ,

	& extra_stamina_delta_min ,
	& extra_stamina_delta_max ,
	& effort_max_delta_factor ,
	& effort_min_delta_factor
	); // end of sscanf
	if(verbose)
		cout << "Playerparam created." << endl;
}




// We need seven instances of Player_type
Player_type::Player_type(char * message){
	sscanf(message, "(player_type %d "
			"%lf %lf %lf %lf %lf " 
			"%lf %lf %lf %lf %lf %lf)",	
	
	// & variables
	& id ,
	& player_speed_max ,
	& stamina_inc_max ,
	& player_decay ,
	& inertia_moment ,
	& dash_power_rate ,
	& player_size ,
	& kickable_margin ,
	& kick_rand , 
	& extra_stamina ,
	& effort_max ,
	& effort_min
	); // end of sscanf;
	if(verbose)
		cout << id << ". Playertype created." << endl;
}

