//include <iostream>
#include "client.h"
#include "geometry.h"

#define LONGBITS 30
#define MIN_CONF 0.90
#define MAX_UPDATED_OPPONENTS 0
#define MAX_UPDATED_MATES     11
#ifndef __MESSAGE__
#define __MESSAGE__
namespace communication {

    class MateMessage;
    
    class MessageBase 
		{
		public:
			virtual int read_from_stream (int startPos, long *data) = 0;
			virtual int write_to_stream (int startPos, long *data) = 0;
		protected:
			int bitCountM;
		};

    class RawDataMessage : public MessageBase
		{
		public:
			RawDataMessage(int bitCount);
			virtual int read_from_stream(int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void setRawData(unsigned long data) { rawDataM = data ; }
			unsigned long getRawData(void) { return rawDataM; }
		protected:
			unsigned long rawDataM;
		};
    
    class FloatMessage : public RawDataMessage 
		{
		public:
			FloatMessage (int bitCount, float min, float max) ;
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			void setValue(float value) { valueM = value; }
			float getValue(void) { return valueM; }
	 
			float minValueM;
			float maxValueM;
			float valueM;
		};
	 
    class IntegerMessage : public RawDataMessage 
		{
		public:
			IntegerMessage (int bitCount) ;
			IntegerMessage (int bitCount, float min, float max) ;

			void toError();

			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void setValue(int value) { valueM = value; }
			int getValue(void) { return valueM; }
	 
			float minValueM;
			float maxValueM;
			int valueM;
		};
	   
    class VectorMessage : public MessageBase
		{
		public:
			VectorMessage(int xBits, int yBits, float xMin, float xMax, float yMin, float yMax)
				: xM(xBits, xMin, xMax), yM(yBits, yMin, yMax) {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			void setX(float x) { xM.setValue(x); }
			void setY(float y) { yM.setValue(y); }
			float getX(void) { return xM.getValue(); }
			float getY(void) { return yM.getValue(); }
	
			FloatMessage xM, yM;
		};
    
    class ConfidenceMessage : public MessageBase
		{
		public:
			ConfidenceMessage()
				: confM(9, 0, 1){}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			float getConf(){ return confM.getValue(); }
			void setConf(float conf) { confM.setValue(conf); }
	 
			FloatMessage confM;
		};

    class AngleMessage : public MessageBase
		{
		public:
			AngleMessage()
				:  angleM(9, -180, 180), confM(){}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			AngleDeg getAngle(){ return angleM.getValue(); }
			void setAngle(AngleDeg angle) { angleM.setValue(angle); }
			float getConf(){ return confM.getConf(); }
			void setConf(float conf) { confM.setConf(conf); }
	 
			FloatMessage angleM;
			ConfidenceMessage confM;
		};

    class TimeStampMessage : public MessageBase
		{
		public:
			TimeStampMessage()
				: timeStampSM(16, 0, 65535), timeStampTM(16, 0, 65535){}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			Time getTimeStamp(){ return Time(timeStampTM.getValue(),timeStampSM.getValue()); }
			void setTimeStamp(Time timeStamp) 
				{ 
					timeStampSM.setValue(timeStamp.s); 
					timeStampTM.setValue(timeStamp.t);
				}
	 
			IntegerMessage timeStampSM;
			IntegerMessage timeStampTM;
		};

    class UnumMessage : public MessageBase
		{
		public:
			UnumMessage()
				: unumM(4, 0, 15) {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			Unum getUnum(){ return unumM.getValue(); }
			void setUnum(Unum unum) { unumM.setValue(unum); }
	 
			IntegerMessage unumM;
		};

    class HeaderMessage : public MessageBase
		{
		public:
			HeaderMessage()
				: timeStampM(), unumM(), magicM(8) {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			void update_message_from_memory();
			void update_memory_from_message(MateMessage sender, Time time);

			int getMagic(){ return magicM.getValue(); }
			void setMagic(int magic){ magicM.setValue(magic); }
			Time getTimeStamp(){ return timeStampM.getTimeStamp(); }
			void setTimeStamp(Time timeStamp) { timeStampM.setTimeStamp(timeStamp); }
			Unum getUnum() { return unumM.getUnum(); }
			void setUnum(Unum unum) { unumM.setUnum(unum); }
			TimeStampMessage timeStampM;
			UnumMessage unumM;
			IntegerMessage magicM;
		};
    
    
    class PositionMessage : public MessageBase
		{
		public:
			PositionMessage()
				: confM(), posM(10, 9, -55, 55, -35, 35){}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			Vector getPos(void) {
				return Vector(posM.getX(), posM.getY());
			}
			void setPos(Vector pos) {
				posM.setX(pos.x);
				posM.setY(pos.y);
			}
			float getConf(){ return confM.getConf(); }
			void setConf(float conf) { confM.setConf(conf); }
	 
