#include <bits/stdc++.h>
using namespace std;


/// Genenal Previous Declaration
enum ElementType {PATH, WALL, FAKE_WALL};
enum RobotType {C = 0, S, W, SW};
enum ItemType {MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD};

// Functions in head


// Classes in head
class MapElement;
class Path;
class Wall;
class FakeWall;
class Position;
class Map;
class Configuration;

class MovingObject;
class Character;

class SherLock;
class Watson;

class Criminal;
class Robot;
class RobotC;
class RobotS;
class RobotW;
class RobotSW;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class MagicBook;
class EnergyDrink;
class FirstAid;
class ExcemptionCard;
class PassingCard;

class BaseBag;
class SherlockBag;
class WatsonBag;



// Classes
class MapElement
{
	public :
		MapElement() {};
		MapElement(ElementType in_type) : type(in_type) {}
		virtual ~MapElement() {};
		
		virtual ElementType getType() const = 0;
		
		void setType(ElementType type)
		{ this->type = type; }

	protected :
		ElementType type;
};

class Path : public MapElement
{
    public :
        Path() { this->type = PATH; }
		~Path() {}

		ElementType getType() const override { return type; }
};

class Wall : public MapElement
{
    public :
        Wall() { this->type = WALL; }
		~Wall() {}

		ElementType getType() const override { return type; }
};

class FakeWall : public MapElement
{
    public :
        FakeWall() { this->type = FAKE_WALL; }
		FakeWall(int in_reqEXP) : reqEXP(in_reqEXP) { this->type = FAKE_WALL; }
		~FakeWall() {}
		
		int getReqExp() const { return reqEXP; }
		ElementType getType() const override { return type; }

    private :
        int reqEXP;
};

class Position
{
    public :
        Position() {}
		Position(int r, int c) : row(r), col(c) {} 
		Position(const string& str_pos)
		{
		    string temp;
		    int i = 1;
		    while (isdigit(str_pos[i])) temp += str_pos[i++];
		    row = stoi(temp);
		    i++;
		    while (isdigit(str_pos[i])) temp += str_pos[i++];
		    col = stoi(temp);
		}
		Position(const Position& pos) : row(pos.getRow()), col(pos.getCol()) {}
		
		int getRow() const { return row; }
		int getCol() const { return col; }
		void setRow(int r) { row = r; }
		void setCol(int c) { col = c; }
		string str() const { return "(" + to_string(row) + "," + to_string(col) + ")"; }
		bool isEqual (int in_r, int in_c) const { return (in_r == row) and (in_c == col); }
		
		Position& operator = (const Position& pos)
		{
		    this->row = pos.getRow();
		    this->col = pos.getCol();
		
		    return *this;
		}
		bool operator == (const Position& pos)
		{
		    return (this->row == pos.getRow()) and (this->col == pos.getCol());
		}
		bool operator != (const Position& pos)
		{
		    return (this->row != pos.getRow()) or (this->col != pos.getCol());
		}

        static const Position npos;
    private :
        int row;
        int col;
           
};
const Position Position::npos = Position(-1, -1);

class MovingObject
{
    public :
        MovingObject() {}
		MovingObject(int index, const Position& pos, Map *map, const string& name = "")
		{
		    this->index = index;
		    this->name = name;
		    this->pos.setRow(pos.getRow());
		    this->pos.setCol(pos.getCol());
		    this->map = map;
		}
		virtual ~MovingObject() {}
        
        virtual Position getNextPosition() = 0;
        virtual Position getCurrentPosition() const = 0;
        virtual void move() = 0;
        virtual string str() const = 0;

        int getIndex() const { return index; }
        Position getPosition() const { return pos; }
		string getName() const { return name; }
		void setIndex(int index) { this->index = index; }
		void setPosition(const Position& pos) { this->pos = pos; }
		void setName(string name) { this->name = name; }

    protected :
        int index;
        Position pos;
        Map *map;
        string name;

};

