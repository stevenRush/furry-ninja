#include "Game.h"
#include <limits>

using namespace std;

Manager::Manager(string fileName)
{
	m_StepNumber = 1;

	if (fileName != "")
		freopen(fileName.c_str(), "r", stdin);
	string params;

	getline(cin, params);
	while(params[0] == '#')
		getline(cin, params);
	vector<string> fieldParams;
	Utilities::SplitParams(params, fieldParams);
	m_Rows = Utilities::ToNumber(fieldParams[0]) + 2;
	m_Columns = Utilities::ToNumber(fieldParams[1]) + 2;

	vector<Object *> temp(m_Rows, NULL);
	m_Field.assign(m_Columns, temp);

	while(getline(cin, params))
	{
		Object * object = m_Factory.CreateObject(params);
		Point pos = object->GetPosition();
		m_Field[pos.x][pos.y] = object;
		if (Microbe * microbe = dynamic_cast<Microbe *>(object))
		{
			m_Microbes.insert(make_pair(microbe->GetId(), microbe));
			microbe->SetManager(this);
		}
		else
			m_Blocks.push_back(object);
	}

	CreateBorders();

	srand(time(NULL));
}

void Manager::CreateBorders()
{
	size_t rows[] = {0, m_Rows-1};
	for(size_t i = 0; i < 2; ++i)
	{
		for(size_t index = 0; index < m_Columns; ++index)
		{
			size_t posX = index;
			size_t posY = rows[i];
			Block * block = m_Factory.CreateBlock(posX, posY);
			m_Field[posX][posY] = block;
			m_Blocks.push_back(block);
		}
	}

	size_t columns[] = {0, m_Columns-1};
	for(size_t i = 0; i < 2; ++i)
	{
		for(size_t index = 1; index < m_Rows-1; ++index)
		{
			size_t posX = columns[i];
			size_t posY = index;
			Block * block = m_Factory.CreateBlock(posX, posY);
			m_Field[posX][posY] = block;
			m_Blocks.push_back(block);
		}
	}
}

bool Manager::dfs(Point cur_point, vector<vector<bool> > & wasThere, vector<vector<Object *> > & newField)
{
	wasThere[cur_point.x][cur_point.y] = true;

	if (m_Field[cur_point.x][cur_point.y] == NULL)
		return true;

	if (m_Field[cur_point.x][cur_point.y]->WantToMove() == false)
	{
		newField[cur_point.x][cur_point.y] = m_Field[cur_point.x][cur_point.y];
		return false;
	}

	Microbe * curMicrobe = dynamic_cast<Microbe *>(m_Field[cur_point.x][cur_point.y]);
	Point point = curMicrobe->GetDecision().m_newPosition;

	if (newField[point.x][point.y] != NULL)
	{
		bool result = EatAndMove(curMicrobe, newField[point.x][point.y], point, newField);
		if (result == false)
		{
			newField[cur_point.x][cur_point.y] = static_cast<Object *>(curMicrobe);
		}
		return result;
	}


	newField[point.x][point.y] = static_cast<Object *>(curMicrobe);

	if (wasThere[point.x][point.y] == true)
		return true;

	if (dfs(point, wasThere, newField))
	{
		return true;
	} 
	else 
	{
		bool result = EatAndMove(curMicrobe, newField[point.x][point.y], point, newField);
		if (result == false)
		{
			newField[cur_point.x][cur_point.y] = static_cast<Object *>(curMicrobe);
		}
		return result;
	}
}

bool Manager::EatAndMove(Microbe * eater, Object * iter, Point pos, vector<vector<Object *> > & field)
{
	Predator * predator = dynamic_cast<Predator *>(eater);
	Victim * victim = dynamic_cast<Victim *>(iter);

	if (predator != NULL && victim != NULL)
	{		
		predator->GainFood(victim->GetWorth());
		field[pos.x][pos.y] = static_cast<Object *>(predator);
		delete victim;
		//m_Log << "Victim was eaten at (" << pos.x << "," << pos.y << ")\n"; 
		return true;
	}

	predator = dynamic_cast<Predator *>(iter);
	victim = dynamic_cast<Victim *>(eater);

	if (predator != NULL && victim != NULL)
	{
		predator->GainFood(victim->GetWorth());
		field[pos.x][pos.y] = static_cast<Object *>(predator);
		delete victim;
		//m_Log << "Victim eaten at (" << pos.x << "," << pos.y << ")\n"; 
		return true;
	}

	return false;
}