			ConfidenceMessage confM;
			VectorMessage posM;
		};

    class VelocityMessage : public MessageBase
		{
		public:
			VelocityMessage()
				: confM(), velM(9, 9, -2.7, 2.7, -2.7, 2.7){}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			Vector getVel(void) {
				return Vector(velM.getX(), velM.getY());
			}
			void setVel(Vector vel) {
				velM.setX(vel.x);
				velM.setY(vel.y);
			}
			float getConf(){ return confM.getConf(); }
			void setConf(float conf) { confM.setConf(conf); }
	 
			ConfidenceMessage confM;
			VectorMessage velM;
		};

    class StaminaMessage : public MessageBase
		{
		public:
			StaminaMessage()
				: confM(), 
				staminaM(5, 0, 3500) {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			int getStamina(void) 
				{ return staminaM.getValue(); }
			void setStamina(float stamina) 
				{ staminaM.setValue(stamina); }
			float getConf()
				{ return confM.getConf(); }
			void setConf(float conf) 
				{ confM.setConf(conf); }
	 
			ConfidenceMessage confM;
			IntegerMessage staminaM;
		};


    class PlayerTypeMessage : public MessageBase
		{
		public:
			PlayerTypeMessage()
				: confM(), pTypeM(3, 0, 7) {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			int getPlayerType(void) 
				{ return pTypeM.getValue(); }
			void setPlayerType(int ptype) 
				{ pTypeM.setValue(ptype); }
			float getConf()
				{ return confM.getConf(); }
			void setConf(float conf) 
				{ confM.setConf(conf); }
	 
			ConfidenceMessage confM;
			IntegerMessage pTypeM;
		};


    class OffenseMessage : public MessageBase
		{
		public:
			OffenseMessage()
				: distM(10, 0, 110), timeM(), confM(), offenseM(1, 0, 1) {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			int getOffense(void)
				{ return offenseM.getValue(); }
			void setOffense(int offense) 
				{ offenseM.setValue(offense); }
			float getConf()
				{ return confM.getConf(); }
			void setConf(float conf) 
				{ confM.setConf(conf); }
			Time getTime()
				{ return timeM.getTimeStamp(); }
			void setTime(Time time)
				{ timeM.setTimeStamp(time); }
			float getDist()
				{ return distM.getValue(); }
			void setDist(float dist)
				{ distM.setValue(dist); }
	 
			FloatMessage distM;
			TimeStampMessage timeM;
			ConfidenceMessage confM;
			IntegerMessage offenseM;
		};


    class FormationMessage : public MessageBase
		{
		public:
			FormationMessage()
				: timeM(), formationM(5, 0, 31) {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void toError();

			int getFormation(void) 
				{ return formationM.getValue(); }
			void setFormation(int formation) 
				{ formationM.setValue(formation); }
			Time getTimeStamp()
				{ return timeM.getTimeStamp(); }
			void setTimeStamp(Time time) 
				{ timeM.setTimeStamp(time); }
	    
			TimeStampMessage timeM;
			IntegerMessage formationM;
		};
    

