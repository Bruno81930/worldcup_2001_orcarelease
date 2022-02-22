#include "message.h"

using namespace communication ;

#define MAGIC (Mem->MySide)

static int opponentsUpdated;
static int matesUpdated;

/**********************************************************************
 * RAWDATAMESSAGE
 **********************************************************************/
int RawDataMessage::read_from_stream(int startPos, long *data)
{
    int longStartPos = startPos / LONGBITS;
    int bitPos = startPos % LONGBITS;
    int currentLongPos = longStartPos;
    int currentBitPos = bitPos;
    rawDataM = 0;
    int i;
    if (currentBitPos + bitCountM >LONGBITS)
    {
		currentBitPos = 0;
		currentLongPos ++;
    }
    unsigned long tempData = data[currentLongPos];
    for (i=0; i<bitCountM; i++)
    {
		rawDataM = rawDataM >> 1;
		rawDataM |= ((tempData >> currentBitPos) & 1) << 31;
		currentBitPos++;
		if (currentBitPos > LONGBITS) 
		{
			currentBitPos = 0;
			currentLongPos++;
			tempData = data[currentLongPos];
		}
    }
    rawDataM = rawDataM >> (sizeof(rawDataM) * 8 - bitCountM);

    return (currentLongPos * LONGBITS + currentBitPos);
//    return (startPos + i);
}

int RawDataMessage::write_to_stream (int startPos, long *data)
{
    int longStartPos = startPos / LONGBITS;
    int bitPos = startPos % LONGBITS;
    int currentLongPos = longStartPos;
    int currentBitPos = bitPos;
    unsigned long tempData = 0;
    int i;
    if (currentBitPos + bitCountM >LONGBITS)
    {
		currentBitPos = 0;
		currentLongPos ++;
    }


    for (i = 0; i < bitCountM; i++)
    {
		tempData |= ((rawDataM >> i) & 1) << currentBitPos;
		currentBitPos++;
		if (currentBitPos > LONGBITS) 
		{
			data[currentLongPos] |= tempData;
			tempData = 0;
			currentLongPos++;
			currentBitPos = 0;
		}
    }
    data[currentLongPos] |= tempData;

    return (currentLongPos * LONGBITS + currentBitPos);
    
    //return (startPos + i);
}

RawDataMessage::RawDataMessage (int bitCount) 
{
    bitCountM = bitCount; 
}

/**********************************************************************
 * FLOATMESSAGE
 **********************************************************************/

/** Only data up to 30 bit precision can be handled safely.
 */
FloatMessage::FloatMessage (int bitCount, float min, float max) 
    :RawDataMessage(bitCount)
{
    minValueM = min;
    maxValueM = max;
}

/** returns the position of the next object in stream */
int FloatMessage::read_from_stream(int startPos, long *data)
{
    int pos = RawDataMessage::read_from_stream(startPos, data);
    valueM = (((float)rawDataM) / (float)((1 << bitCountM)-1)) * (maxValueM - minValueM) + minValueM;
    return pos;
}


/** returns the position of the next object in stream. 
 */
int FloatMessage::write_to_stream(int startPos, long *data)
{
    rawDataM = (int) (( valueM - minValueM) / (maxValueM - minValueM) * ((1 << bitCountM)-1) + 0.5);
    return RawDataMessage::write_to_stream(startPos, data);
}

void FloatMessage::toError()
{
    cerr<<valueM;
}

/**********************************************************************
 * INTEGERMESSAGE
 **********************************************************************/

IntegerMessage::IntegerMessage (int bitCount) 
    :RawDataMessage(bitCount)
{
    minValueM = 0;
    maxValueM = (1<<bitCountM) -1;
}

IntegerMessage::IntegerMessage (int bitCount, float min, float max) 
    :RawDataMessage(bitCount)
{
    minValueM = min;
    maxValueM = max;
}

/** returns the position of the next object in stream */
int IntegerMessage::read_from_stream(int startPos, long *data)
{
    int pos = RawDataMessage::read_from_stream(startPos, data);
    float tempValue = ((float) rawDataM / ((1 << bitCountM)-1)) * (maxValueM - minValueM) + minValueM;
    valueM = (int) (tempValue > 0? tempValue + .5 : tempValue - .5);
    return pos;
}


/** returns the position of the next object in stream. 
 */