void Manager::NextStep()
{
	m_Log.str(string());
	for(map<MicrobeID_t, Microbe *>::const_iterator it = m_Microbes.begin(); it != m_Microbes.end(); ++it)
	{
		Microbe * micr = it->second;
		micr->MakeDecision();
	}

	vector<Object *> temp(m_Rows, NULL);
	vector<vector<Object *> > newField(m_Columns, temp);
	
	vector<bool> temp_vector(m_Rows, false);
	vector<vector<bool> > used(m_Columns, temp_vector);

	vector<Microbe *> microbes;
	for (map<MicrobeID_t, Microbe *>::const_iterator iter = m_Microbes.begin(); iter != m_Microbes.end(); ++iter)
	{
		microbes.push_back(iter->second);
	}

	std::random_shuffle(microbes.begin(), microbes.end());

	for (size_t index = 0; index < microbes.size(); ++index) 
	{
		Point pos = microbes[index]->GetPosition();
		if (pos.x > m_Columns || pos.y > m_Rows)
			continue;
		if (used[pos.x][pos.y] == false)
		{
			dfs(pos, used, newField);
		}
	}

	m_Field = newField;

	UpdateBlocks();

	// microbe reproduction part
	for (size_t posX = 0; posX < m_Columns; ++posX)
	{
		for (size_t posY = 0; posY < m_Rows; ++posY)
		{
			Microbe * curMicrobe = dynamic_cast<Microbe *>(m_Field[posX][posY]);
			if (curMicrobe != NULL)
			{
				Decision decision = curMicrobe->GetDecision();
				if (decision.m_DecisionType == REPLICATE) 
				{
					Microbe * children = m_Factory.GetClone(curMicrobe);
					if (m_Field[decision.m_newPosition.x][decision.m_newPosition.y] == NULL)
					{
						m_Field[decision.m_newPosition.x][decision.m_newPosition.y] = children;
						children->ClearDecision();
						continue;
					}
					m_Field[posX][posY] = children;
					
					if (! EatAndMove(curMicrobe, m_Field[decision.m_newPosition.x][decision.m_newPosition.y], decision.m_newPosition, m_Field))
					{
						m_Field[posX][posY] = curMicrobe;
						delete children;
					}
				}
				curMicrobe->ClearDecision();
			}
		}
	}
	
	UpdateMicrobePos();
	UpdateMicrobeMap();
	CheckHungry();
	CountMicrobes();
	++m_StepNumber;
}

void Manager::CheckHungry()
{
	map<MicrobeID_t, Microbe *>::iterator it = m_Microbes.begin();
	while(it != m_Microbes.end())
	{
		Microbe *micr = it->second;
		if (micr->IsHungry())
		{
			Point pos = micr->GetPosition();
			m_Field[pos.x][pos.y] = NULL;
			m_Microbes.erase(it++);
			//m_Log << "Predator died at (" << pos.x << "," << pos.y << ")\n";
		}
		else
		{
			++it;
		}
	}
}

