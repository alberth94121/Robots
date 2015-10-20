// robots.cpp



#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the arena
const int MAXCOLS = 40;             // max number of columns in the arena
const int MAXROBOTS = 180;          // max number of robots allowed

const int UP    = 0;
const int DOWN  = 1;
const int LEFT  = 2;
const int RIGHT = 3;

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Robot declaration.

class Robot
{
  public:
        // Constructor
    Robot(Arena* ap, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;

        // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
};

class Player
{
  public:
        // Constructor
    Player(Arena *ap, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;
    int  age() const;
    bool isDead() const;

        // Mutators
    string takeComputerChosenTurn();
    void   stand();
    void   move(int dir);
    bool   shoot(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_age;
    bool   m_dead;
};

class Arena
{
  public:
        // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     robotCount() const;
    int     nRobotsAt(int r, int c) const;
    void    display(string msg) const;

        // Mutators
    bool   addRobot(int r, int c);
    bool   addPlayer(int r, int c);
    bool   destroyRobot(int r, int c);
    bool   moveRobots();

  private:
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Robot*  m_robots[MAXROBOTS];
    int     m_nRobots;
};

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();

        // Mutators
    void play();

  private:
    Arena* m_arena;
};

///////////////////////////////////////////////////////////////////////////
//  Robot implementation
///////////////////////////////////////////////////////////////////////////

Robot::Robot(Arena* ap, int r, int c)
{
    if (ap == NULL)
    {
        cout << "***** A robot must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Robot created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
}

int Robot::row() const
{
    return m_row;
}

int Robot::col() const
{
    return m_col; 
}

void Robot::move()
{
      // Attempt to move in a random direction; if we can't move, don't move
    switch (rand() % 4)
    {
      case UP:
		  if (row()-1 >= 1)
		  {
			  m_row -= 1;
		  }
          break;
      case DOWN:
		  if (row()+1 <= m_arena->rows())
		  {
			  m_row += 1;
		  }
          break;
      case LEFT:
		   if (col()-1 >= 1)
		   {
			   m_col -= 1;
		   }
		   break;
      case RIGHT:
		   if (col()+1 <= m_arena->cols())
		   {
			   m_col += 1;
		   }
           break;
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == NULL)
    {
        cout << "***** The player must be in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;  
}

int Player::col() const
{
    return m_col; 
}

int Player::age() const
{
    return m_age;  
}

string Player::takeComputerChosenTurn()
{       

    int dangerLeft = 0; //-1 = wall, >0 = dangerous
	int dangerRight = 0;
	int dangerUp = 0;
	int dangerDown = 0;
	int dangerNow = 0;
	int isLeft = 0; //0 is no adjacent robots
	int isRight = 0;
	int isUp = 0;
	int isDown = 0;
	//////////////////////////////////////////////////////////////
	//CHECK HOW DANGEROUS/////////////////////////////////////////
	//////////////////////////////////////////////////////////////
    // check left
	if (m_col - 1 < 1)
		dangerLeft = -1;
	else 
	{
		dangerNow += (m_arena->nRobotsAt(m_row, m_col-1));
		if (m_arena->nRobotsAt(m_row, m_col-1) >= 1)
			isLeft = 1;
		if (m_col - 2 >= 1)
			dangerLeft += (m_arena->nRobotsAt(m_row, m_col-2));
		if (m_row - 1 >= 1)
			dangerLeft += (m_arena->nRobotsAt(m_row-1, m_col-1));
		if (m_row + 1 <= m_arena->rows())
			dangerLeft += (m_arena->nRobotsAt(m_row+1, m_col-1));
	}
	// check right
	if (m_col + 1 > m_arena->cols())
		dangerRight = -1;
	else 
	{
		dangerNow += (m_arena->nRobotsAt(m_row, m_col+1));
		if ((m_arena->nRobotsAt(m_row, m_col+1)) >= 1)
			isRight = 1;
		if (m_col + 2 <= m_arena->cols())
			dangerRight += (m_arena->nRobotsAt(m_row, m_col+2));
		if (m_row - 1 >= 1)
			dangerRight += (m_arena->nRobotsAt(m_row-1, m_col+1));
		if (m_row + 1 <= m_arena->rows())
			dangerRight += (m_arena->nRobotsAt(m_row+1, m_col+1));
	}
	// check up
	if (m_row - 1 < 1)
		dangerUp = -1;
	else 
	{
		dangerNow += m_arena->nRobotsAt(m_row-1, m_col);
		if (m_arena->nRobotsAt(m_row-1, m_col) >= 1)
			isUp = 1;
		if (m_row - 2 >= 1)
			dangerUp += (m_arena->nRobotsAt(m_row-2, m_col));
		if (m_col - 1 >= 1)
			dangerUp += (m_arena->nRobotsAt(m_row-1, m_col-1));
		if (m_col + 1 <= m_arena->cols())
			dangerUp += (m_arena->nRobotsAt(m_row-1, m_col+1));
	}
	// check down
	if (m_row + 1 > m_arena->rows())
		dangerDown = -1;
	else 
	{
		dangerNow += (m_arena->nRobotsAt(m_row+1, m_col));
		if ((m_arena->nRobotsAt(m_row+1, m_col)) >= 1)
			isDown = 1;
		if (m_row + 2 <= m_arena->rows())
			dangerDown += (m_arena->nRobotsAt(m_row+2, m_col));
		if (m_col - 1 >= 1)
			dangerDown += (m_arena->nRobotsAt(m_row+1, m_col-1));
		if (m_col + 1 <= m_arena->cols())
			dangerDown += (m_arena->nRobotsAt(m_row+1, m_col+1));
	}
	////////////////////////////////////////////////////////////
	//DECIDE ACTION BASED ON DANGER LEVEL///////////////////////
	////////////////////////////////////////////////////////////
	//If no danger shoot in random direction or stand////////
	/////////////////////////////////////////////////////
	if (dangerNow == 0) 
	{
		for (int r = m_row;r >= 1;r--) //check to shoot up
		{
			if ((m_arena->nRobotsAt(r, m_col)) >= 1)
			{
				if (shoot(UP))
					return "Shot and hit!";
				else return "Shot and missed!";
			}
		}
		for (int s = m_row;s <= m_arena->rows();s++) //check to shoot down
		{
			if ((m_arena->nRobotsAt(s, m_col)) >= 1)
			{
				if (shoot(DOWN))
					return "Shot and hit!";
				else return "Shot and missed!";
			}
		}
		for (int t = m_col;t >= 1;t--) //check to shoot left
		{
			if ((m_arena->nRobotsAt(m_row, t)) >= 1)
			{
				if (shoot(LEFT))
					return "Shot and hit!";
				else return "Shot and missed!";
			}
		}
		for (int k = m_col;k <= m_arena->cols();k++) //check to shoot right
		{
			if ((m_arena->nRobotsAt(m_row, k)) >= 1)
			{
				if (shoot(RIGHT))
					return "Shot and hit!";
				else return "Shot and missed!";
			}
		}
		stand();
        return "Stood.";
	}
	//////////////////////////////////////////////
	//DECIDE ACTION IF WALL ON THE LEFT/////////////////////
	//////////////////////////////////////////////
	else if (dangerLeft == -1) //check if on left wall
	{
		if (dangerDown == -1) //check bottom left corner
		{
			if (dangerNow <= dangerUp && dangerNow <= dangerRight)
			{
				if (isRight == 1)
				{
					if (shoot(RIGHT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isUp == 1)
				{
					if (shoot(UP))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
			}
			else if (dangerUp <= dangerNow && dangerUp <= dangerRight)
			{
				move(UP);
				return "Moved.";
			}
			else if (dangerRight <= dangerNow && dangerRight <=dangerUp)
			{
				move(RIGHT);
				return "Moved.";
			}

		}
		else if (dangerUp == -1) //check upper left corner
		{
			if (dangerNow <= dangerDown && dangerNow <= dangerRight)
			{
				if (isRight == 1)
				{
					if (shoot(RIGHT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isDown == 1)
				{
					if (shoot(DOWN))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
			}
			else if (dangerDown <= dangerNow && dangerDown <= dangerRight)
			{
				move(DOWN);
				return "Moved.";
			}
			else if (dangerRight <= dangerNow && dangerRight <=dangerDown)
			{
				move(RIGHT);
				return "Moved.";
			}
		}
		else //check on left wall
		{
			if (dangerNow <= dangerDown && dangerNow <= dangerRight && dangerNow <= dangerUp) //if no danger shoot closest robot
			{
				if (isRight == 1)
				{
					if (shoot(RIGHT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isDown == 1)
				{
					if (shoot(DOWN))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isUp == 1)
				{
					if (shoot(UP))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}

			}
			else if (dangerDown <= dangerNow && dangerDown <= dangerRight && dangerDown <= dangerUp) //if danger >= 1 move to least dangerous spot
			{
				move(DOWN);
				return "Moved.";
			}
			else if (dangerRight <= dangerNow && dangerRight <=dangerDown && dangerRight <= dangerUp)
			{
				move(RIGHT);
				return "Moved.";
			}
			else if (dangerUp <= dangerNow && dangerUp <=dangerDown && dangerUp <= dangerRight)
			{
				move(UP);
				return "Moved.";
			}
		}
		stand();
		return "Stood.";
	}
	///////////////////////////////
	//DECIDE ACTION ON RIGHT WALL//////////
	////////////////////////////////
	else if (dangerRight == -1)
	{
		if (dangerDown == -1) //check bottom right corner
		{
			if (dangerNow <= dangerUp && dangerNow <= dangerLeft)
			{
				if (isLeft == 1)
				{
					if (shoot(LEFT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isUp == 1)
				{
					if (shoot(UP))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
			}
			else if (dangerUp <= dangerNow && dangerUp <= dangerLeft)
			{
				move(UP);
				return "Moved.";
			}
			else if (dangerLeft <= dangerNow && dangerLeft <=dangerUp)
			{
				move(LEFT);
				return "Moved.";
			}

		}
		else if (dangerUp == -1) //check upper right corner
		{
			if (dangerNow <= dangerDown && dangerNow <= dangerLeft)
			{
				if (isLeft == 1)
				{
					if (shoot(LEFT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isDown == 1)
				{
					if (shoot(DOWN))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
			}
			else if (dangerDown <= dangerNow && dangerDown <= dangerLeft)
			{
				move(DOWN);
				return "Moved.";
			}
			else if (dangerLeft <= dangerNow && dangerLeft <=dangerDown)
			{
				move(LEFT);
				return "Moved.";
			}
		}
		else //check on right wall
		{
			if (dangerNow <= dangerDown && dangerNow <= dangerLeft && dangerNow <= dangerUp) //if no danger shoot closest robot
			{
				if (isLeft == 1)
				{
					if (shoot(LEFT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isDown == 1)
				{
					if (shoot(DOWN))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isUp == 1)
				{
					if (shoot(UP))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}

			}
			else if (dangerDown <= dangerNow && dangerDown <= dangerLeft && dangerDown <= dangerUp) //if danger >= 1 move to least dangerous spot
			{
				move(DOWN);
				return "Moved.";
			}
			else if (dangerLeft <= dangerNow && dangerLeft <=dangerDown && dangerLeft <= dangerUp)
			{
				move(LEFT);
				return "Moved.";
			}
			else if (dangerUp <= dangerNow && dangerUp <=dangerDown && dangerUp <= dangerLeft)
			{
				move(UP);
				return "Moved.";
			}
		}
		stand();
		return "Stood.";
	}
	///////////////////////////
	///action if on top wall/////////
	//////////////////////////
	else if (dangerUp == -1)
	{
			if (dangerNow <= dangerDown && dangerNow <= dangerLeft && dangerNow <= dangerRight) //if no danger shoot closest robot
			{
				if (isLeft == 1)
				{
					if (shoot(LEFT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isDown == 1)
				{
					if (shoot(DOWN))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isRight == 1)
				{
					if (shoot(RIGHT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}

			}
			else if (dangerDown <= dangerNow && dangerDown <= dangerLeft && dangerDown <= dangerRight) //if danger >= 1 move to least dangerous spot
			{
				move(DOWN);
				return "Moved.";
			}
			else if (dangerLeft <= dangerNow && dangerLeft <=dangerDown && dangerLeft <= dangerRight)
			{
				move(LEFT);
				return "Moved.";
			}
			else if (dangerRight <= dangerNow && dangerRight <=dangerDown && dangerRight <= dangerLeft)
			{
				move(RIGHT);
				return "Moved.";
			}
		    stand();
		    return "Stood.";
	}
	///////////////////////////
	///action if on bottom wall/////////
	//////////////////////////
	else if (dangerDown == -1)
	{
			if (dangerNow <= dangerUp && dangerNow <= dangerLeft && dangerNow <= dangerRight) //if no danger shoot closest robot
			{
				if (isLeft == 1)
				{
					if (shoot(LEFT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isUp == 1)
				{
					if (shoot(UP))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isRight == 1)
				{
					if (shoot(RIGHT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}

			}
			else if (dangerUp <= dangerNow && dangerUp <= dangerLeft && dangerUp <= dangerRight) //if danger >= 1 move to least dangerous spot
			{
				move(UP);
				return "Moved.";
			}
			else if (dangerLeft <= dangerNow && dangerLeft <=dangerUp && dangerLeft <= dangerRight)
			{
				move(LEFT);
				return "Moved.";
			}
			else if (dangerRight <= dangerNow && dangerRight <=dangerUp && dangerRight <= dangerLeft)
			{
				move(RIGHT);
				return "Moved.";
			}
		    stand();
		    return "Stood.";
	}
	////////////////////////////
	/////not against any wall////
	///////////////////////////
	else
	{
			if (dangerNow <= dangerUp && dangerNow <= dangerLeft && dangerNow <= dangerRight && dangerNow <= dangerDown) //if no danger shoot closest robot
			{
				if (isLeft == 1)
				{
					if (shoot(LEFT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isUp == 1)
				{
					if (shoot(UP))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isRight == 1)
				{
					if (shoot(RIGHT))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}
				if (isDown == 1)
				{
					if (shoot(DOWN))
						return "Shot and hit!";
				    else return "Shot and missed!";
				}

			}
			else if (dangerUp <= dangerNow && dangerUp <= dangerLeft && dangerUp <= dangerRight && dangerUp <= dangerDown) //if danger >= 1 move to least dangerous spot
			{
				move(UP);
				return "Moved.";
			}
			else if (dangerLeft <= dangerNow && dangerLeft <=dangerUp && dangerLeft <= dangerRight && dangerLeft <= dangerDown)
			{
				move(LEFT);
				return "Moved.";
			}
			else if (dangerRight <= dangerNow && dangerRight <=dangerUp && dangerRight <= dangerLeft && dangerRight <= dangerDown)
			{
				move(RIGHT);
				return "Moved.";
			}
			else if (dangerDown <= dangerNow && dangerDown <=dangerUp && dangerDown <= dangerLeft && dangerDown <= dangerRight)
			{
				move(DOWN);
				return "Moved.";
			}
		    stand();
		    return "Stood.";
	}
}

void Player::stand()
{
    m_age++;
}

void Player::move(int dir)
{
    m_age++;
    switch (dir)
    {
      case UP:
          if (row()-1 >= 1)
		  {
			  m_row -= 1;
		  }
          break;
      case DOWN:
		   if (row()+1 <= m_arena->rows())
		   {
			   m_row += 1;
		   }
		   break;
      case LEFT:
		  if (col()-1 >= 1)
		  {
			  m_col -= 1;
		  }
          break;
      case RIGHT:
		   if (col()+1 <= m_arena->cols())
		   {
			   m_col += 1;
		   }
		   break;
    }
}

bool Player::shoot(int dir)
{
    m_age++;
    if (rand() % 3 != 0)  // miss with 2/3 probability
        return false;
	switch (dir)
    {
      case UP:
          for (int a = m_row; a >= 1; a--)
		  {
			  if (m_arena->destroyRobot(a,m_col))
			      return true;
		  }
          break;
      case DOWN:
		  for (int b = m_row; b <= m_arena->rows(); b++)
		  {
			  if (m_arena->destroyRobot(b,m_col))
			      return true;
		  }
		  break;
      case LEFT:
		  for (int c = m_col; c >= 1; c--)
		  {
			  if (m_arena->destroyRobot(m_row,c)) 
				  return true;
		  }
          break;
      case RIGHT:
		   for (int d = m_col; d <= m_arena->cols(); d++)
		   {
			   if (m_arena->destroyRobot(m_row,d))
				   return true;
		   }
           break;
	}
    return false; 
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementations
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = NULL;
    m_nRobots = 0;
}

Arena::~Arena()
{
	delete m_player;
	for (int k = 0; k < m_nRobots; k++)
	     delete m_robots[k];	
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols; 
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::robotCount() const
{
    return m_nRobots;
}

int Arena::nRobotsAt(int r, int c) const
{
	int numberOfRobots = 0;
	for (int k = 0; k < m_nRobots; k++)
	{
		if (m_robots[k]->row() == r && m_robots[k]->col() == c)
			numberOfRobots++;
	}
    return numberOfRobots;  
}

void Arena::display(string msg) const
{
      // Position (row,col) in the arena coordinate system is represented in
      // the array element grid[row-1][col-1]
    char grid[MAXROWS][MAXCOLS];
    int r, c;
    
        // Fill the grid with dots
    for (r = 0; r < rows(); r++)
        for (c = 0; c < cols(); c++)
            grid[r][c] = '.';

        // Indicate each robot's position
	for (r = 0; r < rows(); r++)
	{	
		for (c = 0; c < cols(); c++)
		{
			if (nRobotsAt(r+1,c+1) == 1)
				grid[r][c] = 'R';
			else if (nRobotsAt(r+1,c+1) > 1)
			{
				switch (nRobotsAt(r+1,c+1))
				{
					    case 2:
						    grid[r][c] = '2';
						    break;
					    case 3:
				  		    grid[r][c] = '3';
						    break;
					    case 4:
					    	grid[r][c] = '4';
						    break;
					    case 5:
						    grid[r][c] = '5';
						    break;
				    	case 6:
					    	grid[r][c] = '6';
						    break;
					    case 7:
					    	grid[r][c] = '7';
						    break;
					    case 8:
						    grid[r][c] = '8';
						    break;
					    default:
						    grid[r][c] = '9';	
				}
			}
		}
	}


        // Indicate player's position
    if (m_player != NULL)
    {
          // Set the char to '@', unless there's also a robot there,
          // in which case set it to '*'.
        char& gridChar = grid[m_player->row()-1][m_player->col()-1];
        if (gridChar == '.')
            gridChar = '@';
        else
            gridChar = '*';
    }

        // Draw the grid
    clearScreen();
    for (r = 0; r < rows(); r++)
    {
        for (c = 0; c < cols(); c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;

        // Write message, robot, and player info
    cout << endl;
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;
    if (m_player == NULL)
        cout << "There is no player." << endl;
    else
    {
        if (m_player->age() > 0)
            cout << "The player has lasted " << m_player->age() << " steps." << endl;
        if (m_player->isDead())
            cout << "The player is dead." << endl;
    }
}

bool Arena::addRobot(int r, int c)
{
	if (m_nRobots + 1 > MAXROBOTS)
		return false;
	else
	{
	    m_robots[m_nRobots] = new Robot(this, r, c);
	    m_nRobots++;
	    return true;
	}
}

bool Arena::addPlayer(int r, int c)
{
      // Don't add a player if one already exists
    if (m_player != NULL)
        return false;

      // Dynamically allocate a new Player and add it to the arena
    m_player = new Player(this, r, c);
    return true;
}

bool Arena::destroyRobot(int r, int c)
{
	for (int j = 0; j < m_nRobots; j++)
	{
		if (((m_robots[j]->row()) == r) && ((m_robots[j]->col()) == c))
		{   
			delete m_robots[j];
			if (j != m_nRobots-1)
			{
				for (int f = j; f < m_nRobots-1; f++)
			    {
				    m_robots[f] = m_robots[f+1];
			    }
			}
			m_nRobots--;
		    return true;
		}
	}
    return false;  
}

bool Arena::moveRobots()
{
    for (int x = 0; x < m_nRobots; x++)
    {
		m_robots[x]->move();
		if (m_robots[x]->row() == m_player->row() && m_robots[x]->col() == m_player->col())
			m_player->setDead();
    }

      // return true if the player is still alive, false otherwise
    return ! m_player->isDead();
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRobots)
{
    if (nRobots > MAXROBOTS)
    {
        cout << "***** Trying to create Game with " << nRobots
             << " robots; only " << MAXROBOTS << " are allowed!" << endl;
        exit(1);
    }

        // Create arena
    m_arena = new Arena(rows, cols);

        // Add player
    int rPlayer = 1 + rand() % rows;
    int cPlayer = 1 + rand() % cols;
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with robots
    while (nRobots > 0)
    {
        int r = 1 + rand() % rows;
        int c = 1 + rand() % cols;
          // Don't put a robot where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_arena->addRobot(r, c);
        nRobots--;
    }
}

Game::~Game()
{
    delete m_arena;
}

void Game::play()
{
    Player* p = m_arena->player();
    if (p == NULL)
    {
        m_arena->display("");
        return;
    }
    string msg = "";
    do
    {
        m_arena->display(msg);
        msg = "";
        cout << endl;
        cout << "Move (u/d/l/r/su/sd/sl/sr/c//q): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)
            p->stand();
        else
        {
            switch (action[0])
            {
              default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
              case 'q':
                return;
              case 'c':  // computer moves player
                msg = p->takeComputerChosenTurn();
                break;
              case 'u':
              case 'd':
              case 'l':
              case 'r':
                p->move(decodeDirection(action[0]));
                break;
              case 's':
                if (action.size() < 2)  // if no direction, nobody moves
                {
                    cout << '\a' << endl;  // beep
                    continue;
                }
                switch (action[1])
                {
                  default:   // if bad direction, nobody moves
                    cout << '\a' << endl;  // beep
                    continue;
                  case 'u':
                  case 'd':
                  case 'l':
                  case 'r':
                    if (p->shoot(decodeDirection(action[1])))
                        msg = "Hit!";
                    else
                        msg = "Missed!";
                    break;
                }
                break;
            }
        }
        m_arena->moveRobots();
    } while ( ! m_arena->player()->isDead()  &&  m_arena->robotCount() > 0);
    m_arena->display(msg);
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'u':  return UP;
      case 'd':  return DOWN;
      case 'l':  return LEFT;
      case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                                                            &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <cstring>

void clearScreen()
{
    static const char* term = getenv("TERM");
    static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
    if (term == NULL  ||  strcmp(term, "dumb") == 0)
        cout << endl;
     else
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
}

#endif

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{

    srand(static_cast<unsigned int>(time(0)));

      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 3, 2);
    Game g(15, 18, 100);

      // Play the game
    g.play();
}