class Map
{
    public:
        Map() {}
		Map(int num_rows, int num_cols, int num_walls, int num_fake_walls, Position *array_walls, Position *array_fake_walls)
		    : numRows(num_rows), numCols(num_cols)
		{ 
		    map = new MapElement**[num_rows]; 
		    for (int i = 0; i < num_rows; i++)
		    {
		        map[i] = new MapElement*[num_cols];
		        for (int j = 0; j < num_cols; j++) 
		            map[i][j] = new Path; 
		    }
		
		    for (int i = 0; i < num_walls; i++)
		    {
		    	int rowWall = array_walls[i].getRow();
                int colWall = array_walls[i].getCol(); 
				delete map[rowWall][colWall]; // Xóa đối tượng cũ trước khi gán giá trị mới
				map[rowWall][colWall] = new Wall;
		    }
	
            for (int i = 0; i < num_fake_walls; i++)
		    {
		    	int rowFakeWall = array_fake_walls[i].getRow();
                int colFakeWall = array_fake_walls[i].getCol();
				delete map[rowFakeWall][colFakeWall]; // Xóa đối tượng cũ trước khi gán giá trị mới
				int fake_wall_exp = (rowFakeWall * 257 + colFakeWall * 139 + 89) % 900 + 1; // Calculate fake wall experience
        		map[rowFakeWall][colFakeWall] = new FakeWall(fake_wall_exp); 
		    }
		}
		~Map() 
		{
		    for (int i = 0; i < numRows; i++)
		        delete [] map[i];
		    delete [] map;
		}
		
		
		int getNumRows() const { return numRows; }
		int getNumCols() const { return numCols; }    
		MapElement*** getMap() const { return map; }    
		void setNumRows(int numRows) { this->numRows = numRows; }
		void setNumCols(int numCols) { this->numCols = numCols; }
		
		bool isValid(const Position& pos, MovingObject *mv_obj) const;
		
		void printElement()
		{
			for (int i = 0; i < numRows; i++)
			{
				for (int j = 0; j < numCols; j++)
				{
					ElementType type = map[i][j]->getType();
					switch (type)
					{
						case PATH : 
							cout << "P "; break;
						case WALL :
							cout << "W "; break;
						case FAKE_WALL:
							cout << "F "; break;
					}
				}
				cout << endl;
			}
		}

		void showFakeWallExperience() const
		{
			for (int i = 0; i < numRows; i++)
			{
				for (int j = 0; j < numCols; j++)
				{
					if (map[i][j]->getType() != FAKE_WALL) continue;
					FakeWall *fakewall = dynamic_cast<FakeWall*>(map[i][j]);
					cout << "\tFW(" << i << ", " << j << ") : " << fakewall->getReqExp() << endl;
				}
			}
		}

    private:
		int numRows;
        int numCols;
        MapElement ***map;
};

class Character : public MovingObject
{
	public :
		Character() : MovingObject() {}
		Character(int index, const Position& init_pos, Map *map, int init_hp, int init_exp) 
			: MovingObject(index, init_pos, map), hp(init_hp), exp(init_exp) 
		{
			hp = (init_hp <= 500) ? init_hp : 0;
		    exp = (init_exp <= 900) ? init_exp : 0;
		}

	protected :
		int hp;
		int exp;
};

class SherLock : public Character
{
    public:
		SherLock() : Character() {}
        SherLock(int index, const string& moving_rule, const Position& init_pos, Map *map, int init_hp, int init_exp)
		    : Character(index, init_pos, map, init_hp, init_exp), moving_rule(moving_rule)
		{
		    nextMove = 0;
		    setName("Sherlock");
		}
		~SherLock() {}
		        
		Position getNextPosition() override
		{
		    if (nextMove == moving_rule.length()) nextMove = 0;
		
		    Position newPos;
		    if (moving_rule[nextMove] == 'L') 
		        newPos = Position(pos.getRow(), pos.getCol() - 1);
		    if (moving_rule[nextMove] == 'R')
		        newPos = Position(pos.getRow(), pos.getCol() + 1);
		    if (moving_rule[nextMove] == 'U')
		        newPos = Position(pos.getRow() - 1, pos.getCol());
		    if (moving_rule[nextMove] == 'D')
		        newPos = Position(pos.getRow() + 1, pos.getCol());
		
		    if (map->isValid(pos, this))
		        return newPos;
		    else 
		        return Position::npos;
			return newPos;
		}
		Position getCurrentPosition() const override
		{
		    return getPosition();
		}
		void move() override
		{
		    Position nextPosition = getNextPosition();
		    nextMove++;
		    if (nextPosition == Position::npos) return;
		    pos = nextPosition;
		}
		string str() const override
		{
		    stringstream ss;
		    ss << "SherLock[index = " << index << "; ";
		    ss << "pos = " << pos.str() << "; ";
		    ss << "moving_rule = " << moving_rule << "]";
		    return ss.str();
		}
    
