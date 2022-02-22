CREATE TABLE versions (coach float8);
INSERT INTO versions (coach) VALUES (1.0);

CREATE TABLE indices (time_frame int,
	              pos int,
		      pos_list int,
                      action int,
                      action_list int,
                      player int,
                      player_list int
                      );
INSERT INTO indices (time_frame, pos, pos_list, action, action_list, player, player_list)
        VALUES (1, 1, 1, 1, 1, 1, 1);

CREATE TABLE time_frame (id int,
                         ball_pos int,    /* position of the ball: pointer to pos table */
                         team int,        /* the team controlling the ball */
                         nearest int,     /* players near the ball: pointer to player_lists table */
			 pass int,        /* cycles till a pass happens */
			 goal_try int,    /* cycles till a goal is tried */
			 lose_ball int,   /* cycles till the ball is lost */
                         pred int,        /* pointer to predecessor timeframe (selfreference) */
                         action int,      /* player actions: pointer to action_lists table */
			 pos int          /* player positions: pointer to pos_lists table */
			 );
CREATE UNIQUE INDEX time_frame_id ON time_frame (id);

CREATE TABLE pos (id int,
		  x float8,               /* x-coordinate of a position */
		  y float8	          /* y-coordinate of a position */
		  );
CREATE UNIQUE INDEX pos_id ON pos (id);

CREATE TABLE pos_list (id int,
		       pos int            /* pointer to table pos */
                      );

CREATE TABLE action (id int,
                     action int,          /* the taken action (none, dash, kick) */
		     power float8,        /* power of the taken action */
		     direction float8     /* direction of the taken action (if appropriate) */
		     );
CREATE UNIQUE INDEX action_id ON action (id);

CREATE TABLE action_list (id int,
			  action int      /* pointer to action table */
			  );

CREATE TABLE player (id int,
		     side int,            /* side of the player (left, right, neutral) */
		     angle float8,        /* angle to ball */
		     distance float8      /* distance to ball */
		     );
CREATE UNIQUE INDEX player_id ON player (id);

CREATE TABLE player_list (id int,
			  player int      /* pointer to player table */
			  );


