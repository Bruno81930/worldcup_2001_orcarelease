/* -*- Mode: C++ -*- */


/* MemOption.h
 * CMUnited99 (soccer client for Robocup99)
 * Peter Stone <pstone@cs.cmu.edu>
 * Computer Science Department
 * Carnegie Mellon University
 * Copyright (C) 1999 Peter Stone
 *
 * CMUnited-99 was created by Peter Stone, Patrick Riley, and Manuela Veloso
 *
 * You may copy and distribute this program freely as long as you retain this notice.
 * If you make any changes or have any comments we would appreciate a message.
 * For more information, please see http://www.cs.cmu.edu/~robosoccer/
 */

#ifndef _OPTION_H_
#define _OPTION_H_

#include "types.h"

struct option_t {
   char	optname[50] ;
   void	*vptr ;
   int	vsize ;
};

enum InputType{
 V_INT,
 V_FLOAT,
 V_BOOL,
 V_STRING,	
 V_ONOFF,		
 V_NONE		
};

#define MAX_TEAMNAME_LEN  20
#define MAX_HOST_LEN      50
#define MAX_FILE_LEN      256
#define MAX_FP_LEN        20
#define MAX_TREE_STEM_LEN 50

 // We need seven instances of Player_type
class Player_type{
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
       
 public:
       Player_type(char * message);
       double get_player_speed_max(){return  player_speed_max;};
       double get_stamina_inc_max(){return  stamina_inc_max;};
       double get_player_decay(){return  player_decay;};
       double get_inertia_moment (){return  inertia_moment;};
       double get_dash_power_rate(){return  dash_power_rate;};
       double get_player_size(){return  player_size;};
       double get_kickable_margin(){return  kickable_margin;};
       double get_kick_rand(){return  kick_rand;};
       double get_extra_stamina(){return  extra_stamina;};
       double get_effort_max(){return  effort_max;};
       double get_effort_min(){return  effort_min;};
};





/* Things to be read at startup that never change */
class OptionInfo {
public:
  OptionInfo();
  void GetOptions(int, char**);
  void GetOptions(char*);
  void ParsePlayerParam(char*);
  void ParseServerParam(char*);
  

  /* Client version params */
  Bool  VP_test_l;
  Bool  VP_test_r;
  Bool  VP_test;
  Bool  VP_train_DT;
  Bool  VP_use_DT;
      /* special orca parameters */
  Bool  VP_orca_train;
  char  VP_orca_train_net[MAX_FILE_LEN]; 