    private :
        string moving_rule;
        int nextMove;
};

class Watson : public Character
{
    public:
        Watson( int index, 
				        const string& moving_rule, 
				        const Position& init_pos,
				        Map *map, 
				        int init_hp, 
				        int init_exp)
		    : Character(index, init_pos, map, init_hp, init_exp), moving_rule(moving_rule)
		{
		    nextMove = 0;
		    setName("Watson");
		}
		        
		Position getNextPosition() override
		{
		    if (nextMove == moving_rule.length()) nextMove = 0;
		
		    Position newPos;
		    if (moving_rule[nextMove] == 'L') 
		        newPos = Position(pos.getRow(), pos.getCol() - 1);
		    if (moving_rule[nextMove] == 'R')
		        newPos = Position(pos.getRow(), pos.getCol() + 1);
		    if (moving_rule[nextMove] == 'U')
		        newPos = Position(pos.getRow() - 1, pos.getCol());
		    if (moving_rule[nextMove] == 'D')
		        newPos = Position(pos.getRow() + 1, pos.getCol());
		
		    if (map->isValid(pos, this))
		       return newPos;
		    else 
		        return Position::npos;
		
		}
		
		Position getCurrentPosition() const override { return getPosition(); }
		void move() override
		{
		    Position nextPosition = getNextPosition();
		    nextMove++;
		    if (nextPosition == Position::npos) return;
		    pos = nextPosition;
		}
		string str() const override
		{
		    stringstream ss;
		    ss << "Watson[index = " << index << "; ";
		    ss << "pos = " << pos.str() << "; ";
		    ss << "moving_rule = " << moving_rule << "]";
		    return ss.str();
		}

		int getHP() const { return hp; }
		int getEXP() const { return exp; }
        string getMovingRule() const { return moving_rule; }

		void setHP(int hp) { this->hp = hp; }
		void getEXP(int exp) { this->exp = exp; }
        void getMovingRule(string moving_rule) { this->moving_rule = moving_rule; }
    
    private :
        string moving_rule;
        int nextMove;
};

class Criminal : public Character
{
    public :
        Criminal(int index, const Position& init_pos, Map *map, SherLock *sherlock, Watson *watson) 
			: Character(index, init_pos, map, 0, 0), sherlock(sherlock), watson(watson)
		{
			setName("Criminal");
		}
		
        ~Criminal() {};

		void findMaxMahattan(const Position& nextPos, Position& resultPos, int maxDistance)
		{
			if (map->isValid(nextPos,this))
			{
				int rowDistanceSherLock = abs(sherlock->getPosition().getRow() - nextPos.getRow());
				int colDistanceSherLock = abs(sherlock->getPosition().getCol() - nextPos.getCol());
				int rowDistanceWatSon = abs(watson->getPosition().getRow() - nextPos.getRow());
				int colDistanceWatson = abs(watson->getPosition().getCol() - nextPos.getCol());
				if (rowDistanceSherLock + colDistanceSherLock + rowDistanceWatSon + colDistanceWatson > maxDistance)
				{
					maxDistance = rowDistanceSherLock + colDistanceSherLock + rowDistanceWatSon + colDistanceWatson;
					resultPos = nextPos;
				}
			}
		}
        Position getNextPosition() override
		{
			int maxDistance = 0;
			Position resultPos = Position::npos;
			
			findMaxMahattan(Position(pos.getRow(), pos.getCol() - 1), resultPos, maxDistance);
			findMaxMahattan(Position(pos.getRow(), pos.getCol() + 1), resultPos, maxDistance);
			findMaxMahattan(Position(pos.getRow() - 1, pos.getCol()), resultPos, maxDistance);
			findMaxMahattan(Position(pos.getRow() + 1, pos.getCol()), resultPos, maxDistance);

			return resultPos;
		}
		Position getCurrentPosition() const override { return getPosition(); }
		void move() override
		{
		    Position nextPosition = getNextPosition();
		
		    if (nextPosition == Position::npos) return;
		    pos = nextPosition;
		}
		string str() const override
		{
		    stringstream ss;
		    ss << "Criminal[index = " << index << "; ";
		    ss << "pos =" << pos.str() << "]";
		    return ss.str();
		}
		
