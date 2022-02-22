/* -*- Mode: C++ -*- */

/* MemOption.C
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

#include "netif.h"
#include "utils.h"
#include "MemOption.h"
//#include "logging.h"

  int numoption;

/* setting defaults to match version 4.06 server.conf */
OptionInfo::OptionInfo(){

  VP_test_l   = FALSE;
  VP_test_r   = FALSE;
  VP_test     = FALSE;
  VP_train_DT = FALSE;
  VP_use_DT   = FALSE;

  IP_my_score             = 0;
  IP_their_score          = 0;
  IP_reconnect            = 0;

  sprintf(MyTeamName,     "CMUnited");

  /* no option flags for these */
  SP_pitch_length 	  = 105.0;
  SP_pitch_width 	  = 68.0;
  SP_pitch_margin	  = 5.0;
  SP_penalty_area_length  = 16.5;
  SP_penalty_area_width   = 40.32;
  SP_goal_area_length	  = 5.5;
  SP_goal_area_width	  = 18.32;
  SP_penalty_spot_dist    = 11.0;
  SP_corner_arc_r	  = 1.0;
  SP_free_kick_buffer     = 9.15;
  SP_after_goal_wait  	  = 50;
  SP_feel_distance        = 3.0;
  SP_num_lines            = 4;
  SP_num_markers          = 55;
  SP_unum_far_length      = 20.0;
  SP_unum_too_far_length  = 40.0;
  SP_team_far_length      = 40.0;
  SP_team_too_far_length  = 60.0;

  SP_version              = 4.06;
  SP_team_size            = 11;
  SP_half                 = 1;
  sprintf(SP_host,        "localhost");
  SP_goal_width           = 14.02;       
  SP_player_size          = 0.8;
  SP_player_decay         = 0.4;
  SP_player_rand 	  = 0.1;
  SP_player_weight	  = 60.0;
  SP_player_speed_max     = 32.0;
  SP_stamina_max	  = 2000.0;
  SP_stamina_inc          = 20.0;
  SP_recover_dec_thr      = 0.3;	
  SP_recover_dec          = 0.05; 	
  SP_recover_min          = 0.1; 	
  SP_effort_dec_thr       = 0.4;	
  SP_effort_min           = 0.1;	
  SP_effort_dec           = 0.05;	
  SP_effort_inc_thr       = 0.9;	
  SP_effort_inc           = 0.05;	
  SP_ball_size            = 0.085;		
  SP_ball_decay           = 0.96;	
  SP_ball_rand            = 0.05;		
  SP_ball_weight          = 0.2;;	
  SP_ball_speed_max       = 32.0;	
  SP_dash_power_rate      = 0.01;	
  SP_kick_power_rate      = 0.016;
  SP_kickable_margin      = 1.0;	
  SP_kickable_area        = SP_kickable_margin + SP_ball_size + SP_player_size;
  SP_catch_prob           = 1.0;	
  SP_catch_area_l         = 2.0;	
  SP_catch_area_w         = 1.0;	
  SP_catch_ban_cycle      = 5;
  SP_max_power            = 100;		
  SP_min_power            = -30;		
  SP_max_moment           = 180;		
  SP_min_moment           = -180;		
  SP_min_neck_angle       = -90.0;
  SP_max_neck_angle       = 90.0;
  SP_min_neck_moment      = -180.0;
  SP_max_neck_moment      = 180.0;
  SP_visible_angle        = 90.0;	
  SP_audio_cut_dist       = 50.0;	
  SP_dist_qstep           = 0.1;	
  SP_land_qstep           = 0.01;	
  SP_ckmargin             = 1.0;	
  SP_wind_dir             = 0.0;	
  SP_wind_force           = 0.0;
  SP_wind_rand            = 0.0;	
  SP_wind_none            = FALSE;
  SP_wind_random          = FALSE;
  SP_half_time            = 300;
  SP_port                 = 6000;
  SP_coach_port           = 6001;
  SP_olcoach_port         = 6002;
  SP_simulator_step       = 100;
  SP_send_step            = 150;
  SP_recv_step            = 10;
  SP_say_msg_size         = 512;
  SP_hear_max             = 2;
  SP_hear_inc             = 1;
  SP_hear_decay           = 2;
  SP_coach_mode           = FALSE;
  SP_coach_w_referee_mode = FALSE;
  SP_say_coach_cnt_max    = 128;
  SP_say_coach_msg_size   = 128;
  SP_send_vi_step         = 100;
  SP_look_step            = 100;

  /// changed by andreas
  SP_use_offside          = TRUE;

  SP_forbid_kickoff_offside = TRUE;
  SP_verbose              = TRUE;
  SP_offside_area         = 9.15;
  SP_inertia_moment       = 5.0;
  SP_sense_body_step      = 100;
  SP_offside_kick_margin  = 9.15;
  SP_record_messages      = FALSE;
  SP_goalie_max_moves     = 2; /* new in 6.00 */
  
  CP_goalie               = FALSE;
  CP_save_log             = FALSE;
  CP_save_freq            = 10;
  CP_save_sound_log       = FALSE;
  CP_save_sound_freq      = 10;
  CP_save_action_log_level = 0; /* 0 means save nothing */
  CP_save_action_freq      = 40;
  CP_send_ban_recv_step_factor = 3.0;
  CP_interrupts_per_cycle = 2;
  CP_interrupts_left_to_act = 2;
  CP_max_conf             = 1.0;
  CP_min_valid_conf       = 0.5;
  CP_conf_decay           = 0.98;
  CP_player_conf_decay    = 0.99;
  CP_ball_conf_decay      = 0.9;
  CP_max_player_move_factor = 4;
  CP_max_say_interval     = 100;
  CP_ball_moving_threshold = .2; /* experimentally checked -- ball still, player fast => .15 ball speed */
  CP_dodge_angle_buffer    = 25;
  CP_dodge_distance_buffer = 3.5;
  CP_dodge_power           = 30;
  CP_dodge_angle           = 90;
  sprintf(CP_tree_stem,      "pass");
  CP_DT_evaluate_interval  = 10;
  CP_say_tired_interval    = 20;
  CP_tired_buffer          = 10;
  CP_set_plays             = FALSE;
  CP_Setplay_Delay         = 5;
  CP_Setplay_Say_Delay     = SP_hear_decay*5;
  CP_Setplay_Max_Delay     = 100;
  CP_Setplay_Time_Limit    = 150;
  CP_kickable_buffer       = .1;
  CP_mark_persist_time     = 100;
  CP_track_min_distance    = 0;
  CP_track_max_distance    = 15;
  CP_pull_offsides         = FALSE;
  CP_pull_offsides_when_winning = TRUE;
  CP_spar                  = TRUE;
  CP_mark                  = TRUE;
  CP_communicate           = TRUE;
  CP_change_view_for_ball_cycles = 2;
  CP_defer_kick_to_teammate_buffer = .05;
  CP_scan_overlap_angle    = 2;

  CP_pull_offside_threshold = 5;
  CP_pull_offside_buffer = 3;

  CP_ball_forget_angle_buf = 3;
  CP_player_forget_angle_buf = 5;
  CP_ball_forget_dist_buf = 1;
  CP_player_forget_dist_buf = 1;

  CP_beat_offsides_buffer = 20;
  CP_beat_offsides_threshold = 30;
  CP_beat_offsides_max_x = 25;
  CP_congestion_epsilon = .01;
  CP_back_pass_opponent_buffer = 10;
  CP_back_pass_offside_buffer = 10;
  CP_min_less_congested_pass_dist = 7;
  
  /* pat added these */
  CP_use_new_position_based_vel = TRUE;
  CP_stop_on_error = TRUE;
  
  CP_opt_ctrl_dist        = 1.6;//(SP_player_size + .8 *SP_kickable_margin);
  CP_KickTo_err           = 3;
  CP_closest_margin       = .97;//(SP_player_size + 2.0*SP_ball_size);
  CP_dokick_factor        = .22;
  CP_max_turn_kick_pow    = 20;

  CP_max_ignore_vel       = .005;
  CP_kick_time_space      = 1;
  CP_max_est_err          = .3;
  CP_max_go_to_point_angle_err = 5;
  CP_holdball_kickable_buffer = .1;
  CP_stop_ball_power          = 30;
  CP_possessor_intercept_space = 4;
  CP_can_keep_ball_cycle_buffer = 0;

  /* no longer used
  CP_hard_kick_margin     = .97;//(SP_player_size + 2.0*SP_ball_size);
  CP_hard_kick_factor     = .25;
  CP_hard_kick_end_turn_dist = 1.1;//(SP_player_size + .3 *SP_kickable_margin);
  */
  CP_hard_kick_dist_buffer = .1; 
  CP_max_hard_kick_angle_err  = 5;

  CP_hardest_kick_player_ang = 90; //angle relative to direction of ball
  CP_hardest_kick_ball_dist = .831; //kickable_area * .6 
  CP_hardest_kick_ball_ang = 15; // this is realtive to the direction of travel 
  CP_max_dash_help_kick_angle = 60;

  CP_max_int_lookahead    = 20;
  CP_intercept_step       = 2;
  CP_my_intercept_step    = 2;
  CP_intercept_aim_ahead  = 1;
  CP_no_turn_max_cyc_diff = -1;
  CP_no_turn_max_dist_diff = 1.0;
  CP_turnball_opp_worry_dist = 5; 
  CP_collision_buffer = .1;
  CP_behind_angle  = 80;
  CP_time_for_full_rotation = 24; /* average guestimate */
  CP_ball_vel_invalidation_factor = 2.0;

  CP_dribble_dash_pow = 75;
  CP_dribble_ball_dist    = 1.2;
  CP_dribble_angle_norm = 60;
  CP_dribble_exp_angle_buffer = 10;

  CP_dribble_ignore_opp_dist = 15;
  CP_dribble_worry_opp_dist = 4;
  CP_dribble_dodge_max_dist = 6;
  CP_dribble_dodge_angle_err = 15;
  CP_dribble_angle_ignore_buffer = 5;
  CP_dribble_dodge_close_dist = 2;
  CP_dribble_scan_field = TRUE; 
  CP_can_dribble_cone_ratio = .75;

  CP_dribble_towards_length = 10;
  CP_dribble_sideline_buffer = 1.5;
  CP_dribble_circle_inner_rad = 3;
  CP_dribble_circle_outer_rad = 4;
  CP_dribble_circle_ang = 90;

  CP_move_imp_1v1_initial =  0.0;
  CP_move_imp_1v1_inc =       .2;
  CP_move_imp_1v1_threshold = 1.0 ;
  CP_at_point_buffer = 1;
  CP_overrun_dist = 3;
  CP_def_block_dist = 2;
  CP_def_block_dist_ratio = .5;
  CP_overrun_buffer = 2.5;
  CP_cycles_to_kick = 4;
  CP_breakaway_buffer = 3;
  CP_our_breakaway_kickable_buffer = 1.5;
  CP_their_breakaway_front_kickable_buffer = 5.0;
  CP_their_breakaway_back_kickable_buffer = 2.0;

  CP_breakaway_approach_x = 35;
  CP_breakaway_approach_y = 8;
  CP_breakaway_targ_valid_time = 3;
  CP_breakaway_min_goalie_steal_time = 6;
  CP_breakaway_kick_run_min_cycles = 7;
  CP_breakaway_kick_run_max_cycles = 14;
  CP_our_breakaway_min_cone_dist_wid = 18;
  CP_their_breakaway_min_cone_dist_wid = 12;
  CP_breakaway_middle_buffer = 3;
  CP_breakaway_kick_run_worry_dist = 10;
  CP_breakaway_mode = 0;
  
  CP_static_kick_dist_err = .3;//old: .14
  CP_static_kick_ang_err = 15;//old: 5
  //no longer used
  //CP_static_kick_dist = .985;
  //CP_static_kick_ang = 47;  /* caculated value! */
    //CP_static_kick_ang = 42;  /* caculated value!- extar buffer */
    //  CP_static_kick_overrun_dist = 4;

  CP_goalie_baseline_buffer = 1;
  CP_goalie_scan_angle_err = 10;
  CP_goalie_at_point_buffer = .1;
  CP_goalie_vis_angle_err = 5;
  CP_goalie_max_shot_distance = 40;
  CP_goalie_min_pos_dist = 15;
  CP_goalie_max_pos_dist = SP_pitch_length * .75;
  CP_goalie_max_forward_percent = .75;
  CP_goalie_ball_ang_for_corner = 90;
  CP_goalie_max_come_out_dist = 10;
  CP_goalie_ball_dist_for_corner = SP_penalty_area_length;
  CP_goalie_ball_dist_for_center = SP_pitch_length / 2;
  CP_goalie_free_kick_dist = 3;
  CP_goalie_go_to_ball_cone_ratio = .25;
  CP_goalie_warn_space = 10;
  CP_goalie_comes_out = TRUE;
  CP_goalie_catch_wait_time = 2;
  CP_goalie_opponent_dist_to_block = 15;
  CP_goalie_position_weight_dist = 10;
  CP_goalie_narrow_sideline_cyc = 3;
  CP_goalie_no_buffer_dist = 10;
  CP_goalie_wait_cyc = 40;

  CP_clear_ball_ang_step = 5.0;
  CP_clear_ball_cone_ratio = .5;
  CP_clear_ball_max_dist = 30;
  CP_clear_offensive_min_horiz_dist = 20;
  CP_clear_offensive_min_angle = 60;

  CP_should_cross_corner_dist = 14; //pitch_width /2 - penalty_area_w / 2
  CP_should_cross_baseline_buffer = 3;
  CP_should_move_to_cross_corner_dist = 20; 
  CP_cross_pt_x = 36;  //pitch_length / 2 - penalty_area_l
  CP_cross_pt_y = 9; //goalie_area_w / 2
  CP_cross_target_vel = .5;

  CP_dont_dribble_to_middle_min_x = 20;

  /* not used anymore
     CP_hardest_kick_shot_distance = 13;
     CP_moderate_kick_shot_distance = 9;
  */
  CP_good_shot_distance = 20;
  CP_shot_distance = 30;
  CP_cycles_to_kick_buffer = 1;
  CP_better_shot_cyc_diff = 5;
  //CP_breakaway_shot_distance = 16;
  CP_shot_speed = 2.0; //our average shot speed
  CP_shot_goalie_react_buffer = 5;
  CP_good_shot_goalie_react_buffer = 2;

  sprintf(FP_initial_formation, "433");
  sprintf(FP_formation_when_tied, "initial");
  sprintf(FP_formation_when_losing, "initial");
  sprintf(FP_formation_when_losing_lots, "initial");
  sprintf(FP_formation_when_winning, "initial");
  sprintf(FP_initial_hc_method, "Obey");
  sprintf(FP_initial_mc_method, "Obey");
  FP_scale_factor 	      = 3.0/7.0;
  FP_initial_player_1_pos     = 1;
  FP_initial_player_2_pos     = 2;
  FP_initial_player_3_pos     = 3;
  FP_initial_player_4_pos     = 4;
  FP_initial_player_5_pos     = 5;
  FP_initial_player_6_pos     = 6;
  FP_initial_player_7_pos     = 7;
  FP_initial_player_8_pos     = 8;
  FP_initial_player_9_pos     = 9;
  FP_initial_player_10_pos    = 10;
  FP_initial_player_11_pos    = 11;
  FP_goalie_number            = 1;

  /* New for Version 7.00 
     Player-Params for heterogen Players
  */ 

      /* special orca parameters */
  
  OP_example_bool = TRUE;
  OP_override_max_range = TRUE;
  OP_override_home_range = TRUE;
  OP_allow_backpass = FALSE;
  OP_communicate_pass_valid_cycles = 12;
  OP_cover_use_trouble = TRUE;
  OP_communicate = TRUE;
  OP_communicate_strategy = TRUE;
  OP_communicate_ball = TRUE;
  OP_communicate_opponents = TRUE;
  OP_communicate_mates = TRUE;
  OP_example_int = 23;
  OP_goal_kick_num_points_per_side = 13;
  OP_communicate_wait_cycles = 1;
  OP_communicate_max_opponents = 4;
  OP_communicate_max_mates = 4;
  OP_example_float = .5;
  OP_ball_conf_pow = 2.5;
  OP_max_range = 12;
  OP_home_range = 6;
  OP_cone_width_dist_ratio = .5;
  OP_goal_kick_post_offset = .3;
  OP_max_goal_dist = 19;
  OP_max_pass_dist = 35;
  OP_goal_value_weight = 1.0;
  OP_max_opps_in_passway = 0;
  OP_min_pass_dist = .5;
  OP_position_value_y_weight = .3;
  OP_position_value_x_weight = 1;
  OP_trouble_value_weight = .3;
  OP_position_value_weight = 1.0;
  OP_trouble_radius = 30;
  OP_home_dist_pow = 2.5;
  OP_ball_dist_pow = 1.0;
  OP_goal_dist_pow = 2.0;
  OP_my_dist_pow = 1.0;
  OP_passway_angle = 20;
  OP_passway_buffer = .2;
  OP_home_dist_weight = 1.0;
  OP_my_dist_weight = 1.0;
  OP_ball_dist_weight = 2.0;
  OP_goal_dist_weight = 3.0;
  OP_cover_tolerance = 3.0;
  sprintf(OP_example_string, "Beispiel");
  sprintf(OP_pppoints_file_name, "pppoints");
  OP_use_sfls = FALSE;
  OP_ball_conf_pow = 2.5;


      /* end special orca parameters */



}