    class PlayerMessage : public MessageBase
		{
		public:
			PlayerMessage()
				:  posM(), velM(), bodyAngleM(), neckAngleM() {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void update_message_from_memory(char side, Unum unum);
			void update_memory_from_message(MateMessage sender, Time time, char side, Unum player);

			void toError();


			Vector   getPos(void)      
				{ return posM.getPos(); }
			void  setPos(Vector pos) 
				{ posM.setPos(pos); }
			Vector getVel()         
				{ return velM.getVel(); }
			void  setVel(Vector vel)
				{ velM.setVel(vel); }
			AngleDeg getBodyAngle()      
				{ return bodyAngleM.getAngle(); }
			void  setBodyAngle(AngleDeg angle)
				{ bodyAngleM.setAngle(angle); }
			AngleDeg getNeckAngle()      
				{ return neckAngleM.getAngle(); }
			void  setNeckAngle(AngleDeg angle)
				{ neckAngleM.setAngle(angle); }

			float   getPosConf(void)      
				{ return posM.getConf(); }
			void  setPosConf(float conf) 
				{ posM.setConf(conf); }
			float getVelConf()         
				{ return velM.getConf(); }
			void  setVelConf(float conf)
				{ velM.setConf(conf); }
			float getBodyAngleConf()      
				{ return bodyAngleM.getConf(); }
			void  setBodyAngleConf(float conf)
				{ bodyAngleM.setConf(conf); }
			float getNeckAngleConf()      
				{ return neckAngleM.getConf(); }
			void  setNeckAngleConf(float angle)
				{ neckAngleM.setConf(angle); }

			PositionMessage posM;
			VelocityMessage velM;
			AngleMessage bodyAngleM;
			AngleMessage neckAngleM;
		};

    
    class MateMessage : public MessageBase
		{
		public:
			MateMessage()
				: playerM(), staminaM() {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void update_message_from_memory(Unum i);
			void update_memory_from_message(MateMessage sender, Time time, Unum player);

			void toError();

			Vector   getPos(void)      
				{ return playerM.getPos(); }
			void  setPos(Vector pos) 
				{ playerM.setPos(pos); }
			float   getPosConf(void)      
				{ return playerM.posM.getConf(); }
			void  setPosConf(float conf) 
				{ playerM.posM.setConf(conf); }
			Vector getVel()         
				{ return playerM.getVel(); }
			void  setVel(Vector vel)
				{ playerM.setVel(vel); }
			float getVelConf()         
				{ return playerM.velM.getConf(); }
			void  setVelConf(float conf)
				{ playerM.velM.setConf(conf); }
			AngleDeg getBodyAngle()      
				{ return playerM.getBodyAngle(); }
			void  setBodyAngle(AngleDeg angle)
				{ playerM.setBodyAngle(angle); }
			float getBodyAngleConf()      
				{ return playerM.bodyAngleM.getConf(); }
			void  setBodyAngleConf(float conf)
				{ playerM.bodyAngleM.setConf(conf); }
			AngleDeg getNeckAngle()      
				{ return playerM.getNeckAngle(); }
			void  setNeckAngle(AngleDeg angle)
				{ playerM.setNeckAngle(angle); }
			float getNeckAngleConf()      
				{ return playerM.neckAngleM.getConf(); }
			void  setNeckAngleConf(float angle)
				{ playerM.neckAngleM.setConf(angle); }
			int getStamina()      
				{ return staminaM.getStamina(); }
			void  setStamina(int stamina)
				{ staminaM.setStamina(stamina); }
			float getStaminaConf()      
				{ return staminaM.getConf(); }
			void  setStaminaConf(float stamina)
				{ staminaM.setConf(stamina); }
	 
			PlayerMessage playerM;
			StaminaMessage staminaM;
		};

    class OpponentMessage : public MessageBase
		{
		public:
			OpponentMessage()
				: playerM(), pTypeM() {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void update_message_from_memory(Unum i);
			void update_memory_from_message(MateMessage sender, Time time, Unum player);

			void toError();

			Vector   getPos(void)      
				{ return playerM.getPos(); }
			void  setPos(Vector pos) 
				{ playerM.setPos(pos); }
			float   getPosConf(void)      
				{ return playerM.posM.getConf(); }
			void  setPosConf(float conf) 
				{ playerM.posM.setConf(conf); }
			Vector getVel()         
				{ return playerM.getVel(); }
			void  setVel(Vector vel)
				{ playerM.setVel(vel); }
			float getVelConf()         
				{ return playerM.velM.getConf(); }
			void  setVelConf(float conf)
				{ playerM.velM.setConf(conf); }
			AngleDeg getBodyAngle()      
				{ return playerM.getBodyAngle(); }
			void  setBodyAngle(AngleDeg angle)
				{ playerM.setBodyAngle(angle); }
			float getBodyAngleConf()      
				{ return playerM.bodyAngleM.getConf(); }
			void  setBodyAngleConf(float conf)
				{ playerM.bodyAngleM.setConf(conf); }
			AngleDeg getNeckAngle()      
				{ return playerM.getNeckAngle(); }
			void  setNeckAngle(AngleDeg angle)
				{ playerM.setNeckAngle(angle); }
			float getNeckAngleConf()      
				{ return playerM.neckAngleM.getConf(); }
			void  setNeckAngleConf(float angle)
				{ playerM.neckAngleM.setConf(angle); }
			int getPlayerType()         
				{ return pTypeM.getPlayerType(); }
			void  setPlayerType(int pType)
				{ pTypeM.setPlayerType(pType); }
			float getPlayerTypeConf()         
				{ return pTypeM.getConf(); }
			void  setPlayerTypeConf(float conf)
				{ pTypeM.setConf(conf); }
	 
			PlayerMessage playerM;
			PlayerTypeMessage pTypeM;
		};