	Bool  OP_example_bool;
	Bool  OP_override_max_range;
	Bool  OP_override_home_range;
	Bool  OP_allow_backpass;
	Bool  OP_cover_use_trouble;
	Bool  OP_communicate;
	Bool  OP_communicate_strategy;
	Bool  OP_communicate_ball;
	Bool  OP_communicate_opponents;
	Bool  OP_communicate_mates;
	int   OP_example_int;
	int   OP_goal_kick_num_points_per_side;
	int   OP_communicate_pass_valid_cycles;
	int   OP_communicate_wait_cycles;
	int   OP_communicate_max_opponents;
	int   OP_communicate_max_mates;
	float OP_example_float;
	float OP_max_range;
	float OP_home_range;
	float OP_cone_width_dist_ratio;
	float OP_goal_kick_post_offset;
	float OP_max_goal_dist;
	float OP_max_pass_dist;
	float OP_goal_value_weight;
	int   OP_max_opps_in_passway;
	float OP_min_pass_dist;
	float OP_position_value_y_weight;
	float OP_position_value_x_weight;
	float OP_trouble_value_weight;
	float OP_position_value_weight;
	float OP_trouble_radius;
	float OP_home_dist_pow;
	float OP_ball_dist_pow;
	float OP_goal_dist_pow;
	float OP_my_dist_pow;
	float OP_passway_angle;
	float OP_passway_buffer;
	float OP_home_dist_weight;
	float OP_my_dist_weight;
	float OP_ball_dist_weight;
	float OP_goal_dist_weight;
	float OP_cover_tolerance;
	char  OP_example_string[MAX_FILE_LEN];
	char  OP_pppoints_file_name[MAX_FILE_LEN];
        Bool  OP_use_sfls;
	float OP_ball_conf_pow;
      /* end special orca parameters */
  /* Initialize params */
  int   IP_my_score;
  int   IP_their_score;
  int   IP_reconnect;    /* If non-zero, reconnect to that unum */  
  /* Client params */
  char  MyTeamName[MAX_TEAMNAME_LEN];
  Bool  CP_goalie;
  Bool  CP_save_log;
  int   CP_save_freq;
  Bool  CP_save_sound_log;
  int   CP_save_sound_freq;
  int   CP_save_action_log_level;
  int   CP_save_action_freq;
  float CP_send_ban_recv_step_factor;
  int   CP_interrupts_per_cycle;
  int   CP_interrupts_left_to_act;
  float CP_max_conf;
  float CP_min_valid_conf;
  float CP_conf_decay;
  float CP_player_conf_decay;
  float CP_ball_conf_decay;
  float CP_max_player_move_factor;  /* multiply by SP_player_speed_max to see how far a player can be 
				       from its last position and still be considered the same player */
  int   CP_max_say_interval;
  float CP_ball_moving_threshold;
  float CP_dodge_angle_buffer;
  float CP_dodge_distance_buffer;
  float CP_dodge_power;
  float CP_dodge_angle;
  char  CP_tree_stem[MAX_TREE_STEM_LEN];
  int   CP_DT_evaluate_interval;
  int   CP_say_tired_interval;
  float CP_tired_buffer;
  Bool  CP_set_plays;
  int   CP_Setplay_Delay;
  int   CP_Setplay_Say_Delay;
  int   CP_Setplay_Max_Delay;
  int   CP_Setplay_Time_Limit;
  float CP_kickable_buffer;
  int   CP_mark_persist_time;
  float CP_track_max_distance;
  float CP_track_min_distance;
  Bool  CP_pull_offsides;
  Bool  CP_pull_offsides_when_winning;
  Bool  CP_spar;
  Bool  CP_mark;
  Bool  CP_communicate;
  int   CP_change_view_for_ball_cycles;
  float CP_defer_kick_to_teammate_buffer;
  float CP_scan_overlap_angle;

  float CP_pull_offside_threshold;
  float CP_pull_offside_buffer;
  
  float CP_ball_forget_angle_buf;
  float CP_player_forget_angle_buf;
  float CP_ball_forget_dist_buf;
  float CP_player_forget_dist_buf;
  
  /* pat added these */
  Bool CP_use_new_position_based_vel;
  Bool CP_stop_on_error;

  /* these parameters affect turnball */
  float CP_max_turn_kick_pow;
  float CP_opt_ctrl_dist;
  float CP_closest_margin;
  float CP_dokick_factor;

  /* these basically affect the way turnball stops */
  float CP_KickTo_err;
  float CP_max_ignore_vel;

  int   CP_kick_time_space;
  float CP_max_est_err;
  float CP_holdball_kickable_buffer;
  int   CP_stop_ball_power;
  int   CP_possessor_intercept_space;
  int   CP_can_keep_ball_cycle_buffer;
  
  /* no longer used
  float CP_hard_kick_margin;
  float CP_hard_kick_factor; 
  float CP_hard_kick_end_turn_dist; */
  float CP_hard_kick_dist_buffer;
  int   CP_max_hard_kick_angle_err;
  /* angle off perpendicualr to start ball for hardest kick */
  int   CP_hardest_kick_ball_ang; 
  float CP_hardest_kick_ball_dist; 
  int   CP_hardest_kick_player_ang;
  float CP_max_dash_help_kick_angle;
  
  int   CP_max_go_to_point_angle_err;
  int   CP_max_int_lookahead;
  float CP_intercept_close_dist;
  int   CP_intercept_step;
  int   CP_my_intercept_step;
  int   CP_intercept_aim_ahead;
  int   CP_no_turn_max_cyc_diff; /* used for normal interception */
  float CP_no_turn_max_dist_diff; /* used for ball_path intercept */
  float CP_turnball_opp_worry_dist; 
  float CP_collision_buffer;
  float CP_behind_angle;
  int   CP_time_for_full_rotation;
  float CP_ball_vel_invalidation_factor;