GameInfo * Manager::GetGameInfo(const MicrobeID_t id)
{
	GameInfo * result = new GameInfo();

	if (m_Microbes.find(id) == m_Microbes.end())
		return NULL;

	Point pos = m_Microbes[id]->GetPosition();
	DecidingWatcher * watcher = dynamic_cast<DecidingWatcher *>(m_Microbes[id]);
	if (watcher == NULL)
		return NULL;
	int radius = watcher->GetVisionRadius();
	for(int offsetX = -radius; offsetX <= radius; ++offsetX)
	{
		for(int offsetY = -radius; offsetY <= radius; ++offsetY)
		{
			if (offsetX * offsetX + offsetY * offsetY > radius * radius)
				continue;

			Point curPos(pos.x + offsetX, pos.y + offsetY);

			if (curPos.x < 0 || curPos.x >= m_Columns ||
				curPos.y < 0 || curPos.y >= m_Rows)
				continue;

			if (m_Field[curPos.x][curPos.y] == NULL)
				continue;

			Microbe * curMicrobe = dynamic_cast<Microbe *> (m_Field[curPos.x][curPos.y]);
			ObjectInfo info;

			if (curMicrobe == NULL)
			{
				info.m_Type = ObjectInfo::otBlock;
				result->info.insert(make_pair(curPos, info));
				continue;
			}


			if (dynamic_cast<Predator *>(curMicrobe) == NULL)
			{
				info.m_Type = ObjectInfo::otVictim;
			}
			else
			{
				info.m_Type = ObjectInfo::otPredator;
			}
			
			info.m_Id = curMicrobe->GetId();
			if (info.m_Id != id)
				result->info.insert(make_pair(curPos, info));

		}
	}
	return result;
}

void Manager::UpdateBlocks()
{
	for(size_t index = 0; index < m_Blocks.size(); ++index)
	{
		Point pos = m_Blocks[index]->GetPosition();
		m_Field[pos.x][pos.y] = m_Blocks[index];
	}
}

void Manager::UpdateMicrobePos()
{
	for (size_t posX = 0; posX < m_Columns; ++posX)
	{
		for (size_t posY = 0; posY < m_Rows; ++posY) 
		{
			Microbe * curMicrobe = dynamic_cast<Microbe *>(m_Field[posX][posY]);
			if (curMicrobe != NULL) 
			{
				curMicrobe->Move(Point(posX, posY));
			}
		}
	}
}

void Manager::UpdateMicrobeMap()
{
	m_Microbes.clear();
	for (size_t posX = 0; posX < m_Columns; ++posX)
	{
		for (size_t posY = 0; posY < m_Rows; ++posY) 
		{
			Microbe * curMicrobe = dynamic_cast<Microbe *>(m_Field[posX][posY]);
			if (curMicrobe != NULL) 
			{
				m_Microbes.insert(make_pair(curMicrobe->GetId(), curMicrobe));
			}
		}
	}
}

void Manager::PrintState() const
{
	size_t count = 0, predCount = 0;
	string st;
	istringstream iss(m_Log.str());
	for(size_t posY = 0; posY < m_Rows; ++posY)
	{
		for (size_t posX = 0; posX < m_Columns; posX++)
		{
			Object * curObject = m_Field[posX][posY];
			if (curObject == NULL)
				cout << " ";
			else
				cout << curObject->ToChar();
			if (dynamic_cast<Victim *>(curObject))
				++count;
			if (dynamic_cast<Predator *>(curObject))
				++predCount;
		}
		getline(iss, st);
		cout << "\t" << st << endl;
	}
	for(map<MicrobeID_t, Microbe *>::const_iterator it = m_Microbes.begin(); it != m_Microbes.end(); ++it)
	{
		if (dynamic_cast<Victim *>(it->second))
			++count;
		if (dynamic_cast<Predator *>(it->second))
			++predCount;
	}
	cout << endl;
}

Manager::~Manager(void)
{
}

void Manager::CountMicrobes()
{
	size_t victimsCount = 0, predatorsCount = 0;
	for(map<MicrobeID_t, Microbe *>::const_iterator it = m_Microbes.begin(); it != m_Microbes.end(); ++it)
	{
		if (dynamic_cast<Victim *>(it->second))
			++victimsCount;
		if (dynamic_cast<Predator *>(it->second))
			++predatorsCount;
	}
	m_Log << "Victims count: " << victimsCount << endl;
	m_Log << "Predators count: " << predatorsCount << endl;
}

