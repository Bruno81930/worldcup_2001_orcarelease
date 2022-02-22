//_____________________________________________________________________________
//
//	MemFormation.h
//
//	Sean Buttinger, 2001
//_____________________________________________________________________________

#ifndef _MEMFORMATION_H
#define _MEMFORMATION_H

#include "MemAction.h"

#define BUF_LEN 1024


class PlayerPosition
{
    public:

        // data
		Vector Home;
        float HomeRange;
        float MaxRange;

        // constructors
        PlayerPosition();
        PlayerPosition(Vector Home, float HomeRange, float MaxRange);

        // methods
        void SetPlayerPosition(Vector Home, float HomeRange, float MaxRange);

		void SetHome(Vector Home);
        void SetHomeRange(float HomeRange);
        void SetMaxRange (float MaxRange);

       	bool PointInHomeRange(Vector const &point);
		bool PointInMaxRange(Vector const &point);

		void Print();
};


class TeamFormation
{
    public:

        // data
        char name[BUF_LEN];
        PlayerPosition position[11];

        // create uniinitialized TeamFormation
        TeamFormation();

        // create TeamFormation, initialize from file
//		TeamFormation(char filename[], char formation_name[]);

        // read specified formation record from file
        bool ReadFormation(FILE* file, char formation_name[]);

        // read next formation record (used in TeamFormationArray)
		bool ReadNextFormation(FILE* file);

    private:

        // read next non-comment and non-empty line from file
        bool read_next_line(FILE* file, char* line);
};


class TeamFormationArray
{
    public:

        // data
        int num_formations;
        TeamFormation** formation;

        // create TeamFormationArray, initialize from file
        TeamFormationArray(char filename[]);

        // return TeamFormation with specified name
        TeamFormation* GetFormation(char name[]);

	//<COMMUNICATION>

        // return ID of TeamFormation with specified name
        int GetFormationID(char name[]);

        // return TeamFormation with specified id
        TeamFormation* GetFormation(int id);

  	//</COMMUNICATION>
};


class FormationInfo : public ActionInfo
{
	public:

    	// formation data

        TeamFormationArray* formations;

        TeamFormation* currentFormation;

        int currentPositionNumber;
        PlayerPosition* currentPosition;
        PlayerPosition* currentAdjustedPosition;

		//<COMMUNICATION>
        int currentFormationID;
  		Time currentFormationTime;
        //</COMMUNICATION>


		// formation methods

        void Initialize();

		void setCurrentFormation(char name[]);

		void setCurrentPosition(int number);

		void adjustCurrentPosition();

		void adjustCurrentHome();
		void adjustCurrentMaxRange();
		void adjustCurrentHomeRange();

		//<COMMUNICATION>
		void HearFormation(int formationID, Time formationTime, Unum sender, Time time);
		void SetCurrentFormation(int id);
                bool isCurrentFormation(char name[]);

		int GetCurrentFormationID(void);
		Time GetCurrentFormationTime(void);
        //</COMMUNICATION>

		int numOpponentsInMyHomeRange();
		int numOpponentsInMyMaxRange();
		int numOpponentsInCircle(const Vector &center, float radius);

		int opponentsInMyHomeRange(Unum opps[]);
		int opponentsInMyMaxRange(Unum opps[]);
		int opponentsInCircle(Unum opps[], const Vector &center, float radius);

		int numTeammatesInMyHomeRange();
		int numTeammatesInMyMaxRange();
		int numTeammatesInCircle(const Vector &center, float radius);

		int teammatesInMyHomeRange(Unum mates[]);
		int teammatesInMyMaxRange(Unum mates[]);
		int teammatesInCircle(Unum mates[], const Vector &center, float radius);

		bool ballInMyMaxRange();
		bool ballInMyHomeRange();

		bool amInMyHomeRange();
		bool amInMyMaxRange();

		bool pointInMyMaxRange(const Vector &point);
		bool pointInMyHomeRange(const Vector &point);
};

#endif