  /* dribble params */
  int   CP_dribble_dash_pow;
  float CP_dribble_ball_dist;
  /* dist where opponent starts to affect where we dribble ball */
  float CP_dribble_ignore_opp_dist ;
  /* dist of opponent that makes us go to DM_Strict mode */
  float CP_dribble_worry_opp_dist;
  /* angle we normally like to dribble at */
  float CP_dribble_angle_norm ;
  /* max and min distnaces to worry about dodging a player */
  float CP_dribble_dodge_max_dist;
  /* angle diff to make us turn if dodging */
  float CP_dribble_dodge_angle_err ;
  /* how far off in expected angle we let a ball before we kick it to correct */
  float CP_dribble_exp_angle_buffer ;
  /* if drib_ang > 180 - X, we will just dribble on the side where the ball is */ 
  float CP_dribble_angle_ignore_buffer;
  float CP_dribble_dodge_close_dist;
  float CP_can_dribble_cone_ratio;
  float CP_dribble_towards_length;
  float CP_dribble_sideline_buffer;
  float CP_dribble_circle_inner_rad;
  float CP_dribble_circle_outer_rad;
  float CP_dribble_circle_ang; //angle realtive to dribble angle to look for players
  Bool  CP_dribble_scan_field; 
  
  float CP_move_imp_1v1_initial;
  float CP_move_imp_1v1_inc;
  float CP_move_imp_1v1_threshold;
  float CP_at_point_buffer;
  float CP_overrun_dist;
  float CP_def_block_dist;
  float CP_def_block_dist_ratio;
  float CP_overrun_buffer;
  float CP_breakaway_buffer;
  float CP_our_breakaway_kickable_buffer;  
  float CP_their_breakaway_front_kickable_buffer;  
  float CP_their_breakaway_back_kickable_buffer;  
  float CP_goalie_breakaway_kickable_buffer;  
  
  float CP_breakaway_approach_x;
  float CP_breakaway_approach_y;
  int   CP_breakaway_targ_valid_time;
  int   CP_breakaway_min_goalie_steal_time;
  int   CP_breakaway_kick_run_min_cycles;
  int   CP_breakaway_kick_run_max_cycles;
  float CP_their_breakaway_min_cone_dist_wid;
  float CP_our_breakaway_min_cone_dist_wid;
  float CP_breakaway_middle_buffer;
  float CP_breakaway_kick_run_worry_dist;
  int   CP_breakaway_mode; // used to test diff breakaway styles
  
  float CP_beat_offsides_buffer;
  float CP_beat_offsides_threshold;
  float CP_beat_offsides_max_x;
  float CP_congestion_epsilon;
  float CP_back_pass_opponent_buffer;
  float CP_back_pass_offside_buffer;
  float CP_min_less_congested_pass_dist;
  
  float CP_cycles_to_kick;

  /* parameters for moving to a standing ball */
  float CP_static_kick_dist_err;
  float CP_static_kick_ang_err;
  /* no longer used 
  float CP_static_kick_dist;
  float CP_static_kick_ang;
  float CP_static_kick_overrun_dist;
  */
  
  float CP_goalie_baseline_buffer;
  float CP_goalie_scan_angle_err;
  float CP_goalie_at_point_buffer;
  float CP_goalie_vis_angle_err;
  float CP_goalie_max_shot_distance;
  float CP_goalie_min_pos_dist;
  float CP_goalie_max_pos_dist;
  float CP_goalie_max_forward_percent;
  float CP_goalie_ball_ang_for_corner;  
  float CP_goalie_max_come_out_dist;
  float CP_goalie_ball_dist_for_corner;
  float CP_goalie_ball_dist_for_center;
  float CP_goalie_free_kick_dist;
  float CP_goalie_go_to_ball_cone_ratio;
  int   CP_goalie_warn_space;
  Bool  CP_goalie_comes_out;
  int   CP_goalie_catch_wait_time;
  float CP_goalie_opponent_dist_to_block;
  float CP_goalie_position_weight_dist;
  int   CP_goalie_narrow_sideline_cyc;
  float CP_goalie_no_buffer_dist;
  float CP_goalie_wait_cyc;

