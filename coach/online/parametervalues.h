// This file contains classes for server_param, player_param and player_info

class Server_param{
public:
	double gwidth ; 					
	double inertia_moment ; 	
	double psize ; 			
	double pdecay ; 			
	double prand ; 			
	double pweight ; 				
	double pspeed_max ; 		
	double paccel_max ; 		
	double stamina_max ; 			
	double stamina_inc ; 			
	double recover_init ; 		
	double recover_dthr ; 		
	double recover_min ; 			
	double recover_dec ; 			
	double effort_init ; 			
	double effort_dthr ; 			
	double effort_min ; 		
	double effort_dec ; 			
	double effort_ithr ; 			
	double effort_inc ; 			
	double kick_rand ;                             
	int team_actuator_noise ;                     
	double prand_factor_l ;                          
	double prand_factor_r ;                            
	double kick_rand_factor_l ;                       
	double kick_rand_factor_r ;                     
	double bsize ; 			
	double bdecay ; 			
	double brand ; 			
	double bweight ; 				
	double bspeed_max ; 			
	double baccel_max ; 			
	double dprate ; 			
	double kprate ; 			
	double kmargin ; 				
	double ctlradius ; 			
	double ctlradius_width ; 		
	double maxp ;				
	double minp ; 				
	double maxm ; 				
	double minm ; 				
	double maxnm ; 				
	double minnm ; 				
	double maxn ; 				
	double minn ; 				
	double visangle ; 			
	double visdist ; 					
	double windir ; 					
	double winforce ; 				
	double winang ; 					
	double winrand ; 					
	double kickable_area ; 			
	double catch_area_l ; 			
	double catch_area_w ; 			
	double catch_prob ; 				
	int goalie_max_moves ;                  	
	double ckmargin ; 				
	double offside_area ; 
			
	int win_no ; 					
	int win_random ; 				
	int say_cnt_max ; 				
	int SayCoachMsgSize ; 				
	int clang_win_size ;
	int clang_define_win ;
	int clang_meta_win ;
	int clang_advice_win ;
	int clang_info_win ;
	int clang_mess_delay ;
	int clang_mess_per_cycle ;
	int half_time ; 					
	int sim_st ; 					
	int send_st ; 					
	int recv_st ; 					
	int sb_step ; 					
	int lcm_st ; 		                        
	int SayMsgSize ; 				
	int hear_max ; 					
	int hear_inc ; 					
	int hear_decay ; 				
	int cban_cycle ; 				
	int slow_down_factor ;                           
	int useoffside ; 				
	int kickoffoffside ; 			

	double offside_kick_margin ; 		
	double audio_dist ; 				
	double dist_qstep ; 				
	double land_qstep ; 
	double dir_qstep ; 				
	double dist_qstep_l ; 			
	double dist_qstep_r ; 			
	double land_qstep_l ; 			
	double land_qstep_r ; 			
	double dir_qstep_l ; 				
	double dir_qstep_r ;
	int CoachMode ; 				
	int CwRMode ; 					
	int old_hear ; 					
	int sv_st ;
	

	Server_param(char * message);
};




class Player_param{
public:
	int player_types ;
	int subs_max ;
	int pt_max ;
    
	double player_speed_max_delta_min ;
	double player_speed_max_delta_max ;
	double stamina_inc_max_delta_factor ;
    
	double player_decay_delta_min ;
	double player_decay_delta_max ;
	double inertia_moment_delta_factor ;

	double dash_power_rate_delta_min ;
	double dash_power_rate_delta_max ;
	double player_size_delta_factor ;

	double kickable_margin_delta_min ;
	double kickable_margin_delta_max ;
	double kick_rand_delta_factor ;

	double extra_stamina_delta_min ;
	double extra_stamina_delta_max ;
	double effort_max_delta_factor ;
	double effort_min_delta_factor ;
		

	Player_param(char * message);

};


// We need seven instances of Player_type
class Player_type{
public:
	int id ;
	double player_speed_max ;
	double stamina_inc_max ;
	double player_decay ;
	double inertia_moment ;
	double dash_power_rate ;
	double player_size ;
	double kickable_margin ;
	double kick_rand ; 
	double extra_stamina ;
	double effort_max ;
	double effort_min ;
	
	Player_type(char * message);

};



