#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <sstream>
#include <stdint.h>
#include <iterator>
#include <cstdio>

#pragma once

using std::map;
using std::string;
using std::vector;
using std::stringstream;
using std::make_pair;

const double PI = 4 * atan2(1.0, 1.0);

typedef uint64_t MicrobeID_t;

class Microbe;
class Object;
class Victim;
class Predator;
class Block;
class SoloWatchingVictim;
class GroupWatchingVictim;
class OptimicticWatchingPredator;
class RealisticWatchingPredator;

enum Directions
{
	dLeft   = 0,
	dRight  = 1,
	dTop    = 2,
	dBottom = 3
};

class Point 
{
public:
	Point(size_t posX, size_t posY) :
		x(posX),
		y(posY) 
	{	}

    Point(const Point other) :
        x(other.x),
        y(other.y)
    {   }

	Point() :
		x(0),
		y(0)
	{	}

	size_t x;
	size_t y;

	void MoveRandom(bool CanStay);

	bool operator==(const Point & other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const Point & other) const
	{
		return !operator==(other);
	}

	bool operator<(const Point & other) const
	{
		return x < other.x || (x == other.x && y < other.y);
	}

	void Move(Directions direction);
};

class Message
{
public:
    enum MessageType
    {
        mtCooperate
    };

    Message(const Point & basis, const MessageType type, const double density) :
        m_Basis(basis),
        m_Type(type),
        m_Density(density)
    { }

    const Point GetBasis() const
    {
        return m_Basis;
    }

    const MessageType GetType() const
    {
        return m_Type;
    }

    const double GetDensity() const
    {
        return m_Density;
    }

private:
    MessageType m_Type;
    Point m_Basis;
    double m_Density;
};

class Answer
{
public:
    enum AnswerType
    {
        atNotImplemented,
        atAccept,
        atReject
    };

    Answer(const AnswerType type) :
        m_Type(type)
    { }

    static Answer ReturnAnswerNotImplemented() 
    {
        return Answer(atNotImplemented);
    }

private:
    AnswerType m_Type;
};

class ObjectInfo
{
public:
	enum ObjectType
	{
		otBlock,
		otVictim,
		otPredator
	};

	MicrobeID_t m_Id;
	ObjectType m_Type;
};

struct GameInfo
{
	map<Point, ObjectInfo> info;
};

class DirectionsInfo
{
public:

	explicit DirectionsInfo(const Point basePoint,const GameInfo * gameInfo)
	{
		for(size_t i = 0; i < 4; ++i)
		{
			m_PredatorCount[i]  = 0;
			m_VictimCount[i]    = 0;
		}

		map<Point, ObjectInfo>::const_iterator it;
		for(it = gameInfo->info.begin(); it != gameInfo->info.end(); ++it)
		{
			double deltaX = static_cast<double>(it->first.x) - static_cast<double>(basePoint.x);
			double deltaY = static_cast<double>(it->first.y) - static_cast<double>(basePoint.y);

			double angle = atan2(deltaY, deltaX);
			double distance = abs(deltaX) + abs(deltaY);

			size_t direction;

			if (3.0 / 4 * PI >= angle && angle >= 1 / 4.0 * PI)
				direction = dTop;
			else
				if (1 / 4.0 * PI >= angle && angle >= -1 / 4.0 * PI)
					direction = dRight;
				else
					if (-1 / 4.0 * PI >= angle && angle >= -3 / 4.0 * PI)
						direction = dBottom;
					else
						direction = dLeft;

			if (it->second.m_Type == ObjectInfo::otPredator)
				m_PredatorCount[direction] += 1 / distance;
			
			if (it->second.m_Type == ObjectInfo::otVictim)
				m_VictimCount[direction] += 1 / distance;
		}
	}

	// return pair of (VictimsCount, PredatorsCount)
	std::pair<double, double> GetInfoByDirection(const size_t direction)
	{
		return make_pair(m_VictimCount[direction], m_PredatorCount[direction]);
	}

private:
	double m_VictimCount[4];
	double m_PredatorCount[4];
};