int IntegerMessage::write_to_stream(int startPos, long *data)
{
    rawDataM = (int) (( valueM - minValueM) / (maxValueM - minValueM) * ((1 << bitCountM)-1) + 0.5);
    return RawDataMessage::write_to_stream(startPos, data);
}


void IntegerMessage::toError()
{
    cerr<<valueM;
}

/**********************************************************************
 * VECTORMESSAGE
 **********************************************************************/

int VectorMessage::read_from_stream (int startPos, long *data)
{
    int pos = xM.read_from_stream(startPos, data);
    return yM.read_from_stream(pos, data);
}

int VectorMessage::write_to_stream (int startPos, long *data)
{
    int pos = xM.write_to_stream(startPos, data);
    return yM.write_to_stream(pos, data);
}

void VectorMessage::toError()
{
    cerr<<"(";
    xM.toError();
    cerr<<", ";
    yM.toError();
    cerr<<")";
}

/**********************************************************************
 * CONFIDENCEMESSAGE
 **********************************************************************/

int ConfidenceMessage::read_from_stream(int startPos, long *data)
{
    return confM.read_from_stream(startPos, data);
}

int ConfidenceMessage::write_to_stream (int startPos, long *data)
{
    return confM.write_to_stream(startPos, data);
}

void ConfidenceMessage::toError()
{
    cerr<<"<conf:>";
    confM.toError();
    cerr<<"</conf>";
}

/**********************************************************************
 * POSITIONMESSAGE
 **********************************************************************/

int PositionMessage::read_from_stream(int startPos, long *data)
{
    int pos = confM.read_from_stream(startPos, data);
//    cerr <<"COM_BALL_POS:"<<startPos<<"-"<<pos<<"="<<(startPos-pos)<<endl;
    return posM.read_from_stream(pos, data);
}

int PositionMessage::write_to_stream (int startPos, long *data)
{
    int pos = confM.write_to_stream(startPos, data);
    return posM.write_to_stream(pos, data);
}

void PositionMessage::toError()
{
    cerr<<"<Position:>";
    posM.toError();
    confM.toError();
    cerr<<"</Position>";
}

/**********************************************************************
 * ANGLEMESSAGE
 **********************************************************************/

int AngleMessage::read_from_stream(int startPos, long *data)
{
    int pos = confM.read_from_stream(startPos, data);
    return angleM.read_from_stream(pos, data);
}

int AngleMessage::write_to_stream (int startPos, long *data)
{
    int pos = confM.write_to_stream(startPos, data);
    return angleM.write_to_stream(pos, data);
}

void AngleMessage::toError()
{
    cerr<<"<Angle:>";
    angleM.toError();
    confM.toError();
    cerr<<"</Angle>";
}

/**********************************************************************
 * VELOCITYMESSAGE
 **********************************************************************/

int VelocityMessage::read_from_stream(int startPos, long *data)
{
    int pos = confM.read_from_stream(startPos, data);
    return velM.read_from_stream(pos, data);
}

int VelocityMessage::write_to_stream (int startPos, long *data)
{
    int pos = confM.write_to_stream(startPos, data);
    return velM.write_to_stream(pos, data);
}

void VelocityMessage::toError()
{
    cerr<<"<Velocity:>";
    velM.toError();
    confM.toError();
    cerr<<"</Velocity>";
}

/**********************************************************************
 * UNUMMESSAGE
 **********************************************************************/

int UnumMessage::read_from_stream(int startPos, long *data)
{
    return unumM.read_from_stream(startPos, data);
}

int UnumMessage::write_to_stream (int startPos, long *data)
{
    return unumM.write_to_stream(startPos, data);
}

void UnumMessage::toError()
{
    cerr<<"<Unum:>";
    unumM.toError();
    cerr<<"</Unum>";
}

/**********************************************************************
 * STAMINAMESSAGE
 **********************************************************************/

int StaminaMessage::read_from_stream(int startPos, long *data)
{
    int pos = confM.read_from_stream(startPos, data);
    return staminaM.read_from_stream(pos, data);
}

int StaminaMessage::write_to_stream (int startPos, long *data)
{
    int pos = confM.write_to_stream(startPos, data);
    return staminaM.write_to_stream(pos, data);
}

void StaminaMessage::toError()
{
    cerr<<"<Stamina:>";
    staminaM.toError();
    confM.toError();
    cerr<<"</Stamina>";
}

/**********************************************************************
 * TIMESTAMPMESSAGE
 **********************************************************************/

