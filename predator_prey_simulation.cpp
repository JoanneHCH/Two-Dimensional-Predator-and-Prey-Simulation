#include <iostream>
#include <vector>
#include <random>

using namespace std;

const int kGridSize = 20;
const int kStarveToDeath = 3;
const int kAntBreedPeriod = 3;
const int kDoodleBugBreedPeriod = 8;
const int kDirectionVector[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

enum Direction {
    kUp = 0,
    kDown,
    kLeft,
    kRight,
    kNone,
};

bool CheckAvailability(int row, int col, char (&cell_)[kGridSize][kGridSize]) {
    return row >= 0 && row < kGridSize && col >= 0 && col < kGridSize && cell_[row][col] == '-';
}

class Organism {
public:
    char (&cell_)[kGridSize][kGridSize];
    Organism* (&bugs_)[kGridSize][kGridSize];
    Organism(int row, int col, char symbol, int time_step, char (&cell)[kGridSize][kGridSize], Organism* (&bugs)[kGridSize][kGridSize]) :
              row_(row), col_(col), symbol_(symbol), time_step_(time_step), cell_(cell), bugs_(bugs), survive_time_(0){
        cell_[row_][col_] = symbol_;
    }
    ~Organism() {
        cell_[row_][col_] = '-';
        bugs_[row_][col_] = nullptr;
    }
    void UpdateTimeStep() {++time_step_;}
    void UpdateSurviveTime() {++survive_time_;}
    int GetTimeStep() const {return time_step_;}
    int GetSurviveTime() const {return survive_time_;}
    int GetRow() const {return row_;}
    int GetCol() const {return col_;}
    char GetSymbol() const {return symbol_;}
    Direction GetAvailableDirection(char symbol='-');
    void MoveTo(Direction direction=kNone);
    virtual void Move() = 0;
private:
    int row_;
    int col_;
    int time_step_;
    int survive_time_;
    const char symbol_;
};

void Organism::MoveTo(Direction direction) {
    if (direction == kNone)
        direction = static_cast<Direction>(rand() % 4);
    int next_row = row_ + kDirectionVector[direction][0];
    int next_col = col_ + kDirectionVector[direction][1];
    if (CheckAvailability(next_row, next_col, cell_)) {
        cell_[next_row][next_col] = symbol_;
        cell_[row_][col_] = '-';
        bugs_[next_row][next_col] = bugs_[row_][col_];
        bugs_[row_][col_] = nullptr;
        row_ = next_row;
        col_ = next_col;
    }
}

Direction Organism::GetAvailableDirection(char symbol) {
    vector<Direction> available_directions;
    if (row_ > 0 && cell_[row_ - 1][col_] == symbol)
        available_directions.push_back(kUp);
    if (row_ < kGridSize - 1 && cell_[row_ + 1][col_] == symbol)
        available_directions.push_back(kDown);
    if (col_ > 0 && cell_[row_][col_ - 1] == symbol)
        available_directions.push_back(kLeft);
    if (col_ < kGridSize - 1 && cell_[row_][col_ + 1] == symbol)
        available_directions.push_back(kRight);
    if (available_directions.empty())
        return kNone;
    else
        return available_directions[rand() % available_directions.size()];
}

class Ant : public Organism {
public:
    Ant(int row, int col, int time_step, char (&cell)[kGridSize][kGridSize], Organism* (&bugs)[kGridSize][kGridSize])
     : Organism(row, col, 'o', time_step, cell, bugs) {}
    void Breed();
    void Move();
};

void Ant::Breed() {
    Direction direction = GetAvailableDirection();
    if (direction != kNone) {
        int breed_row = GetRow() + kDirectionVector[direction][0];
        int breed_col = GetCol() + kDirectionVector[direction][1];
        bugs_[breed_row][breed_col] = new Ant(breed_row, breed_col, GetTimeStep(), cell_, bugs_);
    }
}

void Ant::Move() {
    MoveTo();
    UpdateTimeStep();
    UpdateSurviveTime();
    if (GetSurviveTime() % kAntBreedPeriod == 0) {
        Breed();
    }
}

class DoodleBug : public Organism {
public:
    DoodleBug(int row, int col, int time_step, char (&cell)[kGridSize][kGridSize], Organism* (&bugs)[kGridSize][kGridSize])
     : Organism(row, col, 'X', time_step, cell, bugs), starve_time_(0) {}
    void Breed();
    void Move();
    int GetStarveTime() {return starve_time_;}
private:
    int starve_time_;
};

void DoodleBug::Breed() {
    Direction direction = GetAvailableDirection();
    if (direction != kNone) {
        int breed_row = GetRow() + kDirectionVector[direction][0];
        int breed_col = GetCol() + kDirectionVector[direction][1];
        bugs_[breed_row][breed_col] = new DoodleBug(breed_row, breed_col, GetTimeStep(), cell_, bugs_);
    }
}

void DoodleBug::Move() {
    Direction ant_direction = GetAvailableDirection('o');
    if (ant_direction != kNone) {
        int next_row = GetRow() + kDirectionVector[ant_direction][0];
        int next_col = GetCol() + kDirectionVector[ant_direction][1];
        static_cast<Ant*>(bugs_[next_row][next_col])->~Ant();
        MoveTo(ant_direction);
        starve_time_ = 0;
    } else {
        MoveTo();
        ++starve_time_;
    }
    UpdateTimeStep();
    UpdateSurviveTime();
    if (starve_time_ == kStarveToDeath) {
        this->~DoodleBug();
    }
    if (GetSurviveTime() % kDoodleBugBreedPeriod == 0) {
        Breed();
    }
}

class World {
public:
    World(int ants, int doodlebugs) {
        time_step_ = 0;
        for (int i = 0; i < kGridSize; ++i) {
            for (int j = 0; j < kGridSize; ++j) {
                cell_[i][j] = '-';
                bugs_[i][j] = nullptr;
            }
        }
        CreateAnt(ants);
        CreateDoodleBugs(doodlebugs);
        Print();
    }
    void CreateAnt(int ants) {
        while (ants > 0) {
            int row = rand() % kGridSize;
            int col = rand() % kGridSize;
            if (bugs_[row][col] == nullptr) {
                bugs_[row][col] = new Ant(row, col, time_step_, cell_, bugs_);
                --ants;
            }
        }
    }
    void CreateDoodleBugs(int doodlebugs) {
        while (doodlebugs > 0) {
            int row = rand() % kGridSize;
            int col = rand() % kGridSize;
            if (bugs_[row][col] == nullptr) {
                bugs_[row][col] = new DoodleBug(row, col, time_step_, cell_, bugs_);
                --doodlebugs;
            }
        }
    }
    void Update() {
        ++time_step_;
        for (int i = 0; i < kGridSize; ++i) {
            for (int j = 0; j < kGridSize; ++j) {
                if (bugs_[i][j] != nullptr && bugs_[i][j]->GetSymbol() == 'X' && bugs_[i][j]->GetTimeStep() != time_step_) {
                    bugs_[i][j]->Move();
                }
            }
        }
        for (int i = 0; i < kGridSize; ++i) {
            for (int j = 0; j < kGridSize; ++j) {
                if (bugs_[i][j] != nullptr && bugs_[i][j]->GetSymbol() == 'o' && bugs_[i][j]->GetTimeStep() != time_step_)
                    bugs_[i][j]->Move();
            }
        }
        Print();
    }
    void Print() {
        for (int i = 0; i < kGridSize; ++i) {
            for (int j = 0; j < kGridSize; ++j) {
                cout << cell_[i][j];
            }
            cout << endl;
        }
    }
private:
    int time_step_;
    char cell_[kGridSize][kGridSize];
    Organism* bugs_[kGridSize][kGridSize];
};

int main() {
    srand(time(0));
    World bugs_world(100, 5);
    cout << "Press ENTER to continue\n";
    while(cin.get() == '\n') {
        bugs_world.Update();
        cout << "Press ENTER to continue\n";
    }
    return 0;
}