void Point::MoveRandom(bool CanStay)
{
	int type;
	if (CanStay)
		type = rand() % 5;
	else
		type = rand() % 4;
	/*  0 - Left;	1 - Right
		2 - Up; 3 - Down; 4 - Stay			*/
	switch(type)
	{
	case 0:
		x -= 1;
		break;
	case 1:
		x += 1;
		break;
	case 2:
		y += 1;
		break;
	case 3:
		y -= 1;
		break;
	}
}

void Point::Move(Directions direction)
{
	switch(direction)
	{
	case dLeft:
		x -= 1;
		break;
	case dRight:
		x += 1;
		break;
	case dTop:
		y += 1;
		break;
	case dBottom:
		y -= 1;
		break;
	}
}

void Microbe::MakeRandomDecision()
{
	m_Decision.m_newPosition = m_Position;
	m_Decision.m_newPosition.MoveRandom(true);
	if (m_Decision.m_newPosition == m_Position)
		m_Decision.m_DecisionType = STAY;
	else
		m_Decision.m_DecisionType = MOVE;
}

void Predator::MakeDecision()
{
	MakeRandomDecision();
}

void Victim::MakeDecision()
{
	if (NeedReproduce())
		return;
	MakeRandomDecision();
}

Microbe * Predator::Clone() const
{
	return new Predator(*this);
}

Microbe * Victim::Clone() const
{
	return new Victim(*this);
}

Microbe * SoloWatchingVictim::Clone() const
{
	return new SoloWatchingVictim(*this);
}

Microbe * GroupWatchingVictim::Clone() const 
{
	return new GroupWatchingVictim(*this);
}

Decision DecidingWatcher::GetWatchDecision(Point position, GameInfo * info)
{
	DirectionsInfo dirInfo(position, info);
	int prefer = 0;
	double preferDirectionCoef = std::numeric_limits<int>::min();
	for (int direction = 0; direction < 4; ++direction)
	{
		// (Victims, Predators)
		std::pair<double, double> counts = dirInfo.GetInfoByDirection(direction);
		double directionCoef = FigureOutCoef(counts.first, counts.second);

		Point tmpPoint = position;
		tmpPoint.Move(static_cast<Directions>(direction));
		if (info->info.find(tmpPoint) != info->info.end() &&
			info->info[tmpPoint].m_Type == ObjectInfo::otBlock)
			directionCoef = std::numeric_limits<int>::min();

		if (directionCoef > preferDirectionCoef)
		{
			prefer = direction;
			preferDirectionCoef = directionCoef;
		}
	}

	Decision result;

	result.m_newPosition = position;
    result.m_IsIndividual = true;

	if (preferDirectionCoef < 0)
	{
		result.m_DecisionType = STAY;
        result.m_DecisionCoefficent = 0;
	}
	else
	{
		result.m_DecisionType = MOVE;
		result.m_newPosition.Move(static_cast<Directions>(prefer));
        result.m_DecisionCefficent = directionCoef;
	}

	return result;
}

void GroupWatchingVictim::MakeDecision() 
{
	if (NeedReproduce())
		return;
	GameInfo * info = GetManager()->GetGameInfo(GetId());
	m_Decision = GetWatchDecision(GetPosition(), info);
	delete info;
}

double GroupWatchingVictim::FigureOutCoef(double victimsCount, double predatorsCount) const
{
	return victimsCount - predatorsCount;
}

double SoloWatchingVictim::FigureOutCoef(double victimsCount, double predatorsCount) const
{
	return -5 * predatorsCount - victimsCount; // magic constant
}

void SoloWatchingVictim::MakeDecision()
{
	if (NeedReproduce())
		return;
	GameInfo * info = GetManager()->GetGameInfo(GetId());
	m_Decision = GetWatchDecision(GetPosition(), info);
	//GetManager()->m_Log << "Solo decided to go from (" << m_Position.x << "," << m_Position.y <<
		//") to (" << m_Decision.m_newPosition.x << "," << m_Decision.m_newPosition.y << ")\n";
	delete info;
}