int TimeStampMessage::read_from_stream(int startPos, long *data)
{
    int pos = timeStampSM.read_from_stream(startPos, data);
    return timeStampTM.read_from_stream(pos, data);
}

int TimeStampMessage::write_to_stream (int startPos, long *data)
{
    int pos = timeStampSM.write_to_stream(startPos, data);
    return timeStampTM.write_to_stream(pos, data);
}

void TimeStampMessage::toError()
{
    cerr<<"<TimeStamp:>(";
    timeStampTM.toError();
    cerr<<".";
    timeStampSM.toError();
    cerr<<")</TimeStamp>";
}

/**********************************************************************
 * HEADERMESSAGE
 **********************************************************************/

int HeaderMessage::read_from_stream(int startPos, long *data)
{
    int pos = magicM.read_from_stream(startPos, data);
    pos = unumM.read_from_stream(pos, data);
    return timeStampM.read_from_stream(pos, data);
}
int HeaderMessage::write_to_stream (int startPos, long *data)
{
    int pos = magicM.write_to_stream(startPos, data);
    pos = unumM.write_to_stream(pos, data);
    return timeStampM.write_to_stream(pos, data);
}

void HeaderMessage::toError()
{
    cerr<<"<Header:>";
    unumM.toError();
    timeStampM.toError();
    cerr<<"</Header>";
}

/**********************************************************************
 * OFFENSEMESSAGE
 **********************************************************************/

int OffenseMessage::read_from_stream(int startPos, long *data)
{
    int pos = confM.read_from_stream(startPos, data);
	pos = timeM.read_from_stream(pos, data);
	pos = distM.read_from_stream(pos, data);
    return offenseM.read_from_stream(pos, data);
}

int OffenseMessage::write_to_stream (int startPos, long *data)
{
    int pos = confM.write_to_stream(startPos, data);
	pos = timeM.write_to_stream(pos, data);
	pos = distM.write_to_stream(pos, data);
    return offenseM.write_to_stream(pos, data);
}

void OffenseMessage::toError()
{
    cerr<<"<Offense:>";
    offenseM.toError();
	timeM.toError();
	distM.toError();
    confM.toError();
    cerr<<"</Offense>";
}

/**********************************************************************
 * FORMATIONMESSAGE
 **********************************************************************/

int FormationMessage::read_from_stream(int startPos, long *data)
{
    int pos = timeM.read_from_stream(startPos, data);
    return formationM.read_from_stream(pos, data);
}

int FormationMessage::write_to_stream (int startPos, long *data)
{
    int pos = timeM.write_to_stream(startPos, data);
    return formationM.write_to_stream(pos, data);
}

void FormationMessage::toError()
{
    cerr<<"<Formation:>";
    formationM.toError();
    timeM.toError();
    cerr<<"</Formation>";
}

/**********************************************************************
 * PLAYERTYPEMESSAGE
 **********************************************************************/

int PlayerTypeMessage::read_from_stream(int startPos, long *data)
{
    int pos = confM.read_from_stream(startPos, data);
    return pTypeM.read_from_stream(pos, data);
}

int PlayerTypeMessage::write_to_stream (int startPos, long *data)
{
    int pos = confM.write_to_stream(startPos, data);
    return pTypeM.write_to_stream(pos, data);
}

void PlayerTypeMessage::toError()
{
    cerr<<"<PlayerType:>";
    pTypeM.toError();
    confM.toError();
    cerr<<"</PlayerType>";
}

/**********************************************************************
 * PLAYERMESSAGE
 **********************************************************************/

int PlayerMessage::read_from_stream(int startPos, long *data)
{
    int pos = posM.read_from_stream(startPos, data);
    pos = velM.read_from_stream(pos, data);
    pos = bodyAngleM.read_from_stream(pos, data);
    return neckAngleM.read_from_stream(pos, data);
}

int PlayerMessage::write_to_stream (int startPos, long *data)
{
    int pos = posM.write_to_stream(startPos, data);
    pos = velM.write_to_stream(pos, data);
    pos = bodyAngleM.write_to_stream(pos, data);
    return neckAngleM.write_to_stream(pos, data);
}