void OptionInfo::GetOptions(int argc, char **argv)
{
  option_t opt[] = {
    /* special orca parameters */
    {"orca_train",                    (void *)&VP_orca_train,       V_ONOFF},
    {"orca_train_net",                    (void *)&VP_orca_train_net,   V_STRING},

    {"example_string",      (void *)&OP_example_string,     V_STRING},
    {"pppoints_file_name",      (void *)&OP_pppoints_file_name,     V_STRING},
    {"example_bool",        (void *)&OP_example_bool,       V_ONOFF},
    {"override_max_range",        (void *)&OP_override_max_range,       V_ONOFF},
    {"override_home_range",        (void *)&OP_override_home_range,       V_ONOFF},
    {"allow_backpass",        (void *)&OP_allow_backpass,       V_ONOFF},
    {"communicate_pass_valid_cycles",        (void *)&OP_communicate_pass_valid_cycles,       V_INT},
    {"cover_use_trouble",        (void *)&OP_cover_use_trouble,       V_ONOFF},
    {"communicate",        (void *)&OP_communicate,       V_ONOFF},
    {"communicate_strategy",        (void *)&OP_communicate_strategy,       V_ONOFF},
    {"communicate_ball",        (void *)&OP_communicate_ball,       V_ONOFF},
    {"communicate_opponents",        (void *)&OP_communicate_opponents,       V_ONOFF},
    {"communicate_mates",        (void *)&OP_communicate_mates,       V_ONOFF},
    {"example_float",       (void *)&OP_example_float,      V_FLOAT},
    {"ball_conf_pow",       (void *)&OP_ball_conf_pow,      V_FLOAT},
    {"max_range",       (void *)&OP_max_range,      V_FLOAT},
    {"home_range",       (void *)&OP_home_range,      V_FLOAT},
    {"cone_width_dist_ratio",       (void *)&OP_cone_width_dist_ratio,      V_FLOAT},
    {"goal_kick_post_offset",       (void *)&OP_goal_kick_post_offset,      V_FLOAT},
    {"max_goal_dist",       (void *)&OP_max_goal_dist,      V_FLOAT},
    {"max_pass_dist",       (void *)&OP_max_pass_dist,      V_FLOAT},
    {"goal_value_weight",       (void *)&OP_goal_value_weight,      V_FLOAT},
    {"max_opps_in_passway",       (void *)&OP_max_opps_in_passway,      V_INT},
    {"min_pass_dist",       (void *)&OP_min_pass_dist,      V_FLOAT},
    {"position_value_y_weight",       (void *)&OP_position_value_y_weight,      V_FLOAT},
    {"position_value_x_weight",       (void *)&OP_position_value_x_weight,      V_FLOAT},
    {"trouble_value_weight",       (void *)&OP_trouble_value_weight,      V_FLOAT},
    {"position_value_weight",       (void *)&OP_position_value_weight,      V_FLOAT},
    {"trouble_radius",       (void *)&OP_trouble_radius,      V_FLOAT},
    {"home_dist_pow",       (void *)&OP_home_dist_pow,      V_FLOAT},
    {"ball_dist_pow",       (void *)&OP_ball_dist_pow,      V_FLOAT},
    {"goal_dist_pow",       (void *)&OP_goal_dist_pow,      V_FLOAT},
    {"my_dist_pow",       (void *)&OP_my_dist_pow,      V_FLOAT},
    {"passway_angle",       (void *)&OP_passway_angle,      V_FLOAT},
    {"passway_buffer",       (void *)&OP_passway_buffer,      V_FLOAT},
    {"my_dist_weight",       (void *)&OP_my_dist_weight,      V_FLOAT},
    {"home_dist_weight",       (void *)&OP_home_dist_weight,      V_FLOAT},
    {"goal_dist_weight",       (void *)&OP_goal_dist_weight,      V_FLOAT},
    {"ball_dist_weight",       (void *)&OP_ball_dist_weight,      V_FLOAT},
    {"cover_tolerance",       (void *)&OP_cover_tolerance,      V_FLOAT},
    {"example_int",         (void *)&OP_example_int,        V_INT},
    {"goal_kick_num_points_per_side",         (void *)&OP_goal_kick_num_points_per_side,        V_INT},
    {"communicate_wait_cycles",         (void *)&OP_communicate_wait_cycles,        V_INT},
    {"communicate_max_opponents",         (void *)&OP_communicate_max_opponents,        V_INT},
    {"communicate_max_mates",         (void *)&OP_communicate_max_mates,        V_INT},
    {"sfls",         (void *)&OP_use_sfls, V_ONOFF},
    {"ball_conf_pow", 	(void *)&OP_ball_conf_pow, V_FLOAT},


      /* end special orca parameters */
    {"test_l",        		(void *)&VP_test_l,             V_ONOFF},
    {"test_r",        		(void *)&VP_test_r,             V_ONOFF},
    {"test",        		(void *)&VP_test,               V_ONOFF},
    {"train_DT",                    (void *)&VP_train_DT,           V_ONOFF},
    {"use_DT",                      (void *)&VP_use_DT,             V_ONOFF},

    {"my_score",        		(void *)&IP_my_score,           V_INT},
    {"their_score",        		(void *)&IP_their_score,        V_INT},
    {"reconnect",                   (void *)&IP_reconnect,          V_INT},

    {"team_name",        		(void *)&MyTeamName,            V_STRING},
    {"goalie",        		(void *)&CP_goalie,             V_ONOFF},
    {"save_log",        		(void *)&CP_save_log,           V_ONOFF},
    {"save_freq",                   (void *)&CP_save_freq,          V_INT},
    {"save_sound_log",  		(void *)&CP_save_sound_log,     V_ONOFF},
    {"save_sound_freq",             (void *)&CP_save_sound_freq,    V_INT},
    {"save_action_log_level",       (void *)&CP_save_action_log_level, V_INT},
    {"save_action_freq",            (void *)&CP_save_action_freq, V_INT},
    {"send_ban_recv_step_factor",   (void *)&CP_send_ban_recv_step_factor, V_FLOAT},
    {"interrupts_per_cycle",        (void *)&CP_interrupts_per_cycle,   V_INT},
    {"interrupts_left_to_act",      (void *)&CP_interrupts_left_to_act,   V_INT},
    {"max_conf",                    (void *)&CP_max_conf,           V_FLOAT},
    {"min_conf",                    (void *)&CP_min_valid_conf,     V_FLOAT},
    {"conf_decay",                  (void *)&CP_conf_decay,         V_FLOAT},
    {"player_conf_decay",           (void *)&CP_player_conf_decay,  V_FLOAT},
    {"ball_conf_decay",             (void *)&CP_ball_conf_decay,    V_FLOAT},
    {"max_player_move_factor",      (void *)&CP_max_player_move_factor, V_FLOAT},
    {"max_say_interval",            (void *)&CP_max_say_interval,   V_INT},
    {"ball_moving_threshold",       (void *)&CP_ball_moving_threshold, V_FLOAT},
    {"dodge_distance_buffer",       (void *)&CP_dodge_distance_buffer, V_FLOAT},
    {"dodge_angle_buffer",          (void *)&CP_dodge_angle_buffer, V_FLOAT},
    {"dodge_power",                 (void *)&CP_dodge_power,        V_FLOAT},
    {"dodge_angle",                 (void *)&CP_dodge_angle,        V_FLOAT},
    {"tree_stem",                   (void *)&CP_tree_stem,          V_STRING},
    {"DT_evaluate_interval",        (void *)&CP_DT_evaluate_interval, V_INT},
    {"say_tired_interval",          (void *)&CP_say_tired_interval, V_INT},
    {"tired_buffer",                (void *)&CP_tired_buffer,       V_FLOAT},
    {"set_plays",                   (void *)&CP_set_plays,          V_ONOFF},
    {"set_play_delay",              (void *)&CP_Setplay_Delay,     V_INT},
    {"set_play_say_delay",          (void *)&CP_Setplay_Say_Delay,  V_INT},
    {"set_play_time_limit",         (void *)&CP_Setplay_Time_Limit, V_INT},
    {"kickable_buffer",             (void *)&CP_kickable_buffer,    V_FLOAT},
    {"mark_persist_time",           (void *)&CP_mark_persist_time,  V_INT},
    {"track_max_distance",          (void *)&CP_track_max_distance, V_FLOAT},
    {"track_min_distance",          (void *)&CP_track_min_distance, V_FLOAT},
    {"pull_offsides",               (void *)&CP_pull_offsides,      V_ONOFF},
    {"pull_offsides_when_winning",  (void *)&CP_pull_offsides_when_winning, V_ONOFF},
    {"spar",                        (void *)&CP_spar,            V_ONOFF},
    {"mark",                        (void *)&CP_mark,            V_ONOFF},
    {"communicate",                 (void *)&CP_communicate,        V_ONOFF},
    {"change_view_for_ball_cycles", (void *)&CP_change_view_for_ball_cycles, V_INT},
    {"defer_kick_to_teammate_buffer",(void *)&CP_defer_kick_to_teammate_buffer, V_FLOAT},
    {"scan_overlap_angle",          (void *)&CP_scan_overlap_angle, V_FLOAT},

    {"pull_offside_threshold",      (void *)&CP_pull_offside_threshold, V_FLOAT},
    {"pull_offside_buffer",         (void *)&CP_pull_offside_buffer,    V_FLOAT},

    {"ball_forget_angle_buf",       (void *)&CP_ball_forget_angle_buf,   V_FLOAT},
    {"player_forget_angle_buf",     (void *)&CP_player_forget_angle_buf, V_FLOAT},
    {"ball_forget_dist_buf",        (void *)&CP_ball_forget_dist_buf,    V_FLOAT},
    {"player_forget_dist_buf",      (void *)&CP_player_forget_dist_buf,  V_FLOAT},

    {"beat_offsides_buffer",        (void *)&CP_beat_offsides_buffer,    V_FLOAT},
    {"beat_offsides_threshold",     (void *)&CP_beat_offsides_threshold,    V_FLOAT},
    {"beat_offsides_max_x",         (void *)&CP_beat_offsides_max_x,    V_FLOAT},
    {"congestion_epsilon",          (void *)&CP_congestion_epsilon,    V_FLOAT},
    {"back_pass_opponent_buffer",   (void *)&CP_back_pass_opponent_buffer,    V_FLOAT},
    {"back_pass_offside_buffer",   (void *)&CP_back_pass_offside_buffer,    V_FLOAT},
    {"min_less_congested_pass_dist",(void *)&CP_min_less_congested_pass_dist,    V_FLOAT},

    {"use_new_position_based_vel",  (void *)&CP_use_new_position_based_vel, V_ONOFF},
    {"stop_on_error",               (void *)&CP_stop_on_error,      V_ONOFF},

    {"opt_ctrl_dist",               (void *)&CP_opt_ctrl_dist,      V_FLOAT},
    {"KickTo_err",                  (void *)&CP_KickTo_err,         V_FLOAT},
    {"closest_margin",              (void *)&CP_closest_margin,     V_FLOAT},
    {"dokick_factor",               (void *)&CP_dokick_factor,      V_FLOAT},
    {"max_turn_kick_pow",           (void *)&CP_max_turn_kick_pow,  V_FLOAT},
    {"kick_time_space",             (void *)&CP_kick_time_space,    V_INT},
    {"max_ignore_vel",              (void *)&CP_max_ignore_vel,     V_FLOAT},
    {"max_est_err",                 (void *)&CP_max_est_err,        V_FLOAT},
    {"holdball_kickable_buffer",    (void *)&CP_holdball_kickable_buffer, V_FLOAT},
    {"stop_ball_power",             (void *)&CP_stop_ball_power, V_INT},
    {"possessor_intercept_space",   (void *)&CP_possessor_intercept_space, V_INT},
    {"can_keep_ball_cycle_buffer",  (void *)&CP_can_keep_ball_cycle_buffer, V_INT},

    /* no longer used 
    {"hard_kick_margin",            (void *)&CP_hard_kick_margin,   V_FLOAT},
    {"hard_kick_end_turn_dist",     (void *)&CP_hard_kick_end_turn_dist,  V_FLOAT},
    {"hard_kick_factor",            (void *)&CP_hard_kick_factor,   V_FLOAT},
    */
    {"max_hard_kick_angle_err",     (void *)&CP_max_hard_kick_angle_err, V_INT},
    {"hard_kick_dist_buffer",       (void *)&CP_hard_kick_dist_buffer, V_FLOAT},
    {"hardest_kick_ball_ang",       (void *)&CP_hardest_kick_ball_ang,  V_INT},
    {"hardest_kick_ball_dist",      (void *)&CP_hardest_kick_ball_dist,  V_FLOAT},
    {"hardest_kick_player_ang",     (void *)&CP_hardest_kick_player_ang, V_INT},
    {"max_dash_help_kick_angle",    (void *)&CP_max_dash_help_kick_angle, V_FLOAT},

    {"max_go_to_point_angle_err",   (void *)&CP_max_go_to_point_angle_err, V_INT},
    {"max_int_lookahead",           (void *)&CP_max_int_lookahead,  V_INT},
    {"intercept_close_dist",        (void *)&CP_intercept_close_dist,  V_FLOAT},
    {"intercept_step",              (void *)&CP_intercept_step,     V_INT},	
    {"my_intercept_step",           (void *)&CP_my_intercept_step,  V_INT},	
    {"intercept_aim_ahead",         (void *)&CP_intercept_aim_ahead, V_INT},
    {"no_turn_max_cyc_diff",        (void *)&CP_no_turn_max_cyc_diff, V_INT},
    {"no_turn_max_dist_diff",       (void *)&CP_no_turn_max_dist_diff, V_FLOAT},

    {"turnball_opp_worry_dist",     (void *)&CP_turnball_opp_worry_dist,V_FLOAT},
    {"collision_buffer",            (void *)&CP_collision_buffer,  V_FLOAT},
    {"behind_angle",                (void *)&CP_behind_angle, V_FLOAT},
    {"ball_vel_invalidation_factor",(void *)&CP_ball_vel_invalidation_factor, V_FLOAT},
    {"time_for_full_rotation",      (void *)&CP_time_for_full_rotation, V_INT},

    {"dribble_dash_pow",            (void *)&CP_dribble_dash_pow, V_INT},
    {"dribble_ball_dist",           (void *)&CP_dribble_ball_dist,  V_FLOAT},
    {"dribble_ignore_opp_dist",     (void *)&CP_dribble_ignore_opp_dist, V_FLOAT},
    {"dribble_worry_opp_dist",      (void *)&CP_dribble_worry_opp_dist, V_FLOAT},
    {"dribble_angle_norm",          (void *)&CP_dribble_angle_norm, V_FLOAT},
    {"dribble_dodge_max_dist",      (void *)&CP_dribble_dodge_max_dist, V_FLOAT},
    {"dribble_dodge_angle_err",     (void *)&CP_dribble_dodge_angle_err, V_FLOAT},
    {"dribble_exp_angle_buffer",    (void *)&CP_dribble_exp_angle_buffer, V_FLOAT},
    {"dribble_angle_ignore_buffer", (void *)&CP_dribble_angle_ignore_buffer, V_FLOAT},
    {"dribble_dodge_close_dist",    (void *)&CP_dribble_dodge_close_dist, V_FLOAT},
    {"dribble_scan_field",          (void *)&CP_dribble_scan_field, V_ONOFF},

    {"can_dribble_cone_ratio",      (void *)&CP_can_dribble_cone_ratio, V_FLOAT},
    {"dribble_towards_length",      (void *)&CP_dribble_towards_length, V_FLOAT},
    {"dribble_sideline_buffer",     (void *)&CP_dribble_sideline_buffer, V_FLOAT},
    {"dribble_circle_inner_rad",    (void *)&CP_dribble_circle_inner_rad, V_FLOAT},
    {"dribble_circle_outer_rad",    (void *)&CP_dribble_circle_outer_rad, V_FLOAT},
    {"dribble_circle_ang",          (void *)&CP_dribble_circle_ang, V_FLOAT},
 
    {"move_imp_1v1_initial",        (void *)&CP_move_imp_1v1_initial, V_FLOAT},
    {"move_imp_1v1_inc",            (void *)&CP_move_imp_1v1_inc, V_FLOAT},
    {"move_imp_1v1_threshold",      (void *)&CP_move_imp_1v1_threshold, V_FLOAT},
    {"at_point_buffer",             (void *)&CP_at_point_buffer, V_FLOAT},
    {"overrun_dist",                (void *)&CP_overrun_dist, V_FLOAT},
    {"def_block_dist",              (void *)&CP_def_block_dist, V_FLOAT},
    {"def_block_dist_ratio",        (void *)&CP_def_block_dist_ratio, V_FLOAT},
    {"overrun_buffer",              (void *)&CP_overrun_buffer, V_FLOAT},
    {"cycles_to_kick",              (void *)&CP_cycles_to_kick, V_FLOAT},
    {"breakaway_buffer",            (void *)&CP_breakaway_buffer, V_FLOAT},
    {"our_breakaway_kickable_buffer",   (void *)&CP_our_breakaway_kickable_buffer, V_FLOAT},
    {"their_breakaway_front_kickable_buffer",   (void *)&CP_their_breakaway_front_kickable_buffer, V_FLOAT},
    {"their_breakaway_back_kickable_buffer",   (void *)&CP_their_breakaway_back_kickable_buffer, V_FLOAT},
    {"goalie_breakaway_kickable_buffer",   (void *)&CP_goalie_breakaway_kickable_buffer, V_FLOAT},

    {"breakaway_approach_x",        (void *)&CP_breakaway_approach_x,  V_FLOAT},
    {"breakaway_approach_y",        (void *)&CP_breakaway_approach_y,  V_FLOAT},
    {"breakaway_targ_valid_time",   (void *)&CP_breakaway_targ_valid_time,       V_INT},
    {"breakaway_min_goalie_steal_time", (void *)&CP_breakaway_min_goalie_steal_time, V_INT},
    {"breakaway_kick_run_min_cycles",(void *)&CP_breakaway_kick_run_min_cycles, V_INT},
    {"breakaway_kick_run_max_cycles",(void *)&CP_breakaway_kick_run_max_cycles, V_INT},
    {"their_breakaway_min_cone_dist_wid",  (void *)&CP_their_breakaway_min_cone_dist_wid,     V_FLOAT},
    {"our_breakaway_min_cone_dist_wid",  (void *)&CP_our_breakaway_min_cone_dist_wid,     V_FLOAT},
    {"breakaway_middle_buffer",      (void *)&CP_breakaway_middle_buffer, V_FLOAT},
    {"breakaway_kick_run_worry_dist",(void *)&CP_breakaway_kick_run_worry_dist, V_FLOAT},
    {"breakaway_mode",              (void *)&CP_breakaway_mode, V_INT},

    {"static_kick_dist_err",        (void *)&CP_static_kick_dist_err, V_FLOAT},
    {"static_kick_ang_err",         (void *)&CP_static_kick_ang_err, V_FLOAT},
    /* no longer used
    {"static_kick_dist",            (void *)&CP_static_kick_dist, V_FLOAT},
    {"static_kick_ang",             (void *)&CP_static_kick_ang, V_FLOAT},
    {"static_kick_overrun_dist",    (void *)&CP_static_kick_overrun_dist, V_FLOAT},
    */
    
    {"goalie_baseline_buffer",      (void *)&CP_goalie_baseline_buffer, V_FLOAT},
    {"goalie_scan_angle_err",       (void *)&CP_goalie_scan_angle_err, V_FLOAT},
    {"goalie_at_point_buffer",      (void *)&CP_goalie_at_point_buffer, V_FLOAT},
    {"goalie_vis_angle_err",        (void *)&CP_goalie_vis_angle_err, V_FLOAT},
    {"goalie_max_shot_distance",    (void *)&CP_goalie_max_shot_distance, V_FLOAT},
    {"goalie_min_pos_dist",         (void *)&CP_goalie_min_pos_dist, V_FLOAT},
    {"goalie_max_pos_dist",         (void *)&CP_goalie_max_pos_dist, V_FLOAT},
    {"goalie_max_forward_percent",  (void *)&CP_goalie_max_forward_percent, V_FLOAT},
    {"goalie_ball_ang_for_corner",  (void *)&CP_goalie_ball_ang_for_corner, V_FLOAT},
    {"goalie_max_come_out_dist",    (void *)&CP_goalie_max_come_out_dist, V_FLOAT},
    {"goalie_ball_dist_for_corner", (void *)&CP_goalie_ball_dist_for_corner, V_FLOAT},
    {"goalie_ball_dist_for_center", (void *)&CP_goalie_ball_dist_for_center, V_FLOAT},
    {"goalie_free_kick_dist",       (void *)&CP_goalie_free_kick_dist, V_FLOAT},
    {"goalie_go_to_ball_cone_ratio",(void *)&CP_goalie_go_to_ball_cone_ratio, V_FLOAT},
    {"goalie_warn_space",           (void *)&CP_goalie_warn_space, V_INT},
    {"goalie_comes_out",            (void *)&CP_goalie_comes_out, V_ONOFF},
    {"goalie_catch_wait_time",      (void *)&CP_goalie_catch_wait_time, V_INT},
    {"goalie_opponent_dist_to_block", (void *)&CP_goalie_opponent_dist_to_block, V_FLOAT},
    {"goalie_position_weight_dist", (void *)&CP_goalie_position_weight_dist, V_FLOAT},
    {"goalie_narrow_sideline_cyc",  (void *)&CP_goalie_narrow_sideline_cyc,  V_INT},
    {"goalie_no_buffer_dist",       (void *)&CP_goalie_no_buffer_dist, V_FLOAT},
    {"goalie_wait_cyc",             (void *)&CP_goalie_wait_cyc, V_FLOAT}, 
    
    {"clear_ball_ang_step",         (void *)&CP_clear_ball_ang_step, V_FLOAT},
    {"clear_ball_cone_ratio",       (void *)&CP_clear_ball_cone_ratio, V_FLOAT},
    {"clear_ball_max_dist",         (void *)&CP_clear_ball_max_dist, V_FLOAT},
    {"clear_offensive_min_horiz_dist", (void *)&CP_clear_offensive_min_horiz_dist, V_FLOAT},
    {"clear_offensive_min_angle",   (void *)&CP_clear_offensive_min_angle, V_FLOAT},


    {"should_cross_corner_dist",    (void *)&CP_should_cross_corner_dist, V_FLOAT},
    {"should_cross_baseline_buffer",(void *)&CP_should_cross_baseline_buffer, V_FLOAT},
    {"should_move_to_cross_corner_dist", (void *)&CP_should_move_to_cross_corner_dist, V_FLOAT},
    {"cross_pt_x",                  (void *)&CP_cross_pt_x, V_FLOAT},
    {"cross_pt_y",                  (void *)&CP_cross_pt_y, V_FLOAT},
    {"cross_target_vel",            (void *)&CP_cross_target_vel, V_FLOAT},

    {"dont_dribble_to_middle_min_x",(void *)&CP_dont_dribble_to_middle_min_x, V_FLOAT},
    
  /* not used anymore
    {"hardest_kick_shot_distance",  (void *)&CP_hardest_kick_shot_distance, V_FLOAT},
    {"moderate_kick_shot_distance", (void *)&CP_moderate_kick_shot_distance, V_FLOAT},
    */
    {"good_shot_distance",          (void *)&CP_good_shot_distance, V_FLOAT},
    {"shot_distance",               (void *)&CP_shot_distance, V_FLOAT},
    {"cycles_to_kick_buffer",       (void *)&CP_cycles_to_kick_buffer, V_INT},
    {"shot_speed",                  (void *)&CP_shot_speed,    V_FLOAT},
    {"shot_goalie_react_buffer",      (void *)&CP_shot_goalie_react_buffer,      V_INT},
    {"good_shot_goalie_react_buffer", (void *)&CP_good_shot_goalie_react_buffer, V_INT},
    {"better_shot_cyc_diff",        (void *)&CP_better_shot_cyc_diff,            V_INT},
    //{"breakaway_shot_distance",     (void *)&CP_breakaway_shot_distance, V_FLOAT},
    
    {"formation",                   (void *)&FP_initial_formation,  V_STRING},
    {"formation_when_losing",       (void *)&FP_formation_when_losing,  V_STRING},
    {"formation_when_losing_lots",  (void *)&FP_formation_when_losing_lots,  V_STRING},
    {"formation_when_winning",      (void *)&FP_formation_when_winning,  V_STRING},
    {"formation_when_tied",         (void *)&FP_formation_when_tied,  V_STRING},
    
    {"home_change",                 (void *)&FP_initial_hc_method,  V_STRING},
    {"mark_change",                 (void *)&FP_initial_mc_method,  V_STRING},
    {"player_1_pos",                (void *)&FP_initial_player_1_pos,  V_INT},
    {"player_2_pos",                (void *)&FP_initial_player_2_pos,  V_INT},
    {"player_3_pos",                (void *)&FP_initial_player_3_pos,  V_INT},
    {"player_4_pos",                (void *)&FP_initial_player_4_pos,  V_INT},
    {"player_5_pos",                (void *)&FP_initial_player_5_pos,  V_INT},
    {"player_6_pos",                (void *)&FP_initial_player_6_pos,  V_INT},
    {"player_7_pos",                (void *)&FP_initial_player_7_pos,  V_INT},
    {"player_8_pos",                (void *)&FP_initial_player_8_pos,  V_INT},
    {"player_9_pos",                (void *)&FP_initial_player_9_pos,  V_INT},
    {"player_10_pos",               (void *)&FP_initial_player_10_pos, V_INT},
    {"player_11_pos",               (void *)&FP_initial_player_11_pos, V_INT},
    {"goalie_number",               (void *)&FP_goalie_number        , V_INT},

    {"version",        		(void *)&SP_version,          V_FLOAT},
    {"team_size",        		(void *)&SP_team_size,          V_INT},
    {"half",        		(void *)&SP_half,          V_INT},
    {"host",             		(void *)&SP_host,               V_STRING},
    {"goal_width",   		(void *)&SP_goal_width,         V_FLOAT},
    {"player_size",   		(void *)&SP_player_size,	V_FLOAT},
    {"player_decay",   	        (void *)&SP_player_decay,    	V_FLOAT},
    {"player_rand",   		(void *)&SP_player_rand,	V_FLOAT},
    {"player_weight",   	        (void *)&SP_player_weight,	V_FLOAT},
    {"player_speed_max",            (void *)&SP_player_speed_max,	V_FLOAT},
    {"stamina_max",   		(void *)&SP_stamina_max,	V_FLOAT},
    {"stamina_inc_max",   	        (void *)&SP_stamina_inc,	V_FLOAT},
    {"recover_dec_thr",   	        (void *)&SP_recover_dec_thr,	V_FLOAT},
    {"recover_min",   		(void *)&SP_recover_min, 	V_FLOAT},
    {"recover_dec",   		(void *)&SP_recover_dec, 	V_FLOAT},
    {"effort_dec_thr",   	        (void *)&SP_effort_dec_thr,	V_FLOAT},
    {"effort_min",   		(void *)&SP_effort_min, 	V_FLOAT},
    {"effort_dec",   		(void *)&SP_effort_dec, 	V_FLOAT},
    {"effort_inc_thr",   	        (void *)&SP_effort_inc_thr,	V_FLOAT},
    {"effort_inc",   		(void *)&SP_effort_inc,	        V_FLOAT},
    {"ball_size",   		(void *)&SP_ball_size,		V_FLOAT},
    {"ball_decay",   		(void *)&SP_ball_decay, 	V_FLOAT},
    {"ball_rand",   		(void *)&SP_ball_rand,		V_FLOAT},
    {"ball_weight",   		(void *)&SP_ball_weight,	V_FLOAT},
    {"ball_speed_max",   	        (void *)&SP_ball_speed_max,	V_FLOAT},
    {"dash_power_rate",   	        (void *)&SP_dash_power_rate,	V_FLOAT},
    {"kick_power_rate",   	        (void *)&SP_kick_power_rate,	V_FLOAT},
    {"kickable_margin",   	        (void *)&SP_kickable_margin,	V_FLOAT},
    {"catch_probability",   	(void *)&SP_catch_prob, 	V_FLOAT},
    {"catchable_area_l",            (void *)&SP_catch_area_l,	V_FLOAT},
    {"catchable_area_w",            (void *)&SP_catch_area_w,	V_FLOAT},
    {"maxpower",   		        (void *)&SP_max_power,		V_FLOAT},
    {"minpower",   		        (void *)&SP_min_power,		V_FLOAT},
    {"maxmoment",   		(void *)&SP_max_moment,		V_FLOAT},
    {"minmoment",   		(void *)&SP_min_moment,		V_FLOAT},
    {"maxneckang",		(void *)&SP_max_neck_angle,		V_FLOAT},
    {"minneckang",		(void *)&SP_min_neck_angle,		V_FLOAT},
    {"maxneckmoment",		(void *)&SP_max_neck_moment,		V_FLOAT},
    {"minneckmoment",		(void *)&SP_min_neck_moment,		V_FLOAT},
    {"visible_angle",   	        (void *)&SP_visible_angle,	V_FLOAT},
    {"visible_distance",            (void *)&SP_visible_dist,	V_FLOAT},
    {"audio_cut_dist",   	        (void *)&SP_audio_cut_dist,	V_FLOAT},
    {"quantize_step",   	        (void *)&SP_dist_qstep, 	V_FLOAT},
    {"quantize_step_l",   	        (void *)&SP_land_qstep, 	V_FLOAT},
    {"ckick_margin",   	        (void *)&SP_ckmargin,   	V_FLOAT},
    {"wind_dir",   		        (void *)&SP_wind_dir,   	V_FLOAT},
    {"wind_force",   		(void *)&SP_wind_force,  	V_FLOAT},
    {"wind_rand",   		(void *)&SP_wind_rand,  	V_FLOAT},
    {"wind_none",   		(void *)&SP_wind_none,  	V_ONOFF},
    {"wind_random",   		(void *)&SP_wind_random,	V_ONOFF},
    {"half_time",   		(void *)&SP_half_time,  	V_INT},
    {"port",   			(void *)&SP_port,       	V_INT},
    {"coach_port",   		(void *)&SP_coach_port,  	V_INT},
    {"olcoach_port",	        (void *)&SP_olcoach_port,	V_INT},
    {"simulator_step",   	        (void *)&SP_simulator_step,	V_INT},
    {"send_step",   		(void *)&SP_send_step,  	V_INT},
    {"recv_step",   		(void *)&SP_recv_step,  	V_INT},
    {"say_msg_size",   	        (void *)&SP_say_msg_size,	V_INT},
    {"hear_max",   		        (void *)&SP_hear_max,   	V_INT},
    {"hear_inc",   		        (void *)&SP_hear_inc,   	V_INT},
    {"hear_decay",   		(void *)&SP_hear_decay,  	V_INT},
    {"catch_ban_cycle",             (void *)&SP_catch_ban_cycle,	V_INT},
    {"coach",   			(void *)&SP_coach_mode,  	V_ONOFF},
    {"coach_w_referee",   	        (void *)&SP_coach_w_referee_mode,V_ONOFF},
    {"say_coach_cnt_max",               (void *)&SP_say_coach_cnt_max,V_INT},
    {"say_coach_msg_size",          (void *)&SP_say_coach_msg_size,V_INT},
    {"send_vi_step",                (void *)&SP_send_vi_step, V_INT},

    {"look_step",                       (void *)&SP_look_step,V_INT},
    {"use_offside",   		(void *)&SP_use_offside,	V_ONOFF},
    {"forbid_kick_off_offside",     (void *)&SP_forbid_kickoff_offside, V_ONOFF},
    {"log_file",		        (void *)&SP_logfile,		V_STRING},
    {"record",			(void *)&SP_recfile,		V_STRING},
    {"record_log",		        (void *)&SP_rec_log,		V_ONOFF},
    {"record_version",	        (void *)&SP_rec_ver,		V_INT},
    {"send_log",	        (void *)&SP_send_log,		V_ONOFF},
    {"replay",			(void *)&SP_replay,		V_STRING},
    {"verbose",                     (void *)&SP_verbose,		V_ONOFF},
    {"offside_active_area_size",    (void *)&SP_offside_area,	V_FLOAT},
    {"inertia_moment",      	(void *)&SP_inertia_moment,     V_FLOAT},
    {"sense_body_step",      	(void *)&SP_sense_body_step,     V_INT},
    {"offside_kick_margin",     (void *)&SP_offside_kick_margin,   V_FLOAT},
    {"slow_down_factor",        (void *)&SP_slow_down_factor,   V_INT},    /*new in 6.06 */
    {"record_messages",         (void *)&SP_record_messages,     V_ONOFF},
    {"goalie_max_moves",          (void *)&SP_goalie_max_moves,  V_INT},   /* New in 6.00*/
    /*Player Params for heterogen players in 7.00 */
    { "player_types",			(void *)&CP_player_types,			V_INT },
    { "subs_max",				(void *)&CP_subs_max, 			V_INT },		
    { "pt_max",				(void *)&CP_pt_max, 			V_INT },		
    { "player_speed_max_delta_min",		(void *)&CP_player_speed_max_delta_min,	V_FLOAT },
    { "player_speed_max_delta_max",		(void *)&CP_player_speed_max_delta_max,	V_FLOAT },
    { "stamina_inc_max_delta_factor",	(void *)&CP_stamina_inc_max_delta_factor,	V_FLOAT },
    { "player_decay_delta_min",		(void *)&CP_player_decay_delta_min,	V_FLOAT },
    { "player_decay_delta_max",		(void *)&CP_player_decay_delta_max,	V_FLOAT },
    { "inertia_moment_delta_factor",	(void *)&CP_inertia_moment_delta_factor,	V_FLOAT },
    { "dash_power_rate_delta_min",		(void *)&CP_dash_power_rate_delta_min,	V_FLOAT },
    { "dash_power_rate_delta_max",		(void *)&CP_dash_power_rate_delta_max,	V_FLOAT },
    { "player_size_delta_factor",		(void *)&CP_player_size_delta_factor,	V_FLOAT },
    { "kickable_margin_delta_min",		(void *)&CP_kickable_margin_delta_min,	V_FLOAT },
    { "kickable_margin_delta_max",		(void *)&CP_kickable_margin_delta_max,	V_FLOAT },
    { "kick_rand_delta_factor",		(void *)&CP_kick_rand_delta_factor,	V_FLOAT },
    { "extra_stamina_delta_min",		(void *)&CP_extra_stamina_delta_min,	V_FLOAT },
    { "extra_stamina_delta_max",		(void *)&CP_extra_stamina_delta_max,	V_FLOAT },
    { "effort_max_delta_factor",		(void *)&CP_effort_max_delta_factor,	V_FLOAT },
    { "effort_min_delta_factor",		(void *)&CP_effort_min_delta_factor,	V_FLOAT },
    /*PlayerParams that are new, but that I don't since when */
    { "player_accel_max",		        (void *)&SP_player_accel_max,	V_FLOAT },
    { "baccel_max",		                (void *)&SP_baccel_max,	V_FLOAT },
    { "kick_rand",                              (void *)&SP_kick_rand, V_FLOAT  },
    { "clang_win_size",                         (void *)&SP_clang_win_size, V_INT },
    { "clang_define_win",                       (void *)&SP_clang_define_win, V_INT },
    { "clang_meta_win",                         (void *)&SP_clang_meta_win, V_INT },
    { "clang_advice_win",                       (void *)&SP_clang_advice_win, V_INT },
    { "clang_info_win",                         (void *)&SP_clang_info_win, V_INT },
    { "clang_mess_delay",                       (void *)&SP_clang_mess_delay, V_INT },
    { "clang_mess_per_cycle",                   (void *)&SP_clang_mess_per_cycle, V_INT },
    { "log_times",                              (void *)&SP_log_times, V_ONOFF }, 
    
    
    {"\0",				NULL, 			       	0}
  } ;

  /* skip command name */
  argv++ ; argc-- ;

  /* first, search option '-file' */
  int i ;
  FILE *fp ;
  for(i = 0 ; i < argc ; i++) {
    if (!strcmp(*(argv + i),"-file")) {
      if ((fp = fopen(*(argv+i+1),"r")) == NULL) {
	cerr << "can't open config file " << *(argv+i+1) << endl ;
	MAKELOG((0, DBG_ALL, "can't open config file %s", *(argv+i+1)));
	exit(2);
      }
			
      char buf[100] ;
      while(fgets(buf,100,fp) != NULL) {
				/* ignore remark line */
	if (buf[0] == '#' || buf[0] == '\n')
	  continue ;

				/* replace from ':' to ' ' */
	char *t = buf ;
	while(*t != NULLCHAR) {
	  if (*t == ':') *t = ' ' ;
	  t++ ;
	}

	int n, p ;
	char com[256] ;
	char onoff[16] ;
	n = sscanf(buf,"%s", com) ;
	if (n < 1) {
	  cerr << "Illegal line : " << buf ;
	  continue ;
	}

	for (p = 0 ; opt[p].vptr != NULL ; p++) {
	  if (strcmp(com, opt[p].optname))
	    continue ;

	  /* match */
	  switch(opt[p].vsize) {
	  case V_INT:
	    n = sscanf(buf, "%s %d", com, (int *)opt[p].vptr) ;
	    break ;

	  case V_STRING:
	    n = sscanf(buf, "%s %s", com, (char *)opt[p].vptr) ;
	    break ;

	  case V_FLOAT:
	    n = sscanf(buf, "%s %f", com, (float *)opt[p].vptr) ;
	    break ;

	  case V_BOOL:
	    n = 2 ;
	    *((Bool *)opt[p].vptr) = TRUE ;
	    break ;

	  case V_ONOFF:
	    n = sscanf(buf, "%s %s", com, onoff) ;
	    if (n == 1) {
	      n = 2;
	      *((Bool *)opt[p].vptr) = TRUE;
	    } else
	      *((Bool *)opt[p].vptr) = (!strcmp(onoff, "on")) ? TRUE :FALSE;
	    break ;
	  }

	  if (n < 2)
	    cerr << "Illegal line (" << com << ") " << endl ;

	  break ;
	}
				
	if (opt[p].vptr == NULL)
	  cerr << "Illegal line (" << com << ") " << endl ;
      }

      fclose(fp) ;
      /* break ; */   // Without this, more than one file can be read
    }
  }

  /* next, analyze command line option */
  int p ;

  while (argc) {
    if (!strcmp(*argv, "-file")) {
      argv += 2 ;
      argc -= 2 ;
      continue ;
    }
		 	
    if (!strcmp(*argv, "-debug_fac")) {
      argv++ ;
      argc-- ;
      if(sscanf(*argv, "%d", &numoption)!=1)
      {
	if(!strcmp(*argv, "DBG_CMU")) numoption=DBG_CMU;
	else
	if(!strcmp(*argv, "DBG_OFFENSE")) numoption=DBG_OFFENSE;
	else
	if(!strcmp(*argv, "DBG_DEFENSE")) numoption=DBG_DEFENSE;
	else
	if(!strcmp(*argv, "DBG_GOALIE")) numoption=DBG_GOALIE;
	else
	if(!strcmp(*argv, "DBG_COMM")) numoption=DBG_COMM;
	else
	if(!strcmp(*argv, "DBG_COACH")) numoption=DBG_COACH;
	else
	if(!strcmp(*argv, "DBG_TRAINER")) numoption=DBG_TRAINER;
	else
	if(!strcmp(*argv, "DBG_FORM")) numoption=DBG_FORM;
	else
	if(!strcmp(*argv, "DBG_SFL")) numoption=DBG_SFL;
	else
	if(!strcmp(*argv, "DBG_OTHER")) numoption=DBG_OTHER;
        fprintf(stderr, "invalid value %s\n", *argv);
        numoption=DBG_ANY;
      }
      logparameter(numoption, -1);
      argv++ ;
      argc-- ;
      continue ;
    }

    if (!strcmp(*argv, "-debug_lev")) {
      argv++ ;
      argc-- ;
      if(sscanf(*argv, "%d", &numoption)!=1)
      {
        fprintf(stderr, "invalid value %s\n", *argv);
        numoption=99;
      }
      logparameter(-1, numoption);
      argv++ ;
      argc-- ;
      continue ;
    }

    if (!strcmp(*argv, "-debug_file")) {
      argv++ ;
      argc-- ;
      logFileName(*argv);
      argv++ ;
      argc-- ;
      continue ;
    }

    for (p = 0 ; opt[p].vptr != NULL ; p++) {
      if (strcmp(*argv + 1, opt[p].optname))
	continue ;

      /* match */
      argv++ ;
      argc-- ;

      switch(opt[p].vsize) {
      case V_INT:
	*((int *)opt[p].vptr) = atoi(*argv) ;
	break ;

      case V_STRING:
	strcpy((char *)opt[p].vptr, *argv) ;
	break ;

      case V_FLOAT:
	*((float *)opt[p].vptr) = atof(*argv) ;
	break ;

      case V_BOOL:
	*((Bool *)opt[p].vptr) = TRUE ;
	argv-- ;
	argc++ ;
	break ;

      case V_ONOFF:
	if (argc > 0 && (*argv)[0] != '-') {	  
	  *((Bool *)opt[p].vptr) = (!strcmp(*argv, "on")) ? TRUE : FALSE ;
	} else {
	  /* if there's nothing specified, then we set it to true */
	  *((Bool *)opt[p].vptr) = TRUE;
	  argv-- ;
	  argc++ ;
	}
	break ;
      }

      break ;
    }

    if (opt[p].vptr == NULL)
      cerr << "Unrecognized Option : " << *argv << endl ;

    argv++ ;
    argc-- ;
  }

  SP_half_time = SP_half_time * 1000 / SP_simulator_step ;
  SP_kickable_area = SP_kickable_margin + SP_ball_size + SP_player_size ;
}