double OptimicticWatchingPredator::FigureOutCoef(double victimsCount, double predatorsCount) const
{
	return victimsCount - predatorsCount;
}

void OptimicticWatchingPredator::MakeDecision()
{ 
	GameInfo * info = GetManager()->GetGameInfo(GetId());
	m_Decision = GetWatchDecision(GetPosition(), info);
	delete info;
}

double RealisticWatchingPredator::FigureOutCoef(double victimsCount, double predatorsCount) const
{
	return victimsCount * 1.0 / (predatorsCount + 1);
}

void RealisticWatchingPredator::MakeDecision()
{
	GameInfo * info = GetManager()->GetGameInfo(GetId());
	m_Decision = GetWatchDecision(GetPosition(), info);
	delete info;
}

Answer CooperateMessenger::ProcessMessage(const Message & message) const 
{
    Decision cooperativeDecision;

    cooperativeDecision.m_IsIndividual = false;
    cooperativeDecision.m_newPosition = PointTowardBasis(message.GetBasis());
    cooperativeDecision.m_DecisionType = MOVE;
    cooperativeDecision.m_DecisionCoefficent = message.GetDensity();

    if (CompareAndSetDecisions(cooperativeDecision))
    {
        return Answer(Answer::atAccept);
    }
    else
    {
        return Answer(Answer::atReject);
    }
}

void CooperateMessenger::Cooperate(const GameInfo * gameInfo, const Decision & plan, const Point & curPos) const
{
    // calculate basis
    Point basis(curPos);
    Direction direction = ToDirection(curPos, plan.m_newPosition);
    size_t stepNumber = 0;
    bool endOfCycle = true;
    while (endOfCycle && stepNumber < 4) 
    {
        Point nextStep(basis)
        nextStep.Move(static_cast<Direction>(direction));

        map<Point, ObjectInfo>::const_iterator it = gameInfo->info.find(nextStep);

        if (it != gameInfo->info.end() && it->m_Type == ObjectInfo::otBlock)
        {
            endOfCycle = true
        }
        else
        {
           basis = nextStep;
           ++stepNumber;
        }
    }
    
    Message message(basis, Message::mtCooperate, plan.m_DecisionCoefficent);

    // sending message to all microbes
    for (map<Point, ObjectInfo>::const_iterator i = gameInfo->info.begin(); i != gameInfo->end(); ++i) 
    {
        this->GetManager()->SendMessage(this, i->second.m_Id, message);
    }
}

void CooperatePredator::MakeIndividualDecision()
{
	GameInfo * info = GetManager()->GetGameInfo(GetId());
	m_Decision = GetWatchDecision(GetPosition(), info);
    return info; 
}

void CooperatePredator::MakeDecision()
{
    GameInfo * info;
    if (m_Decision.m_DecisionType == NONE)
    {
        info = MakeIndividualDecision();
    }
    else
    {
        info = GetManager()->GetGameInfo(GetId());
    }
    Cooperate(info, GetDecision(), GetPosition());
    delete info;
}



void Utilities::SplitParams(const string & desc, vector<string> & params) 
{
	istringstream iss(desc);
	params.clear();
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(params));
}

size_t Utilities::ToNumber(const string & number) 
{
	size_t result;
	stringstream convert(number);
	convert >> result;
	return result;
}

Direction ToDirection(const Point & from, const Point & to)
{
    if (from.x < to.x) 
    {
        return dRight;
    }

    if (from.x > to.x)
    {
        return dLeft;
    }

    if (from.y > to.y)
    {
        return dTop;
    }

    return dBottom;
}

MicrobeID_t ObjectFactory::GetNextId()
{
	return m_MicrobeIdCounter++;
}

Object * ObjectFactory::CreateObject(const string description)
{
	vector<string> params;
	Utilities::SplitParams(description, params);
	unsigned type = Utilities::ToNumber(params[0]);
	switch(type)
	{
	case 0:
		return CreateBlock(params);
	case 1:
		return CreatePredator(params);
	case 2:
		return CreateVictim(params);
	case 3:
		return CreateSoloVictim(params);
	case 4:
		return CreateGroupVictim(params);
	case 5:
		return CreateOptimicticPredator(params);
	case 6:
		return CreateRealisticPredator(params);
	}
}