void PlayerMessage::update_message_from_memory(char side, Unum unum)
{

	if ((unum == Mem->MyNumber) && (side == Mem->MySide))
	{
		if(Mem->MyConf()) {
			setPos(Mem->MyPos());
			setPosConf(Mem->MyConf());
		}
		else { setPos(Vector(0, 0)); setPosConf(0.0); }

		if(Mem->MyVelConf()) {
			setVel(Mem->MyVel());
			setVelConf(Mem->MyVelConf());
		}
		else { setVel(Vector(0, 0)); setVelConf(0.0); }

		if(Mem->PlayerBodyAngleValid(side, unum)) {
			setBodyAngle(Mem->MyBodyAng());
			setBodyAngleConf(1.0);
		}
		else { setBodyAngle(0.0); setBodyAngleConf(0.0); }

		if(Mem->PlayerNeckAngleValid(side, unum)) {
			setNeckAngle(Mem->MyNeckGlobalAng());
			setNeckAngleConf(1.0);
		}
		else { setNeckAngle(0.0); setNeckAngleConf(0.0); }

		return;

	} else 
	{
		if (Mem->GetPlayer(side, unum) == NULL) 
		{
			setPos(Vector(0, 0)); setPosConf(0.0); 
			setVel(Vector(0, 0)); setVelConf(0.0); 
			setBodyAngle(0.0); setBodyAngleConf(0.0);
			setNeckAngle(0.0); setNeckAngleConf(0.0); 
			return;
		}
		if(Mem->PlayerPositionValid(side, unum)) {
			setPos(Mem->PlayerAbsolutePosition(side, unum));
			setPosConf(Mem->PlayerPositionValid(side, unum));
		}
		else { setPos(Vector(0, 0)); setPosConf(0.0); }

		if(Mem->PlayerVelocityValid(side, unum)) {
			setVel(Mem->PlayerAbsoluteVelocity(side, unum));
			setVelConf(Mem->PlayerVelocityValid(side, unum));
		}
		else { setVel(Vector(0, 0)); setVelConf(0.0); }

		if(Mem->PlayerBodyAngleValid(side, unum)) {
			setBodyAngle(Mem->PlayerAbsoluteBodyAngle(side, unum));
			setBodyAngleConf(Mem->PlayerBodyAngleValid(side, unum));
		}
		else { setBodyAngle(0.0); setBodyAngleConf(0.0); }

		if(Mem->PlayerNeckAngleValid(side, unum)) {
			setNeckAngle(Mem->PlayerAbsoluteNeckAngle(side, unum));
			setNeckAngleConf(Mem->PlayerNeckAngleValid(side, unum));
		}
		else { setNeckAngle(0.0); setNeckAngleConf(0.0); }
	}
}
	
void PlayerMessage::update_memory_from_message(MateMessage sender, Time time, char side, Unum unum)
{
	float dist = (sender.getPos() - getPos()).mod();
    if ((unum != Mem->MyNumber) || (side != Mem->MySide))
    {
		if ((getPosConf() > MIN_CONF)
			&& ((Mem->GetPlayer(side, unum) == NULL)
				|| (!Mem->PlayerPositionValid(side, unum))))
		{
			MAKELOG((1, DBG_COMM, " %d hears player %d at time %g", Mem->MyNumber, unum, time.s));
			// cerr<<Mem->MyNumber<<" hears player "<<unum<<" at time "<<time.s<<endl;
			Mem->HearPlayer(side, unum, getPos().x, getPos().y, getPosConf(), getVel().x, getVel().y, getVelConf(), dist, time);
			// Mem->HearPlayer(side, unum, getPos().x, getPos().y, getPosConf(), getVel().x, getVel().y, getVelConf(), 
							// getBodyAngle(), getBodyAngleConf(), getNeckAngle(), getNeckAngleConf(), dist, time);
			
			if(side == Mem->MySide) matesUpdated++;
			else opponentsUpdated++;
			
		}
    }
}

void PlayerMessage::toError()
{
    posM.toError();
    velM.toError();
    cerr<<"<Body:>";
    bodyAngleM.toError();
    cerr<<"</Body:>";
    cerr<<"<Neck:>";
    neckAngleM.toError();
    cerr<<"</Neck:>";
}

/**********************************************************************
 * MATEMESSAGE
 **********************************************************************/

int MateMessage::read_from_stream(int startPos, long *data)
{
    int pos = playerM.read_from_stream(startPos, data);
    return staminaM.read_from_stream(pos, data);
}

int MateMessage::write_to_stream (int startPos, long *data)
{
    int pos = playerM.write_to_stream(startPos, data);
    return staminaM.write_to_stream(pos, data);
}