/* Game description format:
 * First line:
 *	rows colums
 * Objects types: 
 *	Block = 0
 *	Predator = 1
 *	Victim = 2
 *  SoloWatchingVictim = 3
 *  GroupWatchingVictim = 4
 *  OptimisticWatchingPredator = 5
 *  RealisticWatchingPredator = 6
 * Block: 
 *  Type=0 posX posY 
 * Predator:
 *	Type=1 posX posY initialStamina
 *	initialStamina - initial stamina stock
 * Victim:
 *  Type=2 posX posY reproduceSpeed worth
 *  reproduceSpeed - number of steps when microbe reproduce
 *  worth - number of healthPoints that preedator gain when eat me
 * SoloWatchingVictim:
 *  Type=3 posX posY reproduceSpeed worth visionRadius
 * GroupWatchingVictim:
 *  Type=4 posX posY reproduceSpeed worth visionRadius
 * OptimisticWatchingPredator:
 *  Type=5 posX posY initialStamina visionRadius
 * RealisticWatchingPredator:
 *  Type=6 posX posY initialStamina visionRadius
 */


class ObjectFactory
{
public:
	ObjectFactory() :
		m_MicrobeIdCounter(0)
	{	}

	Object * CreateObject(const string decription);
	Microbe * GetClone(Microbe * parent);
	Block * CreateBlock(size_t x, size_t y);
private:
	MicrobeID_t m_MicrobeIdCounter;
		
	Block * CreateBlock(const vector<string> & params);
	Predator * CreatePredator(const vector<string> & params);
	Victim * CreateVictim(const vector<string> & params);
	SoloWatchingVictim * CreateSoloVictim(const vector<string> & params);
	GroupWatchingVictim * CreateGroupVictim(const vector<string> & params);
	OptimicticWatchingPredator * CreateOptimicticPredator(const vector<string> & params);
	RealisticWatchingPredator * CreateRealisticPredator(const vector<string> & params);
	MicrobeID_t GetNextId();

};

class Manager
{
public:
	Manager(string fileName);
	void NextStep(void);
	void StartGame(const size_t);
	void PrintState() const;
	size_t GetStepNumber() const
	{
		return m_StepNumber;
	}
	
	// microb actions
	GameInfo * GetGameInfo(const MicrobeID_t);
	void SendMessage(const MicrobeID_t, const MicrobeID_t, const Message &); 
	~Manager();
	
	stringstream m_Log;
private:
	bool EatAndMove(Microbe *, Object *, Point, vector<vector<Object *> > &);  

	bool dfs(Point, vector<vector<bool> > &, vector<vector<Object *> > &);

	void UpdateMicrobeMap();
	void UpdateMicrobePos();
	void UpdateBlocks();
	void CheckHungry();

	void CreateBorders();
	void CountMicrobes();

	ObjectFactory m_Factory;
	vector<vector<Object *> > m_Field;
	map<MicrobeID_t, Microbe *> m_Microbes;
	vector<Object *> m_Blocks;
	MicrobeID_t m_Counter;
	size_t m_StepNumber;
	size_t m_Rows, m_Columns;
};

class Object
{
public:
	Object(size_t x, size_t y) :
		m_Position(x, y) 
	{	}

	const Point& GetPosition()
	{
		return m_Position;
	}

	virtual bool WantToMove() const = 0;
	virtual char ToChar() const = 0;

	virtual ~Object()
	{	}
protected:
	Point m_Position;
};

class Block : public Object
{
public:
	Block(size_t x, size_t y) :
		Object(x, y) 
	{	}

	virtual	bool WantToMove() const
	{
		return false;
	}

	virtual char ToChar() const
	{
		return '*';
	}

	virtual ~Block()
	{	}

};

enum DecisionType
{
	NONE,
	MOVE,
	STAY,
	REPLICATE
};