Block * ObjectFactory::CreateBlock(const vector<string> & params) 
{
	size_t x, y;
	x = Utilities::ToNumber(params[1]);
	y = Utilities::ToNumber(params[2]);
	return new Block(x, y);
}

Block * ObjectFactory::CreateBlock(size_t x, size_t y)
{
	return new Block(x, y);
}

Microbe * ObjectFactory::GetClone(Microbe * parent)
{
	Microbe * clone = parent->Clone();
	clone->SetId(GetNextId());
	clone->ClearDecision();
	return clone;
}

Predator * ObjectFactory::CreatePredator(const vector<string> & params)
{
	size_t x, y;
	unsigned initialStamina;
	x = Utilities::ToNumber(params[1]);
	y = Utilities::ToNumber(params[2]);
	initialStamina = Utilities::ToNumber(params[3]);
	MicrobeID_t id = GetNextId();
	return new Predator(x, y, id, initialStamina);
}

Victim * ObjectFactory::CreateVictim(const vector<string> & params) 
{
	size_t x, y;
	unsigned reproduceSpeed, worth;
	x = Utilities::ToNumber(params[1]);
	y = Utilities::ToNumber(params[2]);
	reproduceSpeed = Utilities::ToNumber(params[3]);
	worth = Utilities::ToNumber(params[4]);
	MicrobeID_t id = GetNextId();
	return new Victim(x, y, id, reproduceSpeed, worth);
}

SoloWatchingVictim * ObjectFactory::CreateSoloVictim(const vector<string> & params)
{
	size_t x, y;
	unsigned reproduceSpeed, worth, visionRadius;
	x = Utilities::ToNumber(params[1]);
	y = Utilities::ToNumber(params[2]);
	reproduceSpeed = Utilities::ToNumber(params[3]);
	worth = Utilities::ToNumber(params[4]);
	visionRadius = Utilities::ToNumber(params[5]);
	MicrobeID_t id = GetNextId();
	return new SoloWatchingVictim(x, y, id, reproduceSpeed, worth, visionRadius);
}

GroupWatchingVictim * ObjectFactory::CreateGroupVictim(const vector<string> & params)
{
	size_t x, y;
	unsigned reproduceSpeed, worth, visionRadius;
	x = Utilities::ToNumber(params[1]);
	y = Utilities::ToNumber(params[2]);
	reproduceSpeed = Utilities::ToNumber(params[3]);
	worth = Utilities::ToNumber(params[4]);
	visionRadius = Utilities::ToNumber(params[5]);
	MicrobeID_t id = GetNextId();
	return new GroupWatchingVictim(x, y, id, reproduceSpeed, worth, visionRadius);
}

OptimicticWatchingPredator * ObjectFactory::CreateOptimicticPredator(const vector<string> & params)
{
	size_t x, y;
	unsigned initialStamina, visionRadius;
	x = Utilities::ToNumber(params[1]);
	y = Utilities::ToNumber(params[2]);
	initialStamina = Utilities::ToNumber(params[3]);
	visionRadius = Utilities::ToNumber(params[4]);
	MicrobeID_t id = GetNextId();
	return new OptimicticWatchingPredator(x, y, id, initialStamina, visionRadius);
}

RealisticWatchingPredator * ObjectFactory::CreateRealisticPredator(const vector<string> & params)
{
	size_t x, y;
	unsigned initialStamina, visionRadius;
	x = Utilities::ToNumber(params[1]);
	y = Utilities::ToNumber(params[2]);
	initialStamina = Utilities::ToNumber(params[3]);
	visionRadius = Utilities::ToNumber(params[4]);
	MicrobeID_t id = GetNextId();
	return new RealisticWatchingPredator(x, y, id, initialStamina, visionRadius);
}