void MateMessage::update_message_from_memory(Unum unum)
{
	playerM.update_message_from_memory(Mem->MySide, unum);

	if (unum == Mem->MyNumber)
	{
		setStamina(Mem->MyStamina());
		setStaminaConf(1.0);
	}
	else
	{
		setStamina(Mem->GetStamina(Mem->MySide, unum));
		setStaminaConf(Mem->GetStaminaConf(Mem->MySide, unum));
	}
}

void MateMessage::update_memory_from_message(MateMessage sender, Time time, Unum unum)
{
	if (unum == Mem->MyNumber) return;
/*
  if (getStaminaConf > Mem->GetStaminaConf(Mem->MySide, unum))
  {
  float dist = (sender.getPos() - getPos()).mod();
  
  Mem->SetStamina(getStamina(), getStaminaConf(), dist, time, Mem->MySide, unum); 
  }
*/
    playerM.update_memory_from_message(sender, time, Mem->MySide, unum);
}

void MateMessage::toError()
{
    cerr<<"<Mate:>";
    playerM.toError();
    staminaM.toError();
    cerr<<"</Mate>";
}

/**********************************************************************
 * OPPONENTMESSAGE
 **********************************************************************/

int OpponentMessage::read_from_stream(int startPos, long *data)
{
    int pos = playerM.read_from_stream(startPos, data);
    return pTypeM.read_from_stream(pos, data);
}

int OpponentMessage::write_to_stream (int startPos, long *data)
{
    int pos = playerM.write_to_stream(startPos, data);
    return pTypeM.write_to_stream(pos, data);
}

void OpponentMessage::update_message_from_memory(Unum unum)
{
    playerM.update_message_from_memory(Mem->TheirSide, unum);
    setPlayerType(Mem->GetPlayerType(Mem->TheirSide, unum));
    setPlayerTypeConf(Mem->GetPlayerTypeConf(Mem->TheirSide, unum));
}

void OpponentMessage::update_memory_from_message(MateMessage sender, Time time, Unum unum)
{
    //Mem->SetPlayerType(getPlayerType(), getPlayerTypeConf(), Mem->TheirSide, unum); 
    playerM.update_memory_from_message(sender, time, Mem->TheirSide, unum);
}

void OpponentMessage::toError()
{
    cerr<<"<Opponent:>";
    playerM.toError();
    pTypeM.toError();
    cerr<<"</Opponent>";
}

/**********************************************************************
 * BALLMESSAGE
 **********************************************************************/

int BallMessage::read_from_stream(int startPos, long *data)
{
    int pos = posM.read_from_stream(startPos, data);
    pos = velM.read_from_stream(pos, data);
    return pos;
}

int BallMessage::write_to_stream (int startPos, long *data)
{
    int pos = posM.write_to_stream(startPos, data);
    pos = velM.write_to_stream(pos, data);
    return pos;
}

void BallMessage::update_message_from_memory()
{
    if(Mem->BallPositionValid())
	{
		posM.setPos(Mem->BallAbsolutePosition());
		posM.setConf(Mem->BallPositionValid());
	}
	else
	{
		posM.setPos(Vector(0, 0));
		posM.setConf(0.0);
	}
    if(Mem->BallVelocityValid())
	{
		velM.setVel(Mem->BallAbsoluteVelocity());
		velM.setConf(Mem->BallVelocityValid());
	}
	else
	{
		velM.setVel(Vector(0, 0));
		velM.setConf(0.0);
	}
}

void BallMessage::update_memory_from_message(MateMessage sender, Time time)
{
    float dist = (sender.getPos() - getPos()).mod();
    if ((posM.getConf() > MIN_CONF) && (!Mem->BallPositionValid()))
    {
		//cerr<<Mem->MyNumber<<" hears ball "<<" at time "<<time.s<<endl;
		Mem->HearBall(posM.getPos().x, posM.getPos().y, posM.getConf(),
					  velM.getVel().x, velM.getVel().y, velM.getConf(),
					  dist, time);
    }
}

void BallMessage::toError()
{
    cerr<<"<Ball:>";
    posM.toError();
    velM.toError();
    cerr<<"</Ball>";
}


/**********************************************************************
 * PASSMESSAGE
 **********************************************************************/