		private :
		    SherLock *sherlock;
		    Watson *watson;
};

/*
Sau mỗi 3 bước đi của tên tội phạm một robot sẽ được tạo ra tại vị trí trước đó mà
tên tội phạm đang đứng. Các loại robot và điều kiện tạo ra tương ứng:
• Nếu là robot đầu tiên được tạo ra trên bản đồ, đó sẽ là loại robot RobotC. Nếu không,
ta xét khoảng cách từ Robot đến Sherlock và Watson:
• Nếu khoảng cách đến Sherlock gần hơn: Tạo ra loại robot RobotS
• Khoảng cách đến Watson gần hơn: Tạo ra loại robot RobotW
• Khoảng cách đến Sherlock và Watson là bằng nhau: Tạo ra loại robot RobotSW
*/

class Robot : public MovingObject
{
	public :
		Robot(int index, const Position& init_pos, Map *map) : MovingObject(index, init_pos, map)
		{}

		string str() const
		{
			stringstream ss;
			ss << "Robot[pos =" << pos.str() << ";";
			ss << "type =" << robot_type << ";";
			ss << "dist =";
			if (robot_type == C) ss << "\"\"";
			else ss << dist << "]";

			return ss.str();
		}

	protected :
		RobotType robot_type; // Kieu robot
		BaseItem *item; // thuoc tinh cua robot neu thang
		int dist; // Khoang cach tu robot den sherlock hoac watson hoac ca 2 tuy robot
};

class RobotC : public Robot
{
	public :
		RobotC (int index, const Position& init_pos, Map *map , Criminal *criminal)
			: Robot(index, init_pos, map)
		{
			this->index = index;
			this->pos = init_pos;
			this->map = map;
			this->criminal = criminal;
			this->name = "RobotC";
			this->robot_type = C;
			this->dist = 0;
		}

		Position getNextPosition();
        Position getCurrentPosition() const { return pos; }
        void move();

	private :
		Criminal *criminal;
};

class RobotS : public Robot
{
	public :
		RobotS (int index, const Position& init_pos, Map *map , Criminal *criminal, SherLock *sherlock)
			: Robot(index, init_pos, map)
		{
			this->index = index;
			this->pos = init_pos;
			this->map = map;
			this->criminal = criminal;
			this->sherlock = sherlock;
			this->name = "RobotS";
			this->robot_type = S;
		}

	private :
		Criminal *criminal;
		SherLock *sherlock;
};

class RobotW : public Robot
{
	public :
		RobotW (int index, const Position& init_pos, Map *map , Criminal *criminal, Watson *watson)
			: Robot(index, init_pos, map)
		{
			this->index = index;
			this->pos = init_pos;
			this->map = map;
			this->criminal = criminal;
			this->watson = watson;
			this->name = "RobotW";
			this->robot_type = W;
		}

	private :
		Criminal *criminal;
		Watson *watson;
};

class RobotSW : public Robot
{
	public :
		RobotSW (int index, const Position& init_pos, Map *map , Criminal *criminal, SherLock *sherlock , Watson *watson)
			: Robot(index, init_pos, map)
		{
			this->index = index;
			this->pos = init_pos;
			this->map = map;
			this->criminal = criminal;
			this->sherlock = sherlock;
			this->watson = watson;
			this->name = "RobotSW";
			this->robot_type = SW;
		}

	private :
		Criminal *criminal;
		SherLock *sherlock;
		Watson *watson;
};

