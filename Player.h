//
// Created by User on 11/15/2022.
//
#ifndef WET1DS_PLAYER_H
#define WET1DS_PLAYER_H

class Team;
class Player {
private:
    int m_id;

    int m_teamId;

    int m_gamesPlayed;

    int m_goals;

    int m_cards;

    int m_teamGamesBeforeJoin;

    bool m_goalKeeper;

    Player* m_closestRight;

    Player* m_closestLeft;

    Team* m_team;

public:
    Player(int playerId=0, int teamId=0, int gamesPlayed=0, int goals=0, int cards=0, bool goalKeeper=false);

    int getId() const;

    int getTeamId() const;

    void setTeamId(int id);

    int getPlayerCards() const;

    int getGoals() const;

    int getTeamGamesBeforeJoin() const;

    int getGamesPlayed() const;

    Player* getClosestPlayerRight() const;

    Player* getClosestPlayerLeft() const;

    Team* getTeam() const;

    bool isGoalKeeper() const;

    void addGamesPlayed(int games);

    void addGoals(int goals);

    void addCards(int cards);

    void setTeam(Team* newTeam);

    void setClosestPlayerRight(Player* newClosest);

    void setClosestPlayerLeft(Player* newClosest);

    void setTeamGamesBeforeJoin(int teamGamesBeforeJoin);

    ~Player() = default;
};

bool isBiggerIdPlayer(Player& p1, Player& p2);

bool isBiggerStats(Player& p1, Player& p2);
#endif //WET1DS_PLAYER_H