  float CP_clear_ball_ang_step;
  float CP_clear_ball_cone_ratio;
  float CP_clear_ball_max_dist;
  float CP_clear_offensive_min_horiz_dist;
  float CP_clear_offensive_min_angle;

  float CP_should_cross_corner_dist;
  float CP_should_cross_baseline_buffer;
  float CP_should_move_to_cross_corner_dist;
  float CP_cross_pt_x;
  float CP_cross_pt_y;
  float CP_cross_target_vel;

  float CP_dont_dribble_to_middle_min_x;
  
  /* not used anymore
    float CP_hardest_kick_shot_distance;
    float CP_moderate_kick_shot_distance;
  */
  float CP_good_shot_distance;
  float CP_shot_distance;
  int   CP_cycles_to_kick_buffer;
  float CP_shot_speed;
  int CP_shot_goalie_react_buffer;
  int CP_good_shot_goalie_react_buffer;
  int CP_better_shot_cyc_diff;
  //float CP_breakaway_shot_distance; no longer used

  /* Formation params */
  float FP_scale_factor;	// fraction by which to scale the positions
  char  FP_initial_formation[MAX_FP_LEN];
  char  FP_formation_when_tied[MAX_FP_LEN];
  char  FP_formation_when_losing[MAX_FP_LEN];
  char  FP_formation_when_losing_lots[MAX_FP_LEN];
  char  FP_formation_when_winning[MAX_FP_LEN];
  char  FP_initial_hc_method[MAX_FP_LEN];
  char  FP_initial_mc_method[MAX_FP_LEN];
  int   FP_initial_player_1_pos;
  int   FP_initial_player_2_pos;
  int   FP_initial_player_3_pos;
  int   FP_initial_player_4_pos;
  int   FP_initial_player_5_pos;
  int   FP_initial_player_6_pos;
  int   FP_initial_player_7_pos;
  int   FP_initial_player_8_pos;
  int   FP_initial_player_9_pos;
  int   FP_initial_player_10_pos;
  int   FP_initial_player_11_pos;
  int   FP_goalie_number;

  /* Server params */
  float SP_pitch_length;
  float SP_pitch_width;
  float SP_pitch_margin;
  float SP_penalty_area_length;
  float SP_penalty_area_width;
  float SP_goal_area_length;
  float SP_goal_area_width;
  float SP_penalty_spot_dist;
  float SP_corner_arc_r;
  float SP_free_kick_buffer;
  int   SP_after_goal_wait;
  float SP_feel_distance;
  int   SP_num_lines;
  int   SP_num_markers;
  float SP_unum_far_length;
  float SP_unum_too_far_length;
  float SP_team_far_length;
  float SP_team_too_far_length;

  float SP_version;
  int   SP_team_size;
  int   SP_half;
  char  SP_host[MAX_HOST_LEN];
  float SP_goal_width;       
  float SP_player_size;	
  float SP_player_decay;    	
  float SP_player_rand;	
  float SP_player_weight;	
  float SP_player_speed_max;	
  float SP_stamina_max;	
  float SP_stamina_inc;	
  float SP_recover_dec_thr;	
  float SP_recover_min; 	
  float SP_recover_dec; 	
  float SP_effort_dec_thr;	
  float SP_effort_min;	
  float SP_effort_dec;	
  float SP_effort_inc_thr;	
  float SP_effort_inc;	
  float SP_ball_size;		
  float SP_ball_decay;	
  float SP_ball_rand;		
  float SP_ball_weight;	
  float SP_ball_speed_max;	
  float SP_dash_power_rate;	
  float SP_kick_power_rate;	
  float SP_kickable_margin;	
  float SP_kickable_area;	
  float SP_catch_prob;	
  float SP_catch_area_l;	
  float SP_catch_area_w;	
  float SP_max_power;		
  float SP_min_power;		
  float SP_max_moment;		
  float SP_min_moment;		
  float SP_max_neck_angle;
  float SP_min_neck_angle;
  float SP_max_neck_moment;
  float SP_min_neck_moment;
  float SP_visible_angle;	
  float SP_visible_dist;	
  float SP_audio_cut_dist;	
  float SP_dist_qstep;	
  float SP_land_qstep;	
  float SP_ckmargin;	
  float SP_wind_dir;	
  float SP_wind_force;	
  float SP_wind_rand;	
  Bool SP_wind_none;
  Bool SP_wind_random;
  int SP_half_time;
  int SP_port;
  int SP_coach_port;
  int SP_olcoach_port;
  int SP_simulator_step;
  int SP_send_step;
  int SP_recv_step;
  int SP_say_msg_size;
  int SP_hear_max;
  int SP_hear_inc;
  int SP_hear_decay;
  int SP_catch_ban_cycle;
  Bool SP_coach_mode;
  Bool SP_coach_w_referee_mode;
  int SP_say_coach_cnt_max;	
  int SP_say_coach_msg_size;
  int SP_send_vi_step;
  int SP_look_step;