struct Decision
{
	int m_DecisionType;
	Point m_newPosition;
    double m_DecisionCoefficent;
    bool m_IsIndividual;
};

class Microbe : public Object
{
public:
	Microbe(size_t x, size_t y, MicrobeID_t id) : 
		Object(x, y),
		m_Id(id)
	{
	}

	void Move(Point newPosition)
	{
		m_Position = newPosition;
		OnMove();
	}

	virtual void OnMove()
	{	}


	virtual bool WantToMove() const
	{
		return m_Decision.m_DecisionType == MOVE;
	}

	const Decision& GetDecision() const
	{
		return m_Decision;
	}

	const MicrobeID_t GetId() const
	{
		return m_Id;
	}

	void SetId(MicrobeID_t id)
	{
		m_Id = id;
	}

	void SetManager(Manager *manager)
	{
		m_Manager = manager;
	}

	Manager * GetManager() const
	{
		return m_Manager;
	}

	void ClearDecision()
	{
		m_Decision.m_DecisionType = NONE;
	}

	virtual Microbe * Clone() const = 0;
	virtual bool IsHungry() const
	{
		return false;
	}

	virtual void MakeDecision(void) = 0;

	virtual ~Microbe(void)
	{	}

protected:
	MicrobeID_t m_Id;
	Decision m_Decision;

	void MakeRandomDecision();

private:
	Manager * m_Manager;
};

class Predator : public Microbe
{
public:
	Predator(size_t x, size_t y, MicrobeID_t id, unsigned initialStamina) :
		Microbe(x, y, id),
		m_StaminaStock(initialStamina)
	{	}

	void GainFood(unsigned healthPoints)
	{
		m_StaminaStock += healthPoints;
	}

	virtual char ToChar() const
	{
		return '#';
	}

	virtual void OnMove() 
	{
		--m_StaminaStock;
	}

	virtual bool IsHungry() const
	{
		return m_StaminaStock == 0;
	}

	virtual Microbe * Clone() const;
	virtual void MakeDecision();
	virtual ~Predator()
	{	}

private:
	unsigned m_StaminaStock;
};

class Victim : public Microbe
{
public:
	Victim(size_t x, size_t y, MicrobeID_t id, unsigned reproduceSpeed, unsigned worth) :
		Microbe(x, y, id),
		m_ReproduceSpeed(reproduceSpeed),
		m_Worth(worth)
	{	}

	unsigned GetWorth()
	{
		return m_Worth;
	}

	bool NeedReproduce()
	{
		if (GetManager()->GetStepNumber() % m_ReproduceSpeed == 0)
		{
			m_Decision.m_newPosition = m_Position;
			m_Decision.m_newPosition.MoveRandom(false);
			m_Decision.m_DecisionType = REPLICATE;
			return true;
		}
		return false;
	}

	virtual char ToChar() const
	{
		return '$';
	}

	virtual void MakeDecision();
	virtual Microbe * Clone() const;
	virtual ~Victim()
	{	}

private:
	unsigned m_ReproduceSpeed;
	unsigned m_Worth;
};

class Watcher
{
public:
    Watcher(unsigned visionRadius) :
        m_VisionRadius(visionRadius)
    {  }


	unsigned GetVisionRadius() const
	{
		return m_VisionRadius;
	}

private:
	unsigned m_VisionRadius;
};

class DecidingWatcher : public Watcher
{
public:
	DecidingWatcher(unsigned visionRadius) :
		Watcher(visionRadius)
	{	}


	Decision GetWatchDecision(Point position, GameInfo * info);
	virtual double FigureOutCoef(double, double) const = 0;
};

class GroupWatchingVictim : public Victim,
							public DecidingWatcher
{
public:
	GroupWatchingVictim(size_t x, size_t y, MicrobeID_t id, unsigned reproduceSpeed, unsigned worth, unsigned visionRadius) :
		Victim(x, y, id, reproduceSpeed, worth),
		DecidingWatcher(visionRadius)
	{	}

	void MakeDecision();
	Microbe * Clone() const;
private:
	virtual double FigureOutCoef(double, double) const;
};