int PassMessage::read_from_stream(int startPos, long *data)
{
    int pos = passpartnerM.read_from_stream(startPos, data);
    pos = positionM.read_from_stream(pos, data);
    pos = timeM.read_from_stream(pos, data);
    return pos;
}

int PassMessage::write_to_stream (int startPos, long *data)
{
    int pos = passpartnerM.write_to_stream(startPos, data);
    pos = positionM.write_to_stream(pos, data);
    pos = timeM.write_to_stream(pos, data);
    return pos;
}

void PassMessage::update_message_from_memory()
{
    if(Mem->PlayedPass())
	{
		//cerr << "PASS SAID: " << Mem->MyNumber << " to " << Mem->Passpartner << endl;
		setPosition(Mem->PassTargetVector);
		setTime(Mem->LastPassTime);
		setPasspartner(Mem->Passpartner);
	}
	else
	{
		setPosition(Vector(0,0));
		setTime(Time());
		setPasspartner(0);
	}
}

void PassMessage::update_memory_from_message(Unum sender, Time time)
{
    if (getPasspartner() != 0)
    {
		Mem->HearPass(getPasspartner(), getPosition(), getTime());
    }
}

void PassMessage::toError()
{
    cerr<<"<Pass:>";
	passpartnerM.toError();
    positionM.toError();
    timeM.toError();
    cerr<<"</Pass>";
}
/**********************************************************************
 * STRATEGYMESSAGE
 **********************************************************************/

int StrategyMessage::read_from_stream(int startPos, long *data)
{
    int pos = formationM.read_from_stream(startPos, data);
	pos = passM.read_from_stream(pos, data);
    return offenseM.read_from_stream(pos, data);
}

int StrategyMessage::write_to_stream (int startPos, long *data)
{
    int pos = formationM.write_to_stream(startPos, data);
    pos = passM.write_to_stream(pos, data);
    return offenseM.write_to_stream(pos, data);
}


void StrategyMessage::update_message_from_memory()
{
    formationM.setTimeStamp(Mem->GetCurrentFormationTime());
    formationM.setFormation(Mem->GetCurrentFormationID());
    offenseM.setConf(Mem->GetOffenseConf());
	offenseM.setTime(Mem->GetOffenseTime());
	offenseM.setDist(Mem->GetOffenseDist());
	offenseM.setOffense(Mem->GetOffense());
	passM.update_message_from_memory();
}

void StrategyMessage::update_memory_from_message(Unum sender, Time time)
{
    Mem->HearFormation(formationM.getFormation(), formationM.getTimeStamp(), sender, time);
    Mem->SetOffense(offenseM.getOffense(), offenseM.getTime(), offenseM.getDist(), sender, time);
	// cerr << "UPDATING_PASS (" << Mem->MyNumber << ") from " << sender << endl;
	passM.update_memory_from_message(sender, time);
}

void StrategyMessage::toError()
{
    cerr<<"<Strategy:>";
    formationM.toError();
    offenseM.toError();
	passM.toError();
    cerr<<"</Strategy>";

}

/**********************************************************************
 * OURTEAMMESSAGE
 **********************************************************************/

int OurTeamMessage::read_from_stream (int startPos, long *data)
{
    int pos = startPos;
    for (int i=0; i<11; i++)
    {
		pos = matesM[i].read_from_stream(pos, data);
    }
    return pos;
}

int OurTeamMessage::write_to_stream (int startPos, long *data)
{
    int pos = startPos;
    for (int i=0; i<11; i++)
    {
		pos = matesM[i].write_to_stream(pos, data);
    }
    return pos;
}

void OurTeamMessage::update_message_from_memory()
{
    for (int i=0; i<11; i++)
    {
		matesM[i].update_message_from_memory(i+1);
	}
}

void OurTeamMessage::update_memory_from_message(MateMessage sender, Time time)
{
	matesUpdated = 0;
	for (int i=0; i<11; i++)
    {
		if(matesUpdated >= Mem->OP_communicate_max_mates) break;

		matesM[i].update_memory_from_message(sender, time, i+1);

	}

}

void OurTeamMessage::toError()
{
    cerr<<"<OurTeam:>";
    for (int i=0; i<11; i++)
    {
		cerr<<"<"<<i<<":>";
		matesM[i].toError();
		cerr<<"</"<<i<<">";
    }
    cerr<<"</OurTeam>";
}

/**********************************************************************
 * THEIRTEAMMESSAGE
 **********************************************************************/