// isValid for all characters
bool Map::isValid(const Position& pos, MovingObject *mv_obj) const
{  	
	bool check_row, check_col;
	if ((pos.getRow() < 0) or (pos.getRow() >= numRows)) check_row = true; else check_row = false;
	if ((pos.getCol() < 0) or (pos.getCol() >= numCols)) check_col = true; else check_col = false;
	if (check_row or check_col) return false;

	
	
	bool checkWall, checkFakeWall;
	ElementType check = map[pos.getRow()][pos.getCol()]->getType();
	if (check == WALL) checkWall = true; else checkWall = false;
	if (check == FAKE_WALL) checkFakeWall = true; else checkFakeWall = false;
	if (checkWall) return false;

	if (mv_obj->getName() == "Watson" and checkFakeWall)
	{
		Watson* watson = dynamic_cast<Watson*>(mv_obj);
		FakeWall* fakeWall = dynamic_cast<FakeWall*>(map[pos.getRow()][pos.getCol()]);
		return (watson->getEXP() > fakeWall->getReqExp());
	}

	return true;
}

class BaseItem
{
	public :
		BaseItem();

		virtual bool canUse(Character *obj , Robot *robot) = 0;
		virtual void use(Character *obj , Robot *robot) = 0;

	private :
		ItemType type;
};

class MagicBook : public BaseItem
{

};

class EnergyDrink : public BaseItem
{

};

class FirstAid : public BaseItem
{

};

class ExcemptionCard : public BaseItem
{

};

class PassingCard : public BaseItem
{

};

class BaseBag
{

};

class SherlockBag : public BaseBag
{

};

class WatsonBag : public BaseBag
{

};

class ArrayMovingObject
{
	public :
		ArrayMovingObject(int capacity) : capacity(capacity), count(0) 
		{ arr_mo_objs = new MovingObject*[capacity]; } 
		~ArrayMovingObject() { delete [] arr_mo_objs; }

		bool isFull() const { return count == capacity; }
		bool add(MovingObject *mv_obj) 
		{ 
			if (isFull()) return false;
			arr_mo_objs[count++] = mv_obj;
			return true;
		}
		string str() const
		{
			stringstream ss;
			ss << "ArrayMovingObject[count ";
			ss << getCount() << "; ";
			ss << "capacity = " << getCapacity();
			for (int i = 0; i < count; i++)
			{ 
				ss << "; ";
				ss << arr_mo_objs[i]->str();
			}
			ss << "]";

			return ss.str();
		}

		MovingObject** getArrayMovingObject() const { return arr_mo_objs; }
		int getCount() const { return count; }
		int getCapacity() const { return capacity; }

		void setCapacity(int capacity) { this->capacity = capacity; }

	private :
		MovingObject **arr_mo_objs;
		int count;
		int capacity;
};

class Configuration
{
	friend class StudyPinkProgram;

	public :
		Configuration(const string& filepath)
		{
			
		}
		~Configuration()
		{
			delete [] arr_walls;
			delete [] arr_fake_walls;
		}

		string str() const;

	private :
		// rows and cols of map
		int map_num_rows;
		int map_num_cols;

		// Walls
		int num_walls;
		Position *arr_walls;

		//FakeWalls
		int num_fake_walls;
		Position *arr_fake_walls;

		// Sherlock
		string sherlock_moving_rule;
		Position sherlock_init_pos;
		int sherlock_init_hp;
		int sherlock_init_exp;

		// Watson
		string watson_moving_rule;
		Position watson_init_pos;
		int watson_init_hp;
		int watson_init_exp;

		//Criminal
		Position criminal_init_pos;

		//Number of steps
		int num_steps;
};

class StudyPinkProgram
{
	public :

	private :

};



/////////////////////// Genetic Functions