  Bool SP_use_offside;
  Bool SP_forbid_kickoff_offside;
  char SP_logfile[MAX_FILE_LEN];
  char SP_recfile[MAX_FILE_LEN];
  Bool SP_rec_log;
  int  SP_rec_ver;
  char SP_replay[MAX_FILE_LEN];
  Bool SP_verbose;
  Bool SP_send_log;
  float SP_offside_area;
  float SP_inertia_moment;
  int   SP_sense_body_step;
  float SP_offside_kick_margin;
  int SP_slow_down_factor;
  Bool  SP_record_messages;
  int   SP_goalie_max_moves;
/* Changes for Version 7.00 */


    int CP_player_types;
    int CP_subs_max;
    int CP_pt_max;
  
    double CP_player_speed_max_delta_min;
    double CP_player_speed_max_delta_max;
    double CP_stamina_inc_max_delta_factor;
    
    double CP_player_decay_delta_min;
    double CP_player_decay_delta_max;
    double CP_inertia_moment_delta_factor;

    double CP_dash_power_rate_delta_min;
    double CP_dash_power_rate_delta_max;
    double CP_player_size_delta_factor;

    double CP_kickable_margin_delta_min;
    double CP_kickable_margin_delta_max;
    double CP_kick_rand_delta_factor;

    double CP_extra_stamina_delta_min;
    double CP_extra_stamina_delta_max;
    double CP_effort_max_delta_factor;
    double CP_effort_min_delta_factor;

  /*PlayerParams that are new, but that I don't since when */
                                  /********  ????  ****/
   
    double SP_player_accel_max;
    double  SP_baccel_max;
    int SP_kick_rand;
    
    int SP_clang_win_size;
    int SP_clang_define_win;
    int SP_clang_meta_win;
    int SP_clang_advice_win;
    int SP_clang_info_win;
    int SP_clang_mess_delay;
    int SP_clang_mess_per_cycle;
    
    bool SP_log_times;

   /* New Parametervalues send to us by the server*/
   
 /*   int SP_recover_init;
    int SP_effort_init;
    int SP_team_actuator_noise;
    int SP_prand_factor_l;
    int SP_prand_factor_r;
    int SP_kick_rand_factor_l;
    int SP_kick_rand_factor_r;
    int SP_ctlradius;
    int SP_ctlradius_width;
    int SP_ball_accel_max;
    int SP_max_movement;
    int SP_min_movement;
    int SP_max_neck_movement;
    int SP_min_neck_movement;
    
        
    int SP_wind_angle;
    double SP_lcm_st;
    double SP_dir_qstep;
    double SP_dist_qstep_l;
    double SP_dist_qstep_r;
    double SP_land_qstep_l;
    double SP_land_qstep_r;
    double SP_dir_qstep_l;
    double SP_dir_qstep_r;
    double SP_old_hear;
   */     
  /*PlayerType [] to save the 7 playertype */
  Player_type* HeteroPlayerTypes[7];
};

#endif