    class BallMessage : public MessageBase
		{
		public:
			BallMessage()
				:  posM(), velM(){}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void update_message_from_memory();
			void update_memory_from_message(MateMessage sender, Time time);

			void toError();

			Vector   getPos(void)      
				{ return posM.getPos(); }
			void  setPos(Vector pos) 
				{ posM.setPos(pos); }
			float getPosConf(void)      
				{ return posM.getConf(); }
			void  setPosConf(float conf) 
				{ posM.setConf(conf); }
			Vector getVel()         
				{ return velM.getVel(); }
			void  setVel(Vector vel)
				{ velM.setVel(vel); }
			float getVelConf()         
				{ return velM.getConf(); }
			void  setVelConf(float conf)
				{ velM.setConf(conf); }
	 
			PositionMessage posM;
			VelocityMessage velM;
		};

    class PassMessage : public MessageBase
		{
		public:
			PassMessage()
				:  timeM(), passpartnerM(), positionM() {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void update_message_from_memory();
			void update_memory_from_message(Unum sender, Time time);

			void toError();

			Unum getPasspartner(void)      
				{ return passpartnerM.getUnum(); }
			void  setPasspartner(Unum passpartner) 
				{ passpartnerM.setUnum(passpartner); }
			Time getTime(void)      
				{ return timeM.getTimeStamp(); }
			void  setTime(Time time) 
				{ timeM.setTimeStamp(time); }
			Vector getPosition()         
				{ return positionM.getPos(); }
			void  setPosition(Vector position)
				{ positionM.setPos(position); }

			TimeStampMessage timeM;
			UnumMessage passpartnerM;
			PositionMessage positionM;
		};

    class StrategyMessage : public MessageBase
		{
		public:
			StrategyMessage()
				:  formationM(), offenseM(), passM() {}
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void update_message_from_memory();
			void update_memory_from_message(Unum sender, Time time);

			void toError();

			int getFormation(void)      
				{ return formationM.getFormation(); }
			void  setFormation(int formation) 
				{ formationM.setFormation(formation); }
			Time getFormationTime(void)      
				{ return formationM.getTimeStamp(); }
			void  setFormationTime(Time time) 
				{ formationM.setTimeStamp(time); }
			int getOffense()         
				{ return offenseM.getOffense(); }
			void  setOffense(int offense)
				{ offenseM.setOffense(offense); }
			float getOffenseConf()         
				{ return offenseM.getConf(); }
			void  setOffenseConf(float conf)
				{ offenseM.setConf(conf); }
			FormationMessage formationM;
			OffenseMessage offenseM;
			PassMessage passM;
		};




    class OurTeamMessage : public MessageBase
		{
		public:
			OurTeamMessage()
				{}  
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void update_message_from_memory();
			void update_memory_from_message(MateMessage sender, Time time);

			void toError();
	 
			MateMessage getMate(Unum unum)      
				{ return matesM[unum -1]; }
			void setMate(Unum unum, PlayerInfo pInfo);

			MateMessage matesM[11];
		};


    class TheirTeamMessage : public MessageBase
		{
		public:
			TheirTeamMessage()
				{}  
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);
			void update_message_from_memory();
			void update_memory_from_message(MateMessage sender, Time time);

			void toError();
	    
			OpponentMessage getOpponent(Unum unum)      
				{ return opponentsM[unum -1]; }
			void setOpponent(Unum unum, PlayerInfo pInfo);

			OpponentMessage opponentsM[11];
		};


    class TheMessage : public MessageBase
		{
		public:
			TheMessage()
				: headerM(), ourTeamM(), theirTeamM(), ballM(), strategyM(){}  
			virtual int read_from_stream (int startPos, long *data);
			virtual int write_to_stream (int startPos, long *data);

			void update_message_from_memory();
			void update_memory_from_message();

			void toError();
	    
			HeaderMessage headerM;
			OurTeamMessage ourTeamM;
			TheirTeamMessage theirTeamM;
			BallMessage ballM;
			StrategyMessage strategyM;
		};


}
#endif