main()
{
	// Create Map.....
	const int row = 10, col = 10; // row and column
	const int num_walls = 9; // numbers of wall
	Position array_wall[num_walls] = 
		{ 
			Position(1, 1), Position(2, 1), Position(3, 1), Position(3, 2),
			Position(6, 2), Position(6, 3), Position(6, 4), Position(7, 4),
			Position(6, 9)
		}; // array of walls
	const int num_fake_walls = 8;
	Position array_fake_walls[num_fake_walls] =
		{
			Position(0, 4), Position(1, 4), Position(2, 4),
			Position(1, 8),
			Position(4, 7),
			Position(8, 6), Position(9, 6), Position(9, 7),
		}; // array of fake walls
	Map map(row, col, num_walls, num_fake_walls, array_wall, array_fake_walls); // Map pointer
	map.printElement(); cout << endl; // Show map


	// Show Fakewall experience
	cout << "Show fake wall experience : " << endl;
	map.showFakeWallExperience(); cout << endl;


	// Making charater Sherlock Holmes.....
	const int indexSherlock = 1; // Index in array moving object of Holmes
	const string movingRuleSherLock = "UR"; // Holmes's moving rule
	const Position init_pos_Sherlock = Position(8, 1); // Initial Holmes's position
	const int init_hp_sherlock = 400; // Initial Holmes's HP
	const int init_exp_sherlock = 700; // Initial Holmes's experience
	SherLock sherlock(indexSherlock, movingRuleSherLock, init_pos_Sherlock, &map, init_hp_sherlock, init_exp_sherlock);
	cout << sherlock.str() << endl; // Show Holmes's informations


	// Making charater John Watson.....
	const int indexWatson = 2; // Index in array moving object of Watson
	const string movingRuleWatson = "LU"; // Watson's moving rule
	const Position init_pos_Watson = Position(7, 8); // Initial Watson's position
	const int init_hp_watson = 400; // Initial Watson's HP
	const int init_exp_watson = 300; // Initial Watson's experience
	Watson watson(indexWatson, movingRuleWatson, init_pos_Watson, &map, init_hp_watson, init_exp_watson);
	cout << watson.str() << endl; // Show Watson's informations


	// Making character Criminal.....
	const int indexCriminal = 3; // Index in array moving object of Criminal
	const Position init_pos_Criminal = Position(3, 5);
	Criminal criminal(indexCriminal, init_pos_Criminal, &map, &sherlock, &watson);
	cout << criminal.str() << endl << endl; // Show criminal's informations
	

	// Make array moving objects
	ArrayMovingObject arr_mov_objs(7);
	if(arr_mov_objs.add(&sherlock)) cout << "Added Sherlock" << endl;
	if(arr_mov_objs.add(&watson)) cout << "Added Watson" << endl;
	if(arr_mov_objs.add(&criminal)); cout << "Added criminal" << endl;
	cout << arr_mov_objs.str() << endl;
	cout << endl;
	
	// Test isValid for Sherlock Holmes
	cout << "Test isValid for Sherlock Holmes" << endl;
	cout << "\tPosition (4, 3) : "; if (map.isValid(Position(4, 3), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (5, 9) : "; if (map.isValid(Position(5, 9), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (6, 3) : "; if (map.isValid(Position(6, 3), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (2, 4) : "; if (map.isValid(Position(2, 4), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (13, 14) : "; if (map.isValid(Position(13, 14), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (2, 20) : "; if (map.isValid(Position(2, 20), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (14, 7) : "; if (map.isValid(Position(14, 7), &sherlock)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << endl;

	// Test isValid for Watson
	cout << "Test isValid for Watson" << endl;
	cout << "\tPosition (9, 9) : "; if (map.isValid(Position(9, 9), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (1, 4) : "; if (map.isValid(Position(1, 4), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (7, 4) : "; if (map.isValid(Position(7, 4), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (9, 7) : "; if (map.isValid(Position(9, 7), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (11, 7) : "; if (map.isValid(Position(11, 7), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (1, 13) : "; if (map.isValid(Position(1, 13), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (10, 11) : "; if (map.isValid(Position(10, 11), &watson)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << endl;

	// Test isValid for criminal
	cout << "Test isValid for criminal" << endl;
	cout << "\tPosition (10, 5) : "; if (map.isValid(Position(10, 5), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (6, 9) : "; if (map.isValid(Position(6, 9), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (2, 16) : "; if (map.isValid(Position(2, 16), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (8, 6) : "; if (map.isValid(Position(8, 6), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << "\tPosition (10, 16) : "; if (map.isValid(Position(10, 16), &criminal)) cout << "Valid"; else cout << "Invalid"; cout << endl;
	cout << endl;


	//Test Sherlock move
	for (int i = 0; i < 10; i++)
	{
		arr_mov_objs.getArrayMovingObject()[i % 3]->move(); 
		cout << arr_mov_objs.getArrayMovingObject()[i % 3]->str() << endl;
	}
}