/* explode the line into argc and argv */
void OptionInfo::GetOptions(char* line) 
{
  const int MAXOPT = 100;
  char* argv[MAXOPT];
  int argc = 1; /* executable name */
  char* pc;

  advance_past_space(&line);
  while (*line != 0) {
    pc = line;
    get_token(&line);
    argv[argc] = new char[line-pc+1];
    strncpy(argv[argc], pc, line-pc);
    argv[argc][line-pc] = 0; /* null terminate */
    argc++;
    advance_past_space(&line);
  }

  argv[argc] = NULL;

  GetOptions(argc, argv);

  for (int i = 1; i<argc; i++)
    delete [] argv[i];
}



void OptionInfo::ParsePlayerParam(char* message)
{


/*	sscanf(message, "(player_param %d %d %d "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf "
				      "%lf %lf %lf %lf %lf %lf)",
	// & variables	
	& CP_player_types ,
	& CP_subs_max ,
	& CP_pt_max ,
    
	& CP_player_speed_max_delta_min ,
	& CP_player_speed_max_delta_max ,
	& CP_stamina_inc_max_delta_factor ,
    
	& CP_player_decay_delta_min ,
	& CP_player_decay_delta_max ,
	& CP_inertia_moment_delta_factor ,

	& CP_dash_power_rate_delta_min ,
	& CP_dash_power_rate_delta_max ,
	& CP_player_size_delta_factor ,

	& CP_kickable_margin_delta_min ,
	& CP_kickable_margin_delta_max ,
	& CP_kick_rand_delta_factor ,

	& CP_extra_stamina_delta_min ,
	& CP_extra_stamina_delta_max ,
	& CP_effort_max_delta_factor ,
	& CP_effort_min_delta_factor);
*/

}