class SoloWatchingVictim :  public Victim,
							public DecidingWatcher
{
public:
	SoloWatchingVictim(size_t x, size_t y, MicrobeID_t id, unsigned reproduceSpeed, unsigned worth, unsigned visionRadius) :
		Victim(x, y, id, reproduceSpeed, worth),
		DecidingWatcher(visionRadius)
	{	}

	void MakeDecision();
	Microbe * Clone() const;
private:
	virtual double FigureOutCoef(double, double) const;
};

class OptimicticWatchingPredator :	public Predator,
									public DecidingWatcher
{
public:
	OptimicticWatchingPredator(size_t x, size_t y, MicrobeID_t id, unsigned initialStamina, unsigned visionRadius) :
		Predator(x, y, id, initialStamina),
		DecidingWatcher(visionRadius)
	{	}

	void MakeDecision();
private:
	double FigureOutCoef(double, double) const;
};

class RealisticWatchingPredator :	public Predator,
									public DecidingWatcher
{
public:
	RealisticWatchingPredator(size_t x, size_t y, MicrobeID_t id, unsigned initialStamina, unsigned visionRadius) :
		Predator(x, y, id, initialStamina),
		DecidingWatcher(visionRadius)
	{	}

	void MakeDecision();
private:
	double FigureOutCoef(double, double) const;
};


class Messenger
{
public:
    Manager * GetManager() const
    {
        return m_Manager;
    }

    void SetManager(Manager * manager)
    {
        m_Manager = manager;
    }

    virtual Answer ProcessMessage(const Message & message) = 0;
    virtual void MakeCooperativeDesicion() = 0;
private:
    Manager * m_Manager;
};

class CooperateMessenger : public Messenger
{
public:
    Answer ProcessMessage(const Message & message);
    void Cooperate(const GameInfo * gameInfo, const Decision & plan, const Point & curPos) const;
protected:
    virtual bool CompareAndSetDecisions(const Decision & cooperativeDecision) = 0;
    virtual const Point PointTowardBasis(const Point basis) const = 0;
};

class CooperatePredator :   public RealisticWatchingPredator,
                            public CooperateMessenger
{
public:
    CooperatePredator(size_t x, size_t y, unsigned initialStamina, unsigned visionRadius, unsigned characterType) :
        RealisticWatchingPredator(x, y, initialStamina, visionRadius),
        m_CharacterType(characterType)
    {  }

    void SetManager(Manager * manager)
    {
        Predator::SetManager(manager);
        CooperateMessenger::SetManager(manager);
    }

    void MakeDecision();
protected:
    bool CompareAndSetDecisions(const Decision & cooperativeDecision);
    const Point PointTowardBasis(const Point basis) const;

private:
    GameInfo * MakeIndividualDecision();
    unsigned m_CharacterType;
};

class CooperateVictim : public GroupWatchingVictim,
                        public CooperateMessenger
{
public:
    CooperateVictim(size_t x, size_t y, unsigned reproductionSpeed, unsigned worth, unsigned visionRadius, unsigned characterType) :
        GroupWatchingVictim(x, y, reproduceSpeed, worth, visionRadius),
        m_CharacterType(characterType)
    { }
    
    void SetManager(Manager * manager)
    {
        Victim::SetManager(manager);
        CooperateMessenger::SetManager(manager);
    }

    void MakeDecision();
protected:
    bool CompareAndSetDecisions(const Decision & cooperativeDecision);
    const Point PointTowardBasis(const Point basis) const;
private:
    GameInfo * MakeIndividualDecision();
    unsigned m_CharacterType;
};

class Utilities
{
public:
	static void SplitParams(const string & desc, vector<string> & params);
	static size_t ToNumber(const string & number);
    static Direction ToDirection(const Point & from, const Point & to);
};