int TheirTeamMessage::read_from_stream (int startPos, long *data)
{
    int pos = startPos;
    for (int i=0; i<11; i++)
    {
		pos = opponentsM[i].read_from_stream(pos, data);
    }
    return pos;
}

int TheirTeamMessage::write_to_stream (int startPos, long *data)
{
    int pos = startPos;
    for (int i=0; i<11; i++)
    {
		pos = opponentsM[i].write_to_stream(pos, data);
    }
    return pos;
}


void TheirTeamMessage::update_message_from_memory()
{
    for (int i=0; i<11; i++)
    {
		opponentsM[i].update_message_from_memory(i+1);
    }
}

void TheirTeamMessage::update_memory_from_message(MateMessage sender, Time time)
{
	opponentsUpdated = 0;
    for (int i=0; i<11; i++)
    {
		if(opponentsUpdated >= Mem->OP_communicate_max_opponents) break;
		opponentsM[i].update_memory_from_message(sender, time, i+1);
    }
}

void TheirTeamMessage::toError()
{
    cerr<<"<TheirTeam:>";
    for (int i=0; i<11; i++)
    {
		cerr<<"<"<<i<<":>";
		opponentsM[i].toError();
		cerr<<"</"<<i<<">";
    }
    cerr<<"</TheirTeam>";
}


/**********************************************************************
 * THEMESSAGE
 **********************************************************************/

int TheMessage::read_from_stream(int startPos, long *data)
{
    int pos = headerM.read_from_stream(startPos, data);
    
    if (headerM.getMagic() != MAGIC)
	return 0;

	if(Mem->OP_communicate_mates) { pos = ourTeamM.read_from_stream(pos, data); }
	if(Mem->OP_communicate_opponents) { pos = theirTeamM.read_from_stream(pos, data); }
	if(Mem->OP_communicate_strategy) { pos = strategyM.read_from_stream(pos, data); }
	if(Mem->OP_communicate_ball) { pos = ballM.read_from_stream(pos, data); }
    return pos;
}

int TheMessage::write_to_stream (int startPos, long *data)
{
    int pos = headerM.write_to_stream(startPos, data);
	if(Mem->OP_communicate_mates) { pos = ourTeamM.write_to_stream(pos, data); }
	if(Mem->OP_communicate_opponents) { pos = theirTeamM.write_to_stream(pos, data); }
	if(Mem->OP_communicate_strategy) { pos = strategyM.write_to_stream(pos, data); }
	if(Mem->OP_communicate_ball) { pos = ballM.write_to_stream(pos, data); }
    return pos;
}

void TheMessage::update_message_from_memory()
{
    
    headerM.setTimeStamp(Mem->CurrentTime);
    headerM.setUnum(Mem->MyNumber);
    headerM.setMagic(MAGIC);
    
	if(Mem->OP_communicate_mates) { ourTeamM.update_message_from_memory(); }
	if(Mem->OP_communicate_opponents) { theirTeamM.update_message_from_memory(); }
	if(Mem->OP_communicate_strategy) { strategyM.update_message_from_memory(); }
	if(Mem->OP_communicate_ball) { ballM.update_message_from_memory(); }
}

void TheMessage::update_memory_from_message()
{
    if (headerM.getMagic() != MAGIC)
    {
		return ;
    }
    Time time = headerM.getTimeStamp();
    MateMessage sender = ourTeamM.getMate(headerM.getUnum());
    Unum unum = headerM.getUnum();
	if(unum == Mem->MyNumber) return;
    
	if(Mem->OP_communicate_mates) { ourTeamM.update_memory_from_message(sender, time); }
	if(Mem->OP_communicate_opponents) { theirTeamM.update_memory_from_message(sender, time); }
	if(Mem->OP_communicate_strategy) { strategyM.update_memory_from_message(unum, time); }
	if(Mem->OP_communicate_ball) { ballM.update_memory_from_message(sender, time); }
}

void TheMessage::toError()
{
    cerr<<"<TheMessage:>";
    headerM.toError();
	if(Mem->OP_communicate_mates) { ourTeamM.toError(); }
	if(Mem->OP_communicate_opponents) { theirTeamM.toError(); }
	if(Mem->OP_communicate_strategy) { strategyM.toError(); }
	if(Mem->OP_communicate_ball) { ballM.toError(); }
    cerr<<"</TheMessage>";
}