void OptionInfo::ParseServerParam(char* message)
{
/*	sscanf(message, "(server_param %lf %lf %lf %lf %lf "
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
	& SP_goal_width , 					
	& SP_inertia_moment , 	
	& SP_player_size , 			
	& SP_player_decay , 			
	& SP_player_rand , 			
	& SP_player_weight , 				
	& SP_player_speed_max ,
	 & SP_player_accel_max ,
        & SP_stamina_max ,
        & SP_stamina_inc ,
        & SP_recover_init ,
        & SP_recover_dec_thr ,
        & SP_recover_min ,
        & SP_recover_dec ,
        & SP_effort_init ,
        & SP_effort_dec_thr ,
        & SP_effort_min ,
        & SP_effort_dec ,
        & SP_effort_inc_thr ,
        & SP_effort_inc ,
        & SP_kick_rand ,
        & SP_team_actuator_noise ,
        & SP_prand_factor_l ,
        & SP_prand_factor_r ,
        & SP_kick_rand_factor_l ,
        & SP_kick_rand_factor_r ,
        & SP_ball_size ,
        & SP_ball_decay ,
        & SP_ball_rand ,
        & SP_ball_weight ,
        & SP_ball_speed_max ,
        & SP_ball_accel_max ,
        & SP_dash_power_rate ,
        & SP_kick_power_rate ,
        & SP_kickable_margin ,
        & SP_ctlradius ,
        & SP_ctlradius_width ,
        & SP_max_power ,
        & SP_min_power ,
        & SP_max_movement ,
        & SP_min_movement ,
        & SP_max_neck_movement ,
        & SP_min_neck_movement ,
        & SP_max_neck_angle ,
        & SP_min_neck_angle ,
        & SP_visible_angle ,
        & SP_visible_dist ,
        & SP_wind_dir ,
        & SP_wind_force ,
        & SP_wind_angle ,
        & SP_wind_rand ,
        & SP_kickable_area ,
        & SP_catch_area_l ,
        & SP_catch_area_w ,
        & SP_catch_prob ,
        & SP_goalie_max_moves ,
        & SP_ckmargin ,
        & SP_offside_area ,
 & SP_wind_none ,
        & SP_wind_random ,
        & SP_say_coach_cnt_max ,
        & SP_say_coach_msg_size ,
        & SP_clang_win_size ,
        & SP_clang_define_win ,
        & SP_clang_meta_win ,
        & SP_clang_advice_win ,
        & SP_clang_info_win ,
        & SP_clang_mess_delay ,
        & SP_clang_mess_per_cycle ,
        & SP_half_time ,
        & SP_simulator_step ,
        & SP_send_step ,
        & SP_recv_step ,
        & SP_sense_body_step ,
        & SP_lcm_st ,
        & SP_say_msg_size ,
        & SP_hear_max ,
        & SP_hear_inc ,
        & SP_hear_decay ,
        & SP_catch_ban_cycle ,
        & SP_slow_down_factor ,
        & SP_use_offside ,
        & SP_forbid_kickoff_offside ,

        & SP_offside_kick_margin ,
        & SP_audio_cut_dist ,
        & SP_dist_qstep ,
        & SP_land_qstep ,
        & SP_dir_qstep ,
        & SP_dist_qstep_l ,
        & SP_dist_qstep_r ,
        & SP_land_qstep_l ,
        & SP_land_qstep_r ,
        & SP_dir_qstep_l ,
        & SP_dir_qstep_r ,
        & SP_coach_mode ,
        & SP_coach_w_referee_mode,
        & SP_old_hear ,
        & SP_simulator_step
		
	); // end of sscanf
*/
};


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
 